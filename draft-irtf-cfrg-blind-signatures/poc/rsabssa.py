#! /usr/bin/env python3

import time
import json
from typing import List, Dict
from Crypto.Util.number import size, inverse, ceil_div, GCD
from Crypto.Hash import SHA384
from Crypto.Util.strxor import strxor
from Crypto.PublicKey import RSA
from Crypto.PublicKey.RSA import RsaKey
from Crypto.Signature import pss
from Crypto.Signature.pss import MGF1

from os import urandom

H = SHA384
MgfHash = SHA384


def wrap_print(arg, *args):
    line_length = 68
    string = arg + " " + " ".join(args)
    for hunk in (
        string[0 + i : line_length + i] for i in range(0, len(string), line_length)
    ):
        if hunk and len(hunk.strip()) > 0:
            print(hunk)


def OS2IP(b: bytes) -> int:
    return int.from_bytes(b, byteorder="big")


def I2OSP(i: int, length: int) -> bytes:
    return i.to_bytes(length=length, byteorder="big")


def to_hex(n):
    return hex(n)[2:]


def EMSA_PSS_ENCODE(kBits: int, msg: bytes, sLen: int, salt: bytes = None) -> bytes:
    m_hash = H.new(msg).digest()
    hLen = H.digest_size

    emBits = kBits - 1
    emLen = ceil_div(emBits, 8)
    assert emLen >= hLen + sLen + 2

    lmask = 0
    for _ in range(0, 8 * emLen - emBits):
        lmask = lmask >> 1 | 0x80

    if salt is None:  # for test vector verification
        salt = urandom(sLen)

    m_prime = bytes(8) + m_hash + salt
    h = MgfHash.new(m_prime).digest()
    ps = bytes(emLen - sLen - hLen - 2)
    db = ps + bytes([0x01]) + salt
    dbMask = MGF1(h, emLen - hLen - 1, MgfHash)
    masked_db = strxor(db, dbMask)
    masked_db = bytes([masked_db[0] & (~lmask)]) + masked_db[1:]
    encoded_msg = masked_db + h + bytes([0xBC])

    wrap_print("salt = {}".format(salt.hex()))
    wrap_print("encoded_msg = {}".format(encoded_msg.hex()))

    return encoded_msg


def RSAVP1(public_key: RsaKey, r: int) -> int:
    e = public_key.e
    n = public_key.n
    return pow(r, e, n)


def random_integer_uniform(min: int, max: int) -> int:
    # Implement rejection sampling to return an integer in [min, max).
    # This is for reference only; most cryptographic libraries include
    # functions to generate random integers from a uniform distribution.
    range = max - min
    rangeBits = size(range - 1)
    rangeLen = ceil_div(rangeBits, 8)
    mask = (1 << rangeBits) - 1
    while True:
        randomBytes = urandom(rangeLen)
        r = OS2IP(randomBytes) & mask
        if r < range:
            return min + r


def is_coprime(a, b):
    return GCD(a, b) == 1


def rsabssa_randomize(msg: bytes) -> bytes:
    msg_prefix = urandom(32)

    wrap_print("msg_prefix = {}".format(msg_prefix.hex()))

    return msg_prefix + msg


def rsabssa_blind(
    public_key: RsaKey, msg: bytes, sLen: int, r_inv: int = None, salt: bytes = None
) -> tuple[bytes, bytes]:
    n = public_key.n
    kBits = n.bit_length()
    kLen = ceil_div(kBits, 8)
    encoded_msg = EMSA_PSS_ENCODE(kBits, msg, sLen, salt)
    m = OS2IP(encoded_msg)

    if not is_coprime(m, n):
        raise "Invalid input"

    if r_inv is not None:  # for test vector verification
        r = inverse(r_inv, n)
    else:
        r = random_integer_uniform(1, n)
        try:
            r_inv = inverse(r, n)
        except ValueError:
            raise "Invalid blind"

    x = RSAVP1(public_key, r)
    z = (m * x) % n
    blinded_msg = I2OSP(z, length=kLen)
    inv = I2OSP(r_inv, length=kLen)

    return blinded_msg, inv


def rsabssa_blind_sign(secret_key: RsaKey, blinded_msg: bytes) -> bytes:
    kLen = secret_key.size_in_bytes()
    if len(blinded_msg) != kLen:
        raise "Unexpected input size"
    n = secret_key.n
    d = secret_key.d
    m = OS2IP(blinded_msg)
    if m >= n:
        raise "Invalid message length"
    s = pow(m, d, n)
    blind_sig = I2OSP(s, length=kLen)
    return blind_sig


def rsabssa_finalize(
    public_key: RsaKey, blind_sig: bytes, inv: bytes, msg: bytes, sLen: int
) -> bytes:
    kLen = public_key.size_in_bytes()
    n = public_key.n
    z = OS2IP(blind_sig)
    r_inv = OS2IP(inv)
    s = (z * r_inv) % n
    sig = I2OSP(s, length=kLen)
    rsa_pss_verify(public_key, sig, msg, sLen)
    return sig


def rsa_pss_verify(public_key: RsaKey, sig: bytes, msg: bytes, sLen: int) -> None:
    mgf = lambda x, y: MGF1(x, y, MgfHash)
    verifier = pss.new(public_key, salt_bytes=sLen, mask_func=mgf)
    verifier.verify(H.new(msg), sig)  # standard RSASSA-PSS/MGF1 verification function


def test(
    description: str,
    msg: bytes,
    secret_key: RsaKey,
    r_inv: int = None,
    sLen: int = 48,
    salt: bytes = None,
    randomize: bool = False,
) -> None:
    print("\n## {} Test Vector".format(description))

    print("~~~")

    wrap_print("p = {}".format(to_hex(secret_key.p)))
    wrap_print("q = {}".format(to_hex(secret_key.q)))
    wrap_print("n = {}".format(to_hex(secret_key.n)))
    wrap_print("e = {}".format(to_hex(secret_key.e)))
    wrap_print("d = {}".format(to_hex(secret_key.d)))

    wrap_print("msg = {}".format(msg.hex()))
    random_msg = msg
    if randomize:
        random_msg = rsabssa_randomize(msg)
    wrap_print("random_msg = {}".format(random_msg.hex()))

    public_key = secret_key.public_key()
    blinded_msg, inv = None, None
    while True:
        try:
            blinded_msg, inv = rsabssa_blind(public_key, msg, sLen, r_inv, salt)
        finally:
            break
    blind_sig = rsabssa_blind_sign(secret_key, blinded_msg)
    sig = rsabssa_finalize(public_key, blind_sig, inv, msg, sLen)

    wrap_print("inv = {}".format(inv.hex()))
    wrap_print("blinded_msg = {}".format(blinded_msg.hex()))
    wrap_print("blind_sig = {}".format(blind_sig.hex()))
    wrap_print("sig = {}".format(sig.hex()))
    print("~~~")


def test_RSABSSA_SHA384_PSS_Randomized() -> None:
    msg = I2OSP(
        0x8F3DC6FB8C4A02F4D6352EDF0907822C1210A9B32F9BDDA4C45A698C80023AA6B59F8CFEC5FDBB36331372EBEFEDAE7D,
        length=48,
    )
    n = 0xAEC4D69ADDC70B990EA66A5E70603B6FEE27AAFEBD08F2D94CBE1250C556E047A928D635C3F45EE9B66D1BC628A03BAC9B7C3F416FE20DABEA8F3D7B4BBF7F963BE335D2328D67E6C13EE4A8F955E05A3283720D3E1F139C38E43E0338AD058A9495C53377FC35BE64D208F89B4AA721BF7F7D3FEF837BE2A80E0F8ADF0BCD1EEC5BB040443A2B2792FDCA522A7472AED74F31A1EBE1EEBC1F408660A0543DFE2A850F106A617EC6685573702EAAA21A5640A5DCAF9B74E397FA3AF18A2F1B7C03BA91A6336158DE420D63188EE143866EE415735D155B7C2D854D795B7BC236CFFD71542DF34234221A0413E142D8C61355CC44D45BDA94204974557AC2704CD8B593F035A5724B1ADF442E78C542CD4414FCE6F1298182FB6D8E53CEF1ADFD2E90E1E4DEEC52999BDC6C29144E8D52A125232C8C6D75C706EA3CC06841C7BDA33568C63A6C03817F722B50FCF898237D788A4400869E44D90A3020923DC646388ABCC914315215FCD1BAE11B1C751FD52443AAC8F601087D8D42737C18A3FA11ECD4131ECAE017AE0A14ACFC4EF85B83C19FED33CFD1CD629DA2C4C09E222B398E18D822F77BB378DEA3CB360B605E5AA58B20EDC29D000A66BD177C682A17E7EB12A63EF7C2E4183E0D898F3D6BF567BA8AE84F84F1D23BF8B8E261C3729E2FA6D07B832E07CDDD1D14F55325C6F924267957121902DC19B3B32948BDEAD5
    e = 0x010001
    d = 0x0D43242AEFE1FB2C13FBC66E20B678C4336D20B1808C558B6E62AD16A287077180B177E1F01B12F9C6CD6C52630257CCEF26A45135A990928773F3BD2FC01A313F1DAC97A51CEC71CB1FD7EFC7ADFFDEB05F1FB04812C924ED7F4A8269925DAD88BD7DCFBC4EF01020EBFC60CB3E04C54F981FDBD273E69A8A58B8CEB7C2D83FBCBD6F784D052201B88A9848186F2A45C0D2826870733E6FD9AA46983E0A6E82E35CA20A439C5EE7B502A9062E1066493BDADF8B49EB30D9558ED85ABC7AFB29B3C9BC644199654A4676681AF4BABCEA4E6F71FE4565C9C1B85D9985B84EC1ABF1A820A9BBEBEE0DF1398AAE2C85AB580A9F13E7743AFD3108EB32100B870648FA6BC17E8ABAC4D3C99246B1F0EA9F7F93A5DD5458C56D9F3F81FF2216B3C3680A13591673C43194D8E6FC93FC1E37CE2986BD628AC48088BC723D8FBE293861CA7A9F4A73E9FA63B1B6D0074F5DEA2A624C5249FF3AD811B6255B299D6BC5451BA7477F19C5A0DB690C3E6476398B1483D10314AFD38BBAF6E2FBDBCD62C3CA9797A420CA6034EC0A83360A3EE2ADF4B9D4BA29731D131B099A38D6A23CC463DB754603211260E99D19AFFC902C915D7854554AABF608E3AC52C19B8AA26AE042249B17B2D29669B5C859103EE53EF9BDC73BA3C6B537D5C34B6D8F034671D7F3A8A6966CC4543DF223565343154140FD7391C7E7BE03E241F4ECFEB877A051
    p = 0xE1F4D7A34802E27C7392A3CEA32A262A34DC3691BD87F3F310DC75673488930559C120FD0410194FB8A0DA55BD0B81227E843FDCA6692AE80E5A5D414116D4803FCA7D8C30EAAAE57E44A1816EBB5C5B0606C536246C7F11985D731684150B63C9A3AD9E41B04C0B5B27CB188A692C84696B742A80D3CD00AB891F2457443DADFEBA6D6DAF108602BE26D7071803C67105A5426838E6889D77E8474B29244CEFAF418E381B312048B457D73419213063C60EE7B0D81820165864FEF93523C9635C22210956E53A8D96322493FFC58D845368E2416E078E5BCB5D2FD68AE6ACFA54F9627C42E84A9D3F2774017E32EBCA06308A12ECC290C7CD1156DCCCFB2311
    q = 0xC601A9CAEA66DC3835827B539DB9DF6F6F5AE77244692780CD334A006AB353C806426B60718C05245650821D39445D3AB591ED10A7339F15D83FE13F6A3DFB20B9452C6A9B42EAA62A68C970DF3CADB2139F804AD8223D56108DFDE30BA7D367E9B0A7A80C4FDBA2FD9DDE6661FC73FC2947569D2029F2870FC02D8325ACF28C9AFA19ECF962DAA7916E21AFAD09EB62FE9F1CF91B77DC879B7974B490D3EBD2E95426057F35D0A3C9F45F79AC727AB81A519A8B9285932D9B2E5CCD347E59F3F32AD9CA359115E7DA008AB7406707BD0E8E185A5ED8758B5BA266E8828F8D863AE133846304A2936AD7BC7C9803879D2FC4A28E69291D73DBD799F8BC238385
    r_inv = 0x80682C48982407B489D53D1261B19EC8627D02B8CDA5336750B8CEE332AE260DE57B02D72609C1E0E9F28E2040FC65B6F02D56DBD6AA9AF8FDE656F70495DFB723BA01173D4707A12FDDAC628CA29F3E32340BD8F7DDB557CF819F6B01E445AD96F874BA235584EE71F6581F62D4F43BF03F910F6510DEB85E8EF06C7F09D9794A008BE7FF2529F0EBB69DECEF646387DC767B74939265FEC0223AA6D84D2A8A1CC912D5CA25B4E144AB8F6BA054B54910176D5737A2CFF011DA431BD5F2A0D2D66B9E70B39F4B050E45C0D9C16F02DEDA9DDF2D00F3E4B01037D7029CD49C2D46A8E1FC2C0C17520AF1F4B5E25BA396AFC4CD60C494A4C426448B35B49635B337CFB08E7C22A39B256DD032C00ADDDAFB51A627F99A0E1704170AC1F1912E49D9DB10EC04C19C58F420212973E0CB329524223A6AA56C7937C5DFFDB5D966B6CD4CBC26F3201DD25C80960A1A111B32947BB78973D269FAC7F5186530930ED19F68507540EED9E1BAB8B00F00D8CA09B3F099AAE46180E04E3584BD7CA054DF18A1504B89D1D1675D0966C4AE1407BE325CDF623CF13FF13E4A28B594D59E3EADBADF6136EEE7A59D6A444C9EB4E2198E8A974F27A39EB63AF2C9AF3870488B8ADAAD444674F512133AD80B9220E09158521614F1FAADFE8505EF57B7DF6813048603F0DD04F4280177A11380FBFC861DBCBD7418D62155248DAD5FDEC0991F
    sLen = 48
    salt = I2OSP(
        0x051722B35F458781397C3A671A7D3BD3096503940E4C4F1AAA269D60300CE449555CD7340100DF9D46944C5356825ABF,
        length=sLen,
    )
    secret_key = RSA.construct((n, e, d, p, q), consistency_check=True)
    test(
        "RSABSSA-SHA384-PSS-Randomized",
        msg=msg,
        secret_key=secret_key,
        r_inv=r_inv,
        sLen=sLen,
        salt=salt,
        randomize=True,
    )


def test_RSABSSA_SHA384_PSSZERO_Randomized() -> None:
    msg = I2OSP(
        0x8F3DC6FB8C4A02F4D6352EDF0907822C1210A9B32F9BDDA4C45A698C80023AA6B59F8CFEC5FDBB36331372EBEFEDAE7D,
        length=48,
    )
    n = 0xAEC4D69ADDC70B990EA66A5E70603B6FEE27AAFEBD08F2D94CBE1250C556E047A928D635C3F45EE9B66D1BC628A03BAC9B7C3F416FE20DABEA8F3D7B4BBF7F963BE335D2328D67E6C13EE4A8F955E05A3283720D3E1F139C38E43E0338AD058A9495C53377FC35BE64D208F89B4AA721BF7F7D3FEF837BE2A80E0F8ADF0BCD1EEC5BB040443A2B2792FDCA522A7472AED74F31A1EBE1EEBC1F408660A0543DFE2A850F106A617EC6685573702EAAA21A5640A5DCAF9B74E397FA3AF18A2F1B7C03BA91A6336158DE420D63188EE143866EE415735D155B7C2D854D795B7BC236CFFD71542DF34234221A0413E142D8C61355CC44D45BDA94204974557AC2704CD8B593F035A5724B1ADF442E78C542CD4414FCE6F1298182FB6D8E53CEF1ADFD2E90E1E4DEEC52999BDC6C29144E8D52A125232C8C6D75C706EA3CC06841C7BDA33568C63A6C03817F722B50FCF898237D788A4400869E44D90A3020923DC646388ABCC914315215FCD1BAE11B1C751FD52443AAC8F601087D8D42737C18A3FA11ECD4131ECAE017AE0A14ACFC4EF85B83C19FED33CFD1CD629DA2C4C09E222B398E18D822F77BB378DEA3CB360B605E5AA58B20EDC29D000A66BD177C682A17E7EB12A63EF7C2E4183E0D898F3D6BF567BA8AE84F84F1D23BF8B8E261C3729E2FA6D07B832E07CDDD1D14F55325C6F924267957121902DC19B3B32948BDEAD5
    e = 0x010001
    d = 0x0D43242AEFE1FB2C13FBC66E20B678C4336D20B1808C558B6E62AD16A287077180B177E1F01B12F9C6CD6C52630257CCEF26A45135A990928773F3BD2FC01A313F1DAC97A51CEC71CB1FD7EFC7ADFFDEB05F1FB04812C924ED7F4A8269925DAD88BD7DCFBC4EF01020EBFC60CB3E04C54F981FDBD273E69A8A58B8CEB7C2D83FBCBD6F784D052201B88A9848186F2A45C0D2826870733E6FD9AA46983E0A6E82E35CA20A439C5EE7B502A9062E1066493BDADF8B49EB30D9558ED85ABC7AFB29B3C9BC644199654A4676681AF4BABCEA4E6F71FE4565C9C1B85D9985B84EC1ABF1A820A9BBEBEE0DF1398AAE2C85AB580A9F13E7743AFD3108EB32100B870648FA6BC17E8ABAC4D3C99246B1F0EA9F7F93A5DD5458C56D9F3F81FF2216B3C3680A13591673C43194D8E6FC93FC1E37CE2986BD628AC48088BC723D8FBE293861CA7A9F4A73E9FA63B1B6D0074F5DEA2A624C5249FF3AD811B6255B299D6BC5451BA7477F19C5A0DB690C3E6476398B1483D10314AFD38BBAF6E2FBDBCD62C3CA9797A420CA6034EC0A83360A3EE2ADF4B9D4BA29731D131B099A38D6A23CC463DB754603211260E99D19AFFC902C915D7854554AABF608E3AC52C19B8AA26AE042249B17B2D29669B5C859103EE53EF9BDC73BA3C6B537D5C34B6D8F034671D7F3A8A6966CC4543DF223565343154140FD7391C7E7BE03E241F4ECFEB877A051
    p = 0xE1F4D7A34802E27C7392A3CEA32A262A34DC3691BD87F3F310DC75673488930559C120FD0410194FB8A0DA55BD0B81227E843FDCA6692AE80E5A5D414116D4803FCA7D8C30EAAAE57E44A1816EBB5C5B0606C536246C7F11985D731684150B63C9A3AD9E41B04C0B5B27CB188A692C84696B742A80D3CD00AB891F2457443DADFEBA6D6DAF108602BE26D7071803C67105A5426838E6889D77E8474B29244CEFAF418E381B312048B457D73419213063C60EE7B0D81820165864FEF93523C9635C22210956E53A8D96322493FFC58D845368E2416E078E5BCB5D2FD68AE6ACFA54F9627C42E84A9D3F2774017E32EBCA06308A12ECC290C7CD1156DCCCFB2311
    q = 0xC601A9CAEA66DC3835827B539DB9DF6F6F5AE77244692780CD334A006AB353C806426B60718C05245650821D39445D3AB591ED10A7339F15D83FE13F6A3DFB20B9452C6A9B42EAA62A68C970DF3CADB2139F804AD8223D56108DFDE30BA7D367E9B0A7A80C4FDBA2FD9DDE6661FC73FC2947569D2029F2870FC02D8325ACF28C9AFA19ECF962DAA7916E21AFAD09EB62FE9F1CF91B77DC879B7974B490D3EBD2E95426057F35D0A3C9F45F79AC727AB81A519A8B9285932D9B2E5CCD347E59F3F32AD9CA359115E7DA008AB7406707BD0E8E185A5ED8758B5BA266E8828F8D863AE133846304A2936AD7BC7C9803879D2FC4A28E69291D73DBD799F8BC238385
    r_inv = 0x80682C48982407B489D53D1261B19EC8627D02B8CDA5336750B8CEE332AE260DE57B02D72609C1E0E9F28E2040FC65B6F02D56DBD6AA9AF8FDE656F70495DFB723BA01173D4707A12FDDAC628CA29F3E32340BD8F7DDB557CF819F6B01E445AD96F874BA235584EE71F6581F62D4F43BF03F910F6510DEB85E8EF06C7F09D9794A008BE7FF2529F0EBB69DECEF646387DC767B74939265FEC0223AA6D84D2A8A1CC912D5CA25B4E144AB8F6BA054B54910176D5737A2CFF011DA431BD5F2A0D2D66B9E70B39F4B050E45C0D9C16F02DEDA9DDF2D00F3E4B01037D7029CD49C2D46A8E1FC2C0C17520AF1F4B5E25BA396AFC4CD60C494A4C426448B35B49635B337CFB08E7C22A39B256DD032C00ADDDAFB51A627F99A0E1704170AC1F1912E49D9DB10EC04C19C58F420212973E0CB329524223A6AA56C7937C5DFFDB5D966B6CD4CBC26F3201DD25C80960A1A111B32947BB78973D269FAC7F5186530930ED19F68507540EED9E1BAB8B00F00D8CA09B3F099AAE46180E04E3584BD7CA054DF18A1504B89D1D1675D0966C4AE1407BE325CDF623CF13FF13E4A28B594D59E3EADBADF6136EEE7A59D6A444C9EB4E2198E8A974F27A39EB63AF2C9AF3870488B8ADAAD444674F512133AD80B9220E09158521614F1FAADFE8505EF57B7DF6813048603F0DD04F4280177A11380FBFC861DBCBD7418D62155248DAD5FDEC0991F
    sLen = 48
    salt = I2OSP(
        0x051722B35F458781397C3A671A7D3BD3096503940E4C4F1AAA269D60300CE449555CD7340100DF9D46944C5356825ABF,
        length=sLen,
    )
    secret_key = RSA.construct((n, e, d, p, q), consistency_check=True)
    test(
        "RSABSSA-SHA384-PSSZERO-Randomized",
        msg=msg,
        secret_key=secret_key,
        r_inv=r_inv,
        sLen=sLen,
        salt=salt,
        randomize=True,
    )


def test_RSABSSA_SHA384_PSS_Deterministic() -> None:
    msg = I2OSP(
        0x8F3DC6FB8C4A02F4D6352EDF0907822C1210A9B32F9BDDA4C45A698C80023AA6B59F8CFEC5FDBB36331372EBEFEDAE7D,
        length=48,
    )
    n = 0xAEC4D69ADDC70B990EA66A5E70603B6FEE27AAFEBD08F2D94CBE1250C556E047A928D635C3F45EE9B66D1BC628A03BAC9B7C3F416FE20DABEA8F3D7B4BBF7F963BE335D2328D67E6C13EE4A8F955E05A3283720D3E1F139C38E43E0338AD058A9495C53377FC35BE64D208F89B4AA721BF7F7D3FEF837BE2A80E0F8ADF0BCD1EEC5BB040443A2B2792FDCA522A7472AED74F31A1EBE1EEBC1F408660A0543DFE2A850F106A617EC6685573702EAAA21A5640A5DCAF9B74E397FA3AF18A2F1B7C03BA91A6336158DE420D63188EE143866EE415735D155B7C2D854D795B7BC236CFFD71542DF34234221A0413E142D8C61355CC44D45BDA94204974557AC2704CD8B593F035A5724B1ADF442E78C542CD4414FCE6F1298182FB6D8E53CEF1ADFD2E90E1E4DEEC52999BDC6C29144E8D52A125232C8C6D75C706EA3CC06841C7BDA33568C63A6C03817F722B50FCF898237D788A4400869E44D90A3020923DC646388ABCC914315215FCD1BAE11B1C751FD52443AAC8F601087D8D42737C18A3FA11ECD4131ECAE017AE0A14ACFC4EF85B83C19FED33CFD1CD629DA2C4C09E222B398E18D822F77BB378DEA3CB360B605E5AA58B20EDC29D000A66BD177C682A17E7EB12A63EF7C2E4183E0D898F3D6BF567BA8AE84F84F1D23BF8B8E261C3729E2FA6D07B832E07CDDD1D14F55325C6F924267957121902DC19B3B32948BDEAD5
    e = 0x010001
    d = 0x0D43242AEFE1FB2C13FBC66E20B678C4336D20B1808C558B6E62AD16A287077180B177E1F01B12F9C6CD6C52630257CCEF26A45135A990928773F3BD2FC01A313F1DAC97A51CEC71CB1FD7EFC7ADFFDEB05F1FB04812C924ED7F4A8269925DAD88BD7DCFBC4EF01020EBFC60CB3E04C54F981FDBD273E69A8A58B8CEB7C2D83FBCBD6F784D052201B88A9848186F2A45C0D2826870733E6FD9AA46983E0A6E82E35CA20A439C5EE7B502A9062E1066493BDADF8B49EB30D9558ED85ABC7AFB29B3C9BC644199654A4676681AF4BABCEA4E6F71FE4565C9C1B85D9985B84EC1ABF1A820A9BBEBEE0DF1398AAE2C85AB580A9F13E7743AFD3108EB32100B870648FA6BC17E8ABAC4D3C99246B1F0EA9F7F93A5DD5458C56D9F3F81FF2216B3C3680A13591673C43194D8E6FC93FC1E37CE2986BD628AC48088BC723D8FBE293861CA7A9F4A73E9FA63B1B6D0074F5DEA2A624C5249FF3AD811B6255B299D6BC5451BA7477F19C5A0DB690C3E6476398B1483D10314AFD38BBAF6E2FBDBCD62C3CA9797A420CA6034EC0A83360A3EE2ADF4B9D4BA29731D131B099A38D6A23CC463DB754603211260E99D19AFFC902C915D7854554AABF608E3AC52C19B8AA26AE042249B17B2D29669B5C859103EE53EF9BDC73BA3C6B537D5C34B6D8F034671D7F3A8A6966CC4543DF223565343154140FD7391C7E7BE03E241F4ECFEB877A051
    p = 0xE1F4D7A34802E27C7392A3CEA32A262A34DC3691BD87F3F310DC75673488930559C120FD0410194FB8A0DA55BD0B81227E843FDCA6692AE80E5A5D414116D4803FCA7D8C30EAAAE57E44A1816EBB5C5B0606C536246C7F11985D731684150B63C9A3AD9E41B04C0B5B27CB188A692C84696B742A80D3CD00AB891F2457443DADFEBA6D6DAF108602BE26D7071803C67105A5426838E6889D77E8474B29244CEFAF418E381B312048B457D73419213063C60EE7B0D81820165864FEF93523C9635C22210956E53A8D96322493FFC58D845368E2416E078E5BCB5D2FD68AE6ACFA54F9627C42E84A9D3F2774017E32EBCA06308A12ECC290C7CD1156DCCCFB2311
    q = 0xC601A9CAEA66DC3835827B539DB9DF6F6F5AE77244692780CD334A006AB353C806426B60718C05245650821D39445D3AB591ED10A7339F15D83FE13F6A3DFB20B9452C6A9B42EAA62A68C970DF3CADB2139F804AD8223D56108DFDE30BA7D367E9B0A7A80C4FDBA2FD9DDE6661FC73FC2947569D2029F2870FC02D8325ACF28C9AFA19ECF962DAA7916E21AFAD09EB62FE9F1CF91B77DC879B7974B490D3EBD2E95426057F35D0A3C9F45F79AC727AB81A519A8B9285932D9B2E5CCD347E59F3F32AD9CA359115E7DA008AB7406707BD0E8E185A5ED8758B5BA266E8828F8D863AE133846304A2936AD7BC7C9803879D2FC4A28E69291D73DBD799F8BC238385
    r_inv = 0x80682C48982407B489D53D1261B19EC8627D02B8CDA5336750B8CEE332AE260DE57B02D72609C1E0E9F28E2040FC65B6F02D56DBD6AA9AF8FDE656F70495DFB723BA01173D4707A12FDDAC628CA29F3E32340BD8F7DDB557CF819F6B01E445AD96F874BA235584EE71F6581F62D4F43BF03F910F6510DEB85E8EF06C7F09D9794A008BE7FF2529F0EBB69DECEF646387DC767B74939265FEC0223AA6D84D2A8A1CC912D5CA25B4E144AB8F6BA054B54910176D5737A2CFF011DA431BD5F2A0D2D66B9E70B39F4B050E45C0D9C16F02DEDA9DDF2D00F3E4B01037D7029CD49C2D46A8E1FC2C0C17520AF1F4B5E25BA396AFC4CD60C494A4C426448B35B49635B337CFB08E7C22A39B256DD032C00ADDDAFB51A627F99A0E1704170AC1F1912E49D9DB10EC04C19C58F420212973E0CB329524223A6AA56C7937C5DFFDB5D966B6CD4CBC26F3201DD25C80960A1A111B32947BB78973D269FAC7F5186530930ED19F68507540EED9E1BAB8B00F00D8CA09B3F099AAE46180E04E3584BD7CA054DF18A1504B89D1D1675D0966C4AE1407BE325CDF623CF13FF13E4A28B594D59E3EADBADF6136EEE7A59D6A444C9EB4E2198E8A974F27A39EB63AF2C9AF3870488B8ADAAD444674F512133AD80B9220E09158521614F1FAADFE8505EF57B7DF6813048603F0DD04F4280177A11380FBFC861DBCBD7418D62155248DAD5FDEC0991F
    sLen = 0
    salt = bytes()
    secret_key = RSA.construct((n, e, d, p, q), consistency_check=True)
    test(
        "RSABSSA-SHA384-PSS-Deterministic",
        msg=msg,
        secret_key=secret_key,
        r_inv=r_inv,
        sLen=sLen,
        salt=salt,
        randomize=False,
    )


def test_RSABSSA_SHA384_PSSZERO_Deterministic() -> None:
    msg = I2OSP(
        0x8F3DC6FB8C4A02F4D6352EDF0907822C1210A9B32F9BDDA4C45A698C80023AA6B59F8CFEC5FDBB36331372EBEFEDAE7D,
        length=48,
    )
    n = 0xAEC4D69ADDC70B990EA66A5E70603B6FEE27AAFEBD08F2D94CBE1250C556E047A928D635C3F45EE9B66D1BC628A03BAC9B7C3F416FE20DABEA8F3D7B4BBF7F963BE335D2328D67E6C13EE4A8F955E05A3283720D3E1F139C38E43E0338AD058A9495C53377FC35BE64D208F89B4AA721BF7F7D3FEF837BE2A80E0F8ADF0BCD1EEC5BB040443A2B2792FDCA522A7472AED74F31A1EBE1EEBC1F408660A0543DFE2A850F106A617EC6685573702EAAA21A5640A5DCAF9B74E397FA3AF18A2F1B7C03BA91A6336158DE420D63188EE143866EE415735D155B7C2D854D795B7BC236CFFD71542DF34234221A0413E142D8C61355CC44D45BDA94204974557AC2704CD8B593F035A5724B1ADF442E78C542CD4414FCE6F1298182FB6D8E53CEF1ADFD2E90E1E4DEEC52999BDC6C29144E8D52A125232C8C6D75C706EA3CC06841C7BDA33568C63A6C03817F722B50FCF898237D788A4400869E44D90A3020923DC646388ABCC914315215FCD1BAE11B1C751FD52443AAC8F601087D8D42737C18A3FA11ECD4131ECAE017AE0A14ACFC4EF85B83C19FED33CFD1CD629DA2C4C09E222B398E18D822F77BB378DEA3CB360B605E5AA58B20EDC29D000A66BD177C682A17E7EB12A63EF7C2E4183E0D898F3D6BF567BA8AE84F84F1D23BF8B8E261C3729E2FA6D07B832E07CDDD1D14F55325C6F924267957121902DC19B3B32948BDEAD5
    e = 0x010001
    d = 0x0D43242AEFE1FB2C13FBC66E20B678C4336D20B1808C558B6E62AD16A287077180B177E1F01B12F9C6CD6C52630257CCEF26A45135A990928773F3BD2FC01A313F1DAC97A51CEC71CB1FD7EFC7ADFFDEB05F1FB04812C924ED7F4A8269925DAD88BD7DCFBC4EF01020EBFC60CB3E04C54F981FDBD273E69A8A58B8CEB7C2D83FBCBD6F784D052201B88A9848186F2A45C0D2826870733E6FD9AA46983E0A6E82E35CA20A439C5EE7B502A9062E1066493BDADF8B49EB30D9558ED85ABC7AFB29B3C9BC644199654A4676681AF4BABCEA4E6F71FE4565C9C1B85D9985B84EC1ABF1A820A9BBEBEE0DF1398AAE2C85AB580A9F13E7743AFD3108EB32100B870648FA6BC17E8ABAC4D3C99246B1F0EA9F7F93A5DD5458C56D9F3F81FF2216B3C3680A13591673C43194D8E6FC93FC1E37CE2986BD628AC48088BC723D8FBE293861CA7A9F4A73E9FA63B1B6D0074F5DEA2A624C5249FF3AD811B6255B299D6BC5451BA7477F19C5A0DB690C3E6476398B1483D10314AFD38BBAF6E2FBDBCD62C3CA9797A420CA6034EC0A83360A3EE2ADF4B9D4BA29731D131B099A38D6A23CC463DB754603211260E99D19AFFC902C915D7854554AABF608E3AC52C19B8AA26AE042249B17B2D29669B5C859103EE53EF9BDC73BA3C6B537D5C34B6D8F034671D7F3A8A6966CC4543DF223565343154140FD7391C7E7BE03E241F4ECFEB877A051
    p = 0xE1F4D7A34802E27C7392A3CEA32A262A34DC3691BD87F3F310DC75673488930559C120FD0410194FB8A0DA55BD0B81227E843FDCA6692AE80E5A5D414116D4803FCA7D8C30EAAAE57E44A1816EBB5C5B0606C536246C7F11985D731684150B63C9A3AD9E41B04C0B5B27CB188A692C84696B742A80D3CD00AB891F2457443DADFEBA6D6DAF108602BE26D7071803C67105A5426838E6889D77E8474B29244CEFAF418E381B312048B457D73419213063C60EE7B0D81820165864FEF93523C9635C22210956E53A8D96322493FFC58D845368E2416E078E5BCB5D2FD68AE6ACFA54F9627C42E84A9D3F2774017E32EBCA06308A12ECC290C7CD1156DCCCFB2311
    q = 0xC601A9CAEA66DC3835827B539DB9DF6F6F5AE77244692780CD334A006AB353C806426B60718C05245650821D39445D3AB591ED10A7339F15D83FE13F6A3DFB20B9452C6A9B42EAA62A68C970DF3CADB2139F804AD8223D56108DFDE30BA7D367E9B0A7A80C4FDBA2FD9DDE6661FC73FC2947569D2029F2870FC02D8325ACF28C9AFA19ECF962DAA7916E21AFAD09EB62FE9F1CF91B77DC879B7974B490D3EBD2E95426057F35D0A3C9F45F79AC727AB81A519A8B9285932D9B2E5CCD347E59F3F32AD9CA359115E7DA008AB7406707BD0E8E185A5ED8758B5BA266E8828F8D863AE133846304A2936AD7BC7C9803879D2FC4A28E69291D73DBD799F8BC238385
    r_inv = 0x80682C48982407B489D53D1261B19EC8627D02B8CDA5336750B8CEE332AE260DE57B02D72609C1E0E9F28E2040FC65B6F02D56DBD6AA9AF8FDE656F70495DFB723BA01173D4707A12FDDAC628CA29F3E32340BD8F7DDB557CF819F6B01E445AD96F874BA235584EE71F6581F62D4F43BF03F910F6510DEB85E8EF06C7F09D9794A008BE7FF2529F0EBB69DECEF646387DC767B74939265FEC0223AA6D84D2A8A1CC912D5CA25B4E144AB8F6BA054B54910176D5737A2CFF011DA431BD5F2A0D2D66B9E70B39F4B050E45C0D9C16F02DEDA9DDF2D00F3E4B01037D7029CD49C2D46A8E1FC2C0C17520AF1F4B5E25BA396AFC4CD60C494A4C426448B35B49635B337CFB08E7C22A39B256DD032C00ADDDAFB51A627F99A0E1704170AC1F1912E49D9DB10EC04C19C58F420212973E0CB329524223A6AA56C7937C5DFFDB5D966B6CD4CBC26F3201DD25C80960A1A111B32947BB78973D269FAC7F5186530930ED19F68507540EED9E1BAB8B00F00D8CA09B3F099AAE46180E04E3584BD7CA054DF18A1504B89D1D1675D0966C4AE1407BE325CDF623CF13FF13E4A28B594D59E3EADBADF6136EEE7A59D6A444C9EB4E2198E8A974F27A39EB63AF2C9AF3870488B8ADAAD444674F512133AD80B9220E09158521614F1FAADFE8505EF57B7DF6813048603F0DD04F4280177A11380FBFC861DBCBD7418D62155248DAD5FDEC0991F
    sLen = 0
    salt = bytes()
    secret_key = RSA.construct((n, e, d, p, q), consistency_check=True)
    test(
        "RSABSSA-SHA384-PSSZERO-Deterministic",
        msg=msg,
        secret_key=secret_key,
        r_inv=r_inv,
        sLen=sLen,
        salt=salt,
        randomize=False,
    )


def test_deterministic() -> None:
    msg = b"Test vector with deterministic padding"
    n = 0x98530F850DCC894D84ECFCE9DEC3A475BF30EC3CE4606F677AC4A6EF63F763FF64A162EF1C991D8094B5652D0D78C126B3E97D1D77EBA2F833B5BE9A124E003065EC2A3EA4FBC31BC283DE1C7CD8A971EB57AA7284B082562CCDE572B73702068A6143E6DABF886538FF419874C300A85F3D9D50F0731FC6B9C92A121FEFB7911F5EA92D25B17A4F3B2883EFF34A221B5C28C488E35067A8460D8FAB1C405704EBFA1CA165D69CD4E425995A03A447F6CBBA5D20D459707AB4A2C537A5DBD02801D7B19A03AAA9AEC21D1C363996C6B9FEE2CAB370D501C9B67E7DC4A20EB0CDC3B24BE242093B5A66119B96DA0FB0EC0B1B0DA0BD0B92236ECE47D5C95BDCA7
    e = 0x010001
    d = 0x6B15D18E4F8220709FE75F7226CA517EF9B7320D28DC66D54FA89A5727670F24C7A0F1857A0C6682338946A4A298E6E90788390E137553AFBBE2A4297A7EDD8128D61B68C8E1B96B7596F0FA0406E9308E2BA64735E344EDC237C97B993411B7796721AE54D05BDA1574D5AF913E59E30479B373E86676CB6566F7ADA0480D3AE21D50AC94C0B41C476E566D6BCDEF88EEAB3042EF1016527558E794B6029CFF1120596FE2104FAC928A66AD2FB1094D1AE1231ABF95206CAE7CD4E7AAD388199D7AC1FE17E3F917436232CFFE70E12056E02CFB9604E73CC34984BB83F7112ED197BF3A4D9F6D0C0E3C4DD8F2D9CBE17185F1E63561B08F7D14BD36112F3EA1
    p = 0xCA9D82E9059FA3B145DA850E0C451FF31093D819644BA29A3409393DE2ADFA1BCD65E8669A5C5140142C1404204EDBC380D4E7A5C866C06BB2427C76B9E3D16BBFC1B1668DEC219B8C59FEE90B7BAF557FC2FEB13F2F4B30D8606D20B9928F4F588A3B34BAA659B3BD1DD590C83E90E6251B5239FBBB73B12E90534A375E3F71
    q = 0xC075694F69DB6A07456E19EEACE01B430F2D6CC6CD5495D569E242B6F5E8DED7DF27E6AEEA4DB4E307554FB519B68279A58D9E2D25CEE4B37668554EEC2F2FEB79246955A07BD526F02A6AFEDC7A3AFF2B8953287FEF2C4A02207CCB9F14E4612E9AF3447DD3401728A8957871B759B6BBF22AA0E8271B82F32DD5A2D2550197
    r_inv = 0x6E69972553327EE6240CE0DE7146AEA2243927CF9F7F52C0103367DF79E3BAFEBFA61C2FFDC41EA397A38523654A1A806F4EEBCD5FE9A2592A463F1FAA26C3601F83F29141EDA488F14F7C0AA82FAA025E37ADBE77E02E575F72F7B9D095882923476F2328DFAEB23B607D2F706C6C8EF6C2AEE50DDB14E6D27E043E7DEC8E5DEDE6844AA80B2206B6019350D37925BB8819653AA7A13BFB9CC3C95B53378F278903B5C06A10C0B3CE0AA028E9600F7B2733F0278565F9B88E9D92E039DB78300170D7BBD32CE2B89AD8944167839880E3A2AEBA05BF00EDC8032A63E6279BF42A131CCC9BB95B8693764B27665274FB673BDFB7D69B7957EE8B64A99EFBEED9
    sLen = 0
    secret_key = RSA.construct((n, e, d, p, q), consistency_check=True)
    test(
        "deterministic",
        msg=msg,
        secret_key=secret_key,
        r_inv=r_inv,
        sLen=sLen,
        randomize=False,
    )


def generate_probabilistic_test_vector() -> None:
    secret_key = RSA.generate(2048)
    msg = urandom(32)
    sLen = 48
    test("new probabilistic test vector", msg=msg, secret_key=secret_key, sLen=sLen)


def generate_deterministic_test_vector() -> None:
    secret_key = RSA.generate(2048)
    msg = urandom(32)
    sLen = 0
    test("new deterministic test vector", msg=msg, secret_key=secret_key, sLen=sLen)


def performance_test() -> Dict[str, float]:
    runs = 25
    total = {
        "key_size": 0,
        "sig_size": 0,
        "key_gen_time": 0,
        "sign_time": 0,
        "verify_time": 0,
    }

    for _ in range(runs):
        # Key generation
        key_start_time = time.time()
        key = RSA.generate(3072)  # Generate a new RSA key pair
        key_gen_time = time.time() - key_start_time

        # Message creation
        msg = urandom(32)  # Generate a random 32-byte message

        # Signing
        sign_start_time = time.time()
        blinded_msg, inv = rsabssa_blind(key.publickey(), msg, sLen=32)
        blind_sig = rsabssa_blind_sign(key, blinded_msg)
        sig = rsabssa_finalize(key.publickey(), blind_sig, inv, msg, sLen=32)
        sign_time = time.time() - sign_start_time

        # Verification
        verify_start_time = time.time()
        rsa_pss_verify(key.publickey(), sig, msg, sLen=32)
        verify_time = time.time() - verify_start_time

        # Accumulate results
        total["key_size"] += key.size_in_bytes()
        total["sig_size"] += len(sig)
        total["key_gen_time"] += key_gen_time
        total["sign_time"] += sign_time
        total["verify_time"] += verify_time

    # Calculate averages
    averages = {k: v / runs for k, v in total.items()}

    return averages

if __name__ == "__main__":
    averages = performance_test()
    print(json.dumps(averages, indent=4))  # Pretty-print the results

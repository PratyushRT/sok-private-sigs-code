package main

import (
	"crypto/elliptic"
	"encoding/asn1"
	"fmt"
	pblind "github.com/rot256/pblind"
	"time"
)

func main() {
	var totalSetupTime, totalSignTime, totalVerifyTime time.Duration

	for i := 0; i < 3; i++ {
		startSetup := time.Now()

		// generate a key-pair
		curve := elliptic.P256()
		sk, _ := pblind.NewSecretKey(curve)
		pk := sk.GetPublicKey()

		msgStr := []byte("blinded message")
		infoStr := []byte("plaintext info")

		// create signer/requester with shared public info
		info, _ := pblind.CompressInfo(curve, infoStr)
		requester, _ := pblind.CreateRequester(pk, info, msgStr)
		signer, _ := pblind.CreateSigner(sk, info)

		endSetup := time.Now()
		totalSetupTime += endSetup.Sub(startSetup)

		startSign := time.Now()

		// signer
		msg1S, _ := signer.CreateMessage1()
		ser1S, _ := asn1.Marshal(msg1S)

		// requester
		var msg1R pblind.Message1
		asn1.Unmarshal(ser1S, &msg1R)
		requester.ProcessMessage1(msg1R)
		msg2R, _ := requester.CreateMessage2()
		ser2R, _ := asn1.Marshal(msg2R)

		// signer
		var msg2S pblind.Message2
		asn1.Unmarshal(ser2R, &msg2S)
		signer.ProcessMessage2(msg2S)
		msg3S, _ := signer.CreateMessage3()
		ser3S, _ := asn1.Marshal(msg3S)

		// requester
		var msg3R pblind.Message3
		asn1.Unmarshal(ser3S, &msg3R)
		requester.ProcessMessage3(msg3R)
		signature, _ := requester.Signature()

		endSign := time.Now()
		totalSignTime += endSign.Sub(startSign)

		startVerify := time.Now()

		// check signature
		pk.Check(signature, info, msgStr)

		endVerify := time.Now()
		totalVerifyTime += endVerify.Sub(startVerify)
	}

	fmt.Printf("Average setup time: %v\n", totalSetupTime.Seconds()/3)
	fmt.Printf("Average sign time: %v\n", totalSignTime.Seconds()/3)
	fmt.Printf("Average verify time: %v\n", totalVerifyTime.Seconds()/3)
}

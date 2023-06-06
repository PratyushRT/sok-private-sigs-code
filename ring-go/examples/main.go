package main

import (
	"fmt"
	"time"
	"encoding/binary"
	"golang.org/x/crypto/sha3"
	ring "github.com/noot/ring-go"
)

func signAndVerify(curve ring.Curve, size int) (setupTime time.Duration, signTime time.Duration, verifyTime time.Duration, signatureSize int, keySize int) {
	// Repeat experiment 10 times for averaging
	for i := 0; i < 10; i++ {
		privkey := curve.NewRandomScalar()
		msgHash := sha3.Sum256([]byte("helloworld"))
		
		// our key's secret index within the set
		const idx = 7

		// Setup
		setupStart := time.Now()
		keyring, err := ring.NewKeyRing(curve, size, privkey, idx)
		setupEnd := time.Now()

		if err != nil {
			panic(err)
		}

		// Sign
		signStart := time.Now()
		sig, err := keyring.Sign(msgHash, privkey)
		signEnd := time.Now()

		if err != nil {
			panic(err)
		}

		// Verify
		verifyStart := time.Now()
		ok := sig.Verify(msgHash)
		verifyEnd := time.Now()

		if !ok {
			fmt.Println("failed to verify :(")
			return
		}

		setupTime += setupEnd.Sub(setupStart)
		signTime += signEnd.Sub(signStart)
		verifyTime += verifyEnd.Sub(verifyStart)
		signatureSize += binary.Size(sig)
		keySize += binary.Size(keyring)
	}

	setupTime /= 10
	signTime /= 10
	verifyTime /= 10
	signatureSize /= 10
	keySize /= 10

	return
}

func main() {
	sizes := []int{32, 1048576}

	for _, size := range sizes {
		fmt.Printf("Ring Size: %d\n", size)

		fmt.Println("Using ed25519...")
		setupTime, signTime, verifyTime, signatureSize, keySize := signAndVerify(ring.Ed25519(), size)
		fmt.Printf("Setup time: %v, Sign time: %v, Verify time: %v, Signature size: %d, Key size: %d\n", setupTime, signTime, verifyTime, signatureSize, keySize)
	}
}

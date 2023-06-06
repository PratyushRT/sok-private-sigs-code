from linkable_ring_signature import ring_signature, verify_ring_signature
from ecdsa.util import randrange
from ecdsa.curves import SECP256k1
import time
import sys

def run_experiment(group_sizes):
    results = []
    for size in group_sizes:
        print(f"Group Size: {size}")
        setup_times = []
        sign_times = []
        verify_times = []
        signature_sizes = []
        key_sizes = []

        for _ in range(1):
            x = [randrange(SECP256k1.order) for _ in range(size)]
            y = list(map(lambda xi: SECP256k1.generator * xi, x))
            message = "Every move we made was a kiss"

            # Measure setup time
            setup_start = time.time()
            i = randrange(size)
            signature = tuple(ring_signature(x[i], i, message, y))
            setup_end = time.time()
            setup_time = setup_end - setup_start
            setup_times.append(setup_time)

            # Measure sign time
            sign_start = time.time()
            _ = ring_signature(x[i], i, message, y)
            sign_end = time.time()
            sign_time = sign_end - sign_start
            sign_times.append(sign_time)

            # Measure verify time
            verify_start = time.time()
            assert verify_ring_signature(message, y, *signature)
            verify_end = time.time()
            verify_time = verify_end - verify_start
            verify_times.append(verify_time)

            # Measure signature and key sizes
            signature_sizes.append(int(str(sys.getsizeof(signature))))
            key_sizes.append(int(str(sys.getsizeof(y))))

        # Calculate final average values
        avg_setup_time = sum(setup_times) / len(setup_times)
        avg_sign_time = sum(sign_times) / len(sign_times)
        avg_verify_time = sum(verify_times) / len(verify_times)
        avg_signature_size = sum(signature_sizes) / len(signature_sizes)
        avg_key_size = sum(key_sizes) / len(key_sizes)

        print(f"Average Setup Time: {avg_setup_time:.6f} seconds")
        print(f"Average Sign Time: {avg_sign_time:.6f} seconds")
        print(f"Average Verify Time: {avg_verify_time:.6f} seconds")
        print(f"Average Signature Size: {avg_signature_size:.2f} bits")
        print(f"Average Key Size: {avg_key_size:.2f} bits")

        results.append((size, avg_setup_time, avg_sign_time, avg_verify_time, avg_signature_size, avg_key_size))

    return results


group_sizes = [2**15, 2**20]
experiment_results = run_experiment(group_sizes)

for size, setup_time, sign_time, verify_time, signature_size, key_size in experiment_results:
    print(f"Group Size: {size}")
    print(f"Average Setup Time: {setup_time:.6f} seconds")
    print(f"Average Sign Time: {sign_time:.6f} seconds")
    print(f"Average Verify Time: {verify_time:.6f} seconds")
    print(f"Average Signature Size: {signature_size:.2f} bits")
    print(f"Average Key Size: {key_size:.2f} bits")
    print()

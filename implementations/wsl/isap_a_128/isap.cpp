#include <iostream>
#include <cstdint>
#include <chrono>
#include <cstring>

using namespace std;
using namespace std::chrono;

#define ROTR(x,n) (((x) >> (n)) | ((x) << (32 - (n))))

// ------------------ SIMULATED SIMD PERMUTATION ----------------------
void ascon_permutation_10r(uint32_t* S) {
    const uint32_t RC[10] = {
        0x0000000f, 0x0000001e, 0x0000002d, 0x0000003c, 0x0000004b,
        0x0000005a, 0x00000069, 0x00000078, 0x00000087, 0x00000096
    };

    for (int r = 0; r < 10; r++) {
        S[9] ^= RC[r];

        for (int i = 0; i < 10; i += 2) {
            uint32_t x0 = S[i];
            uint32_t x1 = S[i+1];
            S[i]   ^= ~x1 & x0;
            S[i+1] ^= ~x0 | x1;
        }

        for (int i = 0; i < 10; i += 2) {
            uint32_t t0 = S[i];
            uint32_t t1 = S[i+1];
            S[i]   = t0 ^ ROTR(t0, 19) ^ ROTR(t0, 28);
            S[i+1] = t1 ^ ROTR(t1, 1) ^ ROTR(t1, 6);
        }
    }
}

// ------------------ MOCK ISAP-A-128 ENCRYPT/DECRYPT -----------------
void isap_a_128_simd_core(const uint8_t* key, const uint8_t* nonce,
                          const uint8_t* input, size_t len,
                          uint8_t* output) {
    uint64_t state[5] = {0};

    memcpy(&state[0], key, 8);
    memcpy(&state[1], nonce, 8);
    state[2] = 0xc1c2c3c4c5c6c7c8;
    state[3] = 0;
    state[4] = 0;

    uint32_t state32[10];
    for (int i = 0; i < 5; ++i) {
        state32[2 * i]     = (uint32_t)(state[i] >> 32);
        state32[2 * i + 1] = (uint32_t)(state[i] & 0xFFFFFFFF);
    }

    ascon_permutation_10r(state32);

    for (int i = 0; i < 5; ++i) {
        state[i] = ((uint64_t)state32[2 * i] << 32) | state32[2 * i + 1];
    }

    for (size_t i = 0; i < len; ++i) {
        output[i] = input[i] ^ ((uint8_t*)&state[i % 5])[0];
    }
}

// ------------------ BENCHMARK HARNESS -------------------------------
void benchmark(size_t msg_len) {
    const int rounds = 10000;

    uint8_t key[16] = {0};
    uint8_t nonce[16] = {0};
    uint8_t plaintext[512] = {0};
    uint8_t ciphertext[512] = {0};
    uint8_t decrypted[512] = {0};

    for (size_t i = 0; i < msg_len; ++i)
        plaintext[i] = i;

    auto enc_start = high_resolution_clock::now();
    for (int i = 0; i < rounds; ++i)
        isap_a_128_simd_core(key, nonce, plaintext, msg_len, ciphertext);
    auto enc_end = high_resolution_clock::now();

    auto dec_start = high_resolution_clock::now();
    for (int i = 0; i < rounds; ++i)
        isap_a_128_simd_core(key, nonce, ciphertext, msg_len, decrypted);
    auto dec_end = high_resolution_clock::now();

    double enc_time = duration_cast<nanoseconds>(enc_end - enc_start).count() / (1000.0 * rounds);
    double dec_time = duration_cast<nanoseconds>(dec_end - dec_start).count() / (1000.0 * rounds);
    double throughput = (enc_time > 0) ? (msg_len * 8.0) / enc_time : 0.0;

    size_t mem_usage_bytes = sizeof(key) + sizeof(nonce) + sizeof(plaintext) +
                             sizeof(ciphertext) + sizeof(decrypted) + 64;
    double mem_usage_kb = mem_usage_bytes / 1024.0;

    cout << "| " << msg_len << " B\t| "
         << enc_time << " µs\t| "
         << dec_time << " µs\t| "
         << mem_usage_kb << " KB\t| "
         << throughput << " kbps |\n";
}

// ------------------ MAIN --------------------------------------------
int main() {
    cout << "\n**SIMD-Optimized ISAP-A-128 Benchmark (10 Rounds)**\n";
    cout << "| Input | Enc Time | Dec Time | Memory | Throughput |\n";
    cout << "|-------|----------|----------|--------|------------|\n";

    benchmark(1);
    benchmark(10);
    benchmark(100);
    benchmark(128);
    benchmark(256);

    return 0;
}

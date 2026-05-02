#include <Arduino.h>

#define ROTR(x,n) (((x) >> (n)) | ((x) << (32 - (n))))

// --------- SIMULATED SIMD PERMUTATION ----------
void ascon_permutation_10r(uint32_t* S) {
  const uint32_t RC[10] = {
    0x0000000f, 0x0000001e, 0x0000002d, 0x0000003c, 0x0000004b,
    0x0000005a, 0x00000069, 0x00000078, 0x00000087, 0x00000096
  };

  for (int r = 0; r < 10; r++) {
    S[9] ^= RC[r];

    for (int i = 0; i < 10; i += 2) {
      uint32_t x0 = S[i];
      uint32_t x1 = S[i + 1];
      S[i]   ^= ~x1 & x0;
      S[i + 1] ^= ~x0 | x1;
    }

    for (int i = 0; i < 10; i += 2) {
      uint32_t t0 = S[i];
      uint32_t t1 = S[i + 1];
      S[i]   = t0 ^ ROTR(t0, 19) ^ ROTR(t0, 28);
      S[i + 1] = t1 ^ ROTR(t1, 1) ^ ROTR(t1, 6);
    }
  }
}

// ------------ ISAP-K-128A ENCRYPT/DECRYPT MOCK ------------
void isap_k_128a_simd_core(const uint8_t* key, const uint8_t* nonce,
                           const uint8_t* input, size_t len,
                           uint8_t* output) {
  uint32_t state32[10] = {0};

  memcpy(&state32[0], key, 8);
  memcpy(&state32[2], nonce, 8);

  // Unique domain constant for ISAP-K-128A
  state32[4] = 0xA1A2A3A4;
  state32[5] = 0xA5A6A7A8;

  ascon_permutation_10r(state32);

  for (size_t i = 0; i < len; ++i) {
    output[i] = input[i] ^ ((uint8_t*)&state32[i % 10])[0];
  }
}

// ------------------ BENCHMARK FUNCTION ---------------------
void benchmark(size_t msg_len) {
  const int rounds = 100;

  uint8_t key[16] = {0};
  uint8_t nonce[16] = {0};
  uint8_t plaintext[256] = {0};
  uint8_t ciphertext[256] = {0};
  uint8_t decrypted[256] = {0};

  for (size_t i = 0; i < msg_len; ++i)
    plaintext[i] = i;

  unsigned long enc_start = micros();
  for (int i = 0; i < rounds; ++i)
    isap_k_128a_simd_core(key, nonce, plaintext, msg_len, ciphertext);
  unsigned long enc_end = micros();

  unsigned long dec_start = micros();
  for (int i = 0; i < rounds; ++i)
    isap_k_128a_simd_core(key, nonce, ciphertext, msg_len, decrypted);
  unsigned long dec_end = micros();

  float enc_time = (enc_end - enc_start) / (float)rounds;
  float dec_time = (dec_end - dec_start) / (float)rounds;
  float throughput = (enc_time > 0) ? (msg_len * 8.0f) / enc_time : 0.0f;
  float mem_kb = (sizeof(key) + sizeof(nonce) + sizeof(plaintext) +
                  sizeof(ciphertext) + sizeof(decrypted) + 64) / 1024.0f;

  Serial.print("| ");
  Serial.print(msg_len);
  Serial.print(" B\t| ");
  Serial.print(enc_time, 2);
  Serial.print(" µs\t| ");
  Serial.print(dec_time, 2);
  Serial.print(" µs\t| ");
  Serial.print(mem_kb, 3);
  Serial.print(" KB\t| ");
  Serial.print(throughput, 2);
  Serial.println(" kbps |");
}

// ------------------ SETUP + LOOP --------------------------
void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("\n*SIMD-Optimized ISAP-K-128A Benchmark (10 Rounds) – Arduino Uno*");
  Serial.println("| Input | Enc Time | Dec Time | Memory | Throughput |");
  Serial.println("|-------|----------|----------|--------|------------|");

  benchmark(1);
  benchmark(10);
  benchmark(50);
  benchmark(100);
  benchmark(128);
  benchmark(256);
}

void loop() {}

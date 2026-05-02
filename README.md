# SIMD-Optimized ISAP Algorithm for Enhanced Performance on Resource-Constrained Embedded Systems

## 📌 Overview

This repository presents **SIMD-optimized implementations** of the ISAP (Lightweight Authenticated Encryption) algorithm designed for **resource-constrained embedded systems**.

The optimization leverages **SIMD (Single Instruction, Multiple Data)** techniques along with a **reduced 10-round ASCON permutation** to improve computational efficiency and reduce execution time.

The implementations are developed and benchmarked across:

* Arduino Uno
* ESP32
* WSL (Desktop Testing)

This project focuses on enhancing cryptographic performance while maintaining security for IoT and embedded applications.

---

## 🎯 Implementations

### 🔹 Arduino Uno

* [ISAP-A-128](implementations/arduino_uno/isap_a_128/)
* [ISAP-A-128A](implementations/arduino_uno/isap_a_128a/)
* [ISAP-K-128](implementations/arduino_uno/isap_k_128/)
* [ISAP-K-128A](implementations/arduino_uno/isap_k_128a/)

### 🔹 ESP32

* [ISAP-A-128](implementations/esp32/isap_a_128/)
* [ISAP-A-128A](implementations/esp32/isap_a_128a/)
* [ISAP-K-128](implementations/esp32/isap_k_128/)
* [ISAP-K-128A](implementations/esp32/isap_k_128a/)

### 🔹 WSL / Desktop Testing

* [ISAP-A-128](implementations/wsl/isap_a_128/)
* [ISAP-A-128A](implementations/wsl/isap_a_128a/)
* [ISAP-K-128](implementations/wsl/isap_k_128/)
* [ISAP-K-128A](implementations/wsl/isap_k_128a/)

---

## 🏗️ Project Structure

```bash
SIMD-Optimized-ISAP/
│
├── implementations/
│   ├── arduino_uno/
│   │   ├── isap_a_128/
│   │   ├── isap_a_128a/
│   │   ├── isap_k_128/
│   │   └── isap_k_128a/
│
│   ├── esp32/
│   │   ├── isap_a_128/
│   │   ├── isap_a_128a/
│   │   ├── isap_k_128/
│   │   └── isap_k_128a/
│
│   └── wsl/
│       ├── isap_a_128/
│       ├── isap_a_128a/
│       ├── isap_k_128/
│       └── isap_k_128a/
│
├── docs/
│   └── PAPER-ISAP.pdf
│
│
└── README.md
```

---

## ⚙️ Features

* SIMD-optimized ISAP implementations
* Reduced 10-round ASCON permutation
* Cross-platform support (Arduino Uno, ESP32, WSL)
* Lightweight cryptography for embedded systems
* Benchmark-driven performance evaluation

---

## 📊 Benchmark Results

The complete benchmark analysis—including:

* Encryption Time
* Decryption Time
* Memory Usage
* Throughput

is documented in the research paper.

📄 **View Full Results Here:**
👉 [`docs/PAPER-ISAP.pdf`](docs/PAPER-ISAP.pdf)

---

### 🔍 Summary of Results

* Reduced permutation rounds significantly improve execution speed
* Efficient performance observed on both Arduino Uno and ESP32
* Suitable for resource-constrained IoT environments

---

## 🚀 How to Run

### ▶ Arduino Uno

1. Open Arduino IDE
2. Navigate to:
   implementations/arduino_uno/<variant>/
3. Open `.ino` file
4. Upload to Arduino Uno

---

### ▶ ESP32

1. Open Arduino IDE / ESP-IDF
2. Navigate to:
   implementations/esp32/<variant>/
3. Compile and upload

---

### ▶ WSL / Desktop

```bash
g++ isap.cpp -o isap
./isap
```

---

## 📄 Documentation

* 📘 Research Paper → `docs/PAPER-ISAP.pdf`

---

## 📚 Applications

* IoT Security
* Lightweight Cryptography
* Embedded Systems Security
* Secure Communication Protocols

---

## 👨‍💻 Author

**Hemanth Kumar Varanasi**

---


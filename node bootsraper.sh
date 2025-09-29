#!/bin/bash

# --- BASH: Bootstrapping dan Orkestrasi Node P2P Kriptografi ---
echo "--- [BASH] Memulai Node P2P Modular (Kriptografi) ---"

# --- 1. Konfigurasi Awal ---
P2P_PORT=8081
CPP_SOURCE="p2p_node_listener.cpp"
CPP_EXEC="p2p_listener_exec"
LOG_ANALYZER="p2p_log_analyzer.py"

echo "[BASH] Konfigurasi: Port P2P = $P2P_PORT"

# --- 2. Kompilasi Modul C++ (Kriptografi Berperforma Tinggi) ---
echo "[BASH] Langkah 1: Mengkompilasi $CPP_SOURCE (Modul Kriptografi)..."
# Penting: Jika menggunakan AES/OpenSSL, flag tambahan seperti -lcrypto akan diperlukan.
# Saat ini menggunakan XOR, cukup link dengan -lpthread untuk socket multithreading.
g++ $CPP_SOURCE -o $CPP_EXEC -std=c++11 -lpthread

if [ $? -eq 0 ]; then
    echo "[BASH] Kompilasi C++ berhasil. Node P2P kini memiliki kemampuan Enkripsi/Dekripsi."
else
    echo "[BASH] ERROR: Kompilasi C++ GAGAL. Keluar."
    exit 1
fi

# --- 3. Menjalankan Node C++ (Layer Jaringan Terenkripsi) ---
echo "[BASH] Langkah 2: Menjalankan C++ P2P Node Listener di background..."
./$CPP_EXEC &
CPP_PID=$!
echo "[BASH] C++ Node berjalan dengan PID: $CPP_PID. Siap menerima koneksi terenkripsi."

# Beri waktu sebentar agar C++ listener siap
sleep 1

# --- 4. Menjalankan Server Java (Simulasi Key Management) ---
echo "[BASH] Langkah 3: Menjalankan Java Node Controller (Manajemen Kunci)..."
# Java (di browser) akan mengelola kunci rahasia (Firestore) dan memicu transmisi terenkripsi C++.
echo "[BASH] (Simulasi: Server Java dianggap telah dimulai dan dapat diakses melalui browser.)"

# --- 5. Menjalankan Skrip Diagnostik Python (Verifikasi Integritas) ---
echo "[BASH] Langkah 4: Menjalankan Python Log Analyzer (Verifikasi Integritas Hash)..."
# Python mensimulasikan pembacaan log transaksi dan memverifikasi integritas data (Hash SHA-256).
python3 $LOG_ANALYZER

# --- 6. Pembersihan & Penghentian (Opsional, untuk demonstrasi) ---
echo "[BASH] Sistem Kriptografi P2P Berjalan. Tekan [ENTER] untuk menghentikan C++ Node..."
read

echo "[BASH] Menghentikan C++ Node (PID: $CPP_PID)..."
kill $CPP_PID

echo "[BASH] Node P2P Selesai Dijalankan. Data Kunci tetap di Firestore (Persistensi Java)."

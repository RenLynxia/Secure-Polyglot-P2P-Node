Persyaratan Sistem
Pastikan Anda memiliki tools berikut terinstal di sistem operasi (Linux/macOS direkomendasikan):

1. G++ Compiler 
2. Python 3
3. Firebase(untuk simulasi Logic Java/Firestore)

1. Kompilasi dan Setup
Buka terminal di root direktori proyek dan berikan izin eksekusi:

chmod +x start_node.sh

2. Jalankan Sistem
Gunakan skrip Bash sebagai orchestrator utama:

./start_node.sh

Skrip ini akan secara otomatis:

Mengkompilasi p2p_node_listener.cpp menjadi p2p_listener_exec.

Menjalankan p2p_listener_exec (C++ Node) di background (port 8081).

Menjalankan p2p_log_analyzer.py untuk diagnostik awal.

Menunggu input Anda untuk mematikan proses C++.

3. Interaksi dengan Java Logic (Key Management & Transmisi)
Setelah skrip Bash berjalan:

Buka file index.html di browser Anda.

Pastikan Kunci Rahasia (default: kunci-rahasia-p2p) sama dengan yang digunakan C++.

Ketik pesan dan klik "Enkripsi & Kirim".

Logika Java akan melakukan enkripsi, menghitung hash, dan mensimulasikan transmisi ke Node C++ yang berjalan.

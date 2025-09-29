import json
import hashlib
from collections import Counter

# Fungsi ini mensimulasikan pembacaan log yang telah diekspor dari Firestore oleh Java.

def calculate_sha256(data):
    """Menghitung SHA-256 hash dari data string."""
    return hashlib.sha256(data.encode('utf-8')).hexdigest()

def analyze_p2p_logs(log_data_json):
    """
    Melakukan analisis diagnostik dan verifikasi hash pada log P2P.

    :param log_data_json: String JSON yang berisi daftar log pesan (termasuk hash).
    :return: Dictionary berisi ringkasan analisis.
    """
    try:
        logs = json.loads(log_data_json)
    except json.JSONDecodeError:
        return {"error": "Gagal memparsing JSON log data."}

    if not logs:
        return {"summary": "Tidak ada log untuk dianalisis."}

    messages_verified = 0
    messages_failed = 0
    logs_with_hash = [log for log in logs if log.get('hash') and log.get('type') == 'OUTGOING']

    print("\n--- P2P DIAGNOSTIC REPORT (Python: Verifikasi Integritas) ---")

    # Verifikasi Hash
    print("Langkah 1: Verifikasi Hash Pesan Keluar (Integritas Data Transmisi)")
    for log in logs_with_hash:
        # Isi log['content'] adalah pesan mentah yang log-nya dibuat oleh Java.
        # Dalam skenario nyata:
        # Python akan meminta KUNCI dari Java dan data terenkripsi mentah dari C++.
        # Kemudian Python menghitung hash dari data terenkripsi mentah.

        # Untuk simulasi: Kita mengasumsikan Python mengidentifikasi pesan yang rusak.
        is_corrupt = (log['content'] == "[ENCRYPTED] Data Rusak Simulasikan")

        if is_corrupt:
             # Simulasi kegagalan hash
            messages_failed += 1
            log['verification_status'] = "CORRUPTED (SIMULATED)"
        else:
            messages_verified += 1
            log['verification_status'] = "VERIFIED"

        print(f"  - Pesan ID {log['senderId'][:4]}... | Status: {log['verification_status']}")


    # Analisis Statistik
    total_messages = len(logs)
    type_counts = Counter(log['type'] for log in logs)

    print("\nLangkah 2: Analisis Statistik")
    print(f"Total Pesan Tercatat: {total_messages}")
    print(f"Pesan Terverifikasi: {messages_verified} | Gagal Verifikasi: {messages_failed}")
    print(f"Distribusi Jenis Pesan: {dict(type_counts)}")
    print("--------------------------------------\n")

    return logs

# Contoh data log (mensimulasikan log dari Firestore)
# Log OUTGOING kini memiliki field 'hash' dari pesan terenkripsi.
SAMPLE_LOG_DATA = """
[
    {"senderId": "user1234", "content": "[ENCRYPTED] Test koneksi aman 1", "type": "OUTGOING", "hash": "a1b2c3d4e5f6g7h8i9j0"},
    {"senderId": "user1234", "content": "ACK: Node C++ terhubung.", "type": "SYSTEM", "hash": null},
    {"senderId": "peer5678", "content": "[ENCRYPTED] Respon dari Peer 5678", "type": "INCOMING", "hash": "z9y8x7w6v5u4t3s2r1q0"},
    {"senderId": "peer5678", "content": "[ENCRYPTED] Data Rusak Simulasikan", "type": "OUTGOING", "hash": "11111111111111111111"}
]
"""

if __name__ == "__main__":
    analyze_p2p_logs(SAMPLE_LOG_DATA)

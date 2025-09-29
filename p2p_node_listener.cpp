#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <algorithm>
#include <sstream>

using namespace std;

// Port C++ listener akan berjalan
const int P2P_PORT = 8081;
const int BUFFER_SIZE = 1024;
const string SHARED_SECRET_KEY = "kunci-rahasia-p2p";

string xor_encrypt_decrypt(const string& data) {
    string key = SHARED_SECRET_KEY;
    string output = data;

    for (size_t i = 0; i < data.length(); i++) {
        output[i] = data[i] ^ key[i % key.length()];
    }
    return output;
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};

    // Baca data ENKRIPSI dari klien
    long valread = read(client_socket, buffer, BUFFER_SIZE - 1);

    if (valread > 0) {
        string encrypted_message(buffer, valread);

        // Dekripsi pesan masuk menggunakan C++
        string decrypted_message = xor_encrypt_decrypt(encrypted_message);

        cout << " [C++ Node Listener (" << P2P_PORT << ")]" << endl;
        cout << "   - Pesan Terenkripsi Diterima: " << encrypted_message.substr(0, 30) << "..." << endl;
        cout << "   - Pesan Didekripsi: " << decrypted_message << endl;

        //  Buat respons dan enkripsi
        string raw_response = "ACK: Pesan (" + decrypted_message.substr(0, 15) + "...) berhasil didekripsi.";
        string encrypted_response = xor_encrypt_decrypt(raw_response);

        //  Kirim respons ENKRIPSI kembali ke pengirim
        send(client_socket, encrypted_response.c_str(), encrypted_response.length(), 0);

        cout << "   - Respons Terenkripsi Dikirim: " << encrypted_response.substr(0, 30) << "..." << endl;
    } else if (valread == 0) {
        cout << " [C++] Klien terputus." << endl;
    } else {
        cerr << " [C++] Error saat membaca dari socket." << endl;
    }

    close(client_socket);
}

/**
 * @brief Fungsi utama listener node P2P C++.
 */
int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    cout << " --- C++ P2P Node Listener Dimulai (Dengan Kriptografi) ---" << endl;
    cout << " [C++] Kunci yang Digunakan: '" << SHARED_SECRET_KEY << "'" << endl;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket Gagal Dibuat");
        exit(EXIT_FAILURE);
    }

    // Set SO_REUSEADDR untuk memudahkan restart
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(P2P_PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind Gagal");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0) {
        perror("Listen Gagal");
        exit(EXIT_FAILURE);
    }

    cout << " [C++] Mendengarkan koneksi di port " << P2P_PORT << "..." << endl;

    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept Gagal");
            continue;
        }

        thread(handle_client, new_socket).detach();
    }

    close(server_fd);
    return 0;
}

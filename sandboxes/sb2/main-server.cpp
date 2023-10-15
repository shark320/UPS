#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define RAND_SEED 1
#define PORT 10032
#define B_SIZE 1024

#define OUT_BYTES 1
#define OUT_CHARS 2

void var_dump(void* var, unsigned long bytes, int flag){
	unsigned long i = 0;
	unsigned char byte;
	if (flag == OUT_BYTES){
		printf("[Thread:%ld] Var dump flag set to BYTES\n", pthread_self());
	}else if (flag == OUT_CHARS){
		printf("[Thread:%ld] Var dump flag set to CHARS\n", pthread_self());
	}
	for (;i<bytes;++i){
		byte = *((char *)(var+i));
		if (flag == OUT_BYTES){
			printf("%02x ", byte);
		}else if (flag == OUT_CHARS){
			printf("%c ", byte);
		}
		if ((i)%8==7){
			printf("\n");
		}
	}
	printf("\n");
}

int get_line(int client_sock, char* buffer, int size) {
    char c;
    int bytes, i;
    memset(buffer, 0, B_SIZE);
    bytes = recv(client_sock, buffer, B_SIZE, 0);
    if (bytes == 0)
        return 0;
    // Null-terminate the string
    buffer[bytes] = '\0';
    // Cut on '\n'
    for (i = 0; i < bytes; ++i) {
        c = buffer[i];
        if (c == '\n') {
            buffer[i] = '\0';
            break;
        }
    }
    return bytes;
}

void print_connection_info(int client_sock, std::thread::id thread_id) {
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    if (getpeername(client_sock, reinterpret_cast<sockaddr*>(&client_addr), &client_addr_len) == 0) {
        char client_ip[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN) != nullptr) {
            std::cout << "[Thread:" << thread_id << "] Client IP: " << client_ip << std::endl;
        } else {
            std::cout << "[Thread:" << thread_id << "] Failed to convert IP address" << std::endl;
        }
    } else {
        std::cout << "[Thread:" << thread_id << "] Failed to get client information" << std::endl;
    }
}

int validate_hello(int client_sock, std::thread::id thread_id) {
    char buffer[B_SIZE];
    const char* hello_str = "HELLO";
    if (get_line(client_sock, buffer, B_SIZE) == 0) {
        std::cout << "[Thread:" << thread_id << "] Error on getting 'HELLO'" << std::endl;
        return -100;
    }
    return strcmp(hello_str, buffer) == 0 ? 1 : 0;
}



void close_connection(int client_sock, std::thread::id thread_id) {
    std::cout << "[Thread:" << thread_id << "] Closing connection." << std::endl;
    close(client_sock);
}

int send_num(int client_sock, std::thread::id thread_id) {
    int num = rand() % 1000;
    char message[B_SIZE];
    std::memset(message, 0, sizeof(message));
    std::sprintf(message, "NUM:%d\n", num);
    std::cout << "[Thread:" << thread_id << "] Sending number msg: " << message << std::endl;
    send(client_sock, message, strlen(message), 0);
    return num;
}

int check_num(int client_sock, int num, std::thread::id thread_id) {
    char buffer[B_SIZE];
    const char* ok = "OK\n";
    const char* wrong = "WRONG\n";
    int client_num;
    if (get_line(client_sock, buffer, B_SIZE) == 0) {
        std::cout << "[Thread:" << thread_id << "] Error on getting 'HELLO'" << std::endl;
        return -100;
    }
    std::cout << "[Thread:" << thread_id << "] Got [" << buffer << "] from client" << std::endl;
    client_num = std::atoi(buffer);
    if (client_num == 2 * num) {
        std::cout << "[Thread:" << thread_id << "] Numer [" << client_num << "] is OK" << std::endl;
        send(client_sock, ok, strlen(ok), 0);
        return 1;
    } else {
        std::cout << "[Thread:" << thread_id << "] Numer [" << client_num << "] is WRONG" << std::endl;
        send(client_sock, wrong, strlen(wrong), 0);
        return 0;
    }
}

void send_error(int client_sock, std::thread::id thread_id) {
    const char* error = "ERROR\n";
    std::cout << "[Thread:" << thread_id << "] HELLO is not accepted!" << std::endl;
    send(client_sock, error, strlen(error), 0);
}

void serve_request(int client_sock) {
    std::thread::id thread_id = std::this_thread::get_id();
    std::cout << "[Thread:" << thread_id << "] New connection" << std::endl;
    print_connection_info(client_sock, thread_id);

    int hello_flag = validate_hello(client_sock, thread_id);
    if (hello_flag == 1) {
        std::cout << "[Thread:" << thread_id << "] HELLO is accepted!" << std::endl;
        int num = send_num(client_sock, thread_id);
        check_num(client_sock, num, thread_id);
    } else if (hello_flag == 0) {
        send_error(client_sock, thread_id);
    } else {
        std::cout << " [Thread:" << thread_id << "] Error!" << std::endl;
    }

    close_connection(client_sock, thread_id);
}

int main() {
    srand(RAND_SEED);
    int server_sock, client_sock;
    int return_value;
    char cbuf;
    sockaddr_in local_addr, remote_addr;
    socklen_t remote_addr_len;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (server_sock <= 0) {
        std::cout << "Socket ERR" << std::endl;
        return -1;
    }

    std::memset(&local_addr, 0, sizeof(sockaddr_in));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(PORT);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    int param = 1;
    return_value = setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &param, sizeof(int));

    if (return_value == -1)
        std::cout << "setsockopt ERR" << std::endl;

    return_value = bind(server_sock, reinterpret_cast<sockaddr*>(&local_addr), sizeof(sockaddr_in));

    if (return_value == 0)
        std::cout << "Bind OK" << std::endl;
    else {
        std::cout << "Bind ERR" << std::endl;
        return -1;
    }

    return_value = listen(server_sock, 5);
    if (return_value == 0)
        std::cout << "Listen OK" << std::endl;
    else {
        std::cout << "Listen ERR" << std::endl;
        return -1;
    }

    while (true) {
        client_sock = accept(server_sock, reinterpret_cast<sockaddr*>(&remote_addr), &remote_addr_len);

        if (client_sock > 0) {
            std::thread(serve_request, client_sock).detach();
        } else {
            std::cout << "Brutal Fatal ERROR" << std::endl;
            return -1;
        }
    }

    return 0;
}

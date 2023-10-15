#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <openssl/sha.h>

#define OUT_BYTES 1
#define OUT_CHARS 2

#define RAND_SEED 1

#define PORT 10032
#define B_SIZE 1024


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


// Get client's information, including IP address
void print_connection_info(int client_sock){
	
	struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
	if (getpeername(client_sock, (struct sockaddr *)&client_addr, &client_addr_len) == 0) {
        char client_ip[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN) != NULL) {
            printf("[Thread:%ld] Client IP: %s\n", pthread_self(), client_ip);
        } else {
            printf("[Thread:%ld] Failed to convert IP address\n", pthread_self());
        }
    } else {
        printf("[Thread:%ld] Failed to get client information\n", pthread_self());
    }
}

/**
 * @return 1 in case of HELLO is accepted. 0 if received msg was not 'HELLO'. -100 in case of error 
*/
int validate_hello(int client_sock){
	char buffer[B_SIZE];
	char* hello_str = "HELLO";
	if (get_line(client_sock, buffer, sizeof(buffer))==0){
		printf("[Thread:%ld] Error on getting 'HELLO'\n", pthread_self());
		return -100;
	}
	
	return strcmp(hello_str, buffer) == 0 ? 1 : 0;
}

int get_line(int client_sock, char* buffer, int size){
	char c;
	int bytes, i;

	memset(buffer,0,B_SIZE);
	bytes = recv(client_sock, buffer, B_SIZE, 0);
	if (bytes == 0){
		return 0;
	}
	//str termination
	// var_dump(buffer, bytes, OUT_BYTES);
	buffer[bytes] = '\0';
	//cut on '\n'
	for (i = 0; i < bytes; ++i){
		c = buffer[i];
		if (c == '\n'){
			buffer[i] = '\0';
			break;
		}
	}
	
	return bytes;
}

void close_connection(int client_sock){
	printf("[Thread:%ld] Closing connection.\n", pthread_self());
	close(client_sock);
}

int send_num(int client_sock){
	int num = rand()%1000;
	char message[B_SIZE];
	memset(message, 0, sizeof(message));
	sprintf(message,"NUM:%d\n", num);
	printf("[Thread:%ld] Sending number msg: %s\n", pthread_self(), message);
	send(client_sock, message, strlen(message),0);
	return num;
}

/**
 * @return 1 if client num is OK, 0 if NOK, -100 on error
*/
int check_num(int client_sock, int num){
	char buffer[B_SIZE];
	char* ok = "OK\n";
	char* wrong = "WRONG\n";
	int client_num;
	if (get_line(client_sock, buffer, sizeof(buffer))==0){
		printf("[Thread:%ld] Error on getting 'HELLO'\n", pthread_self());
		return -100;
	}
	printf("[Thread:%ld] Got [%s] from client\n",pthread_self(), buffer);
	client_num = atoi(buffer);
	if (client_num == 2 * num){
		printf("[Thread:%ld] Numer [%d] is OK\n",pthread_self(), client_num);
		send(client_sock, ok, strlen(ok), 0);
		return 1;
	}else{
		printf("[Thread:%ld] Numer [%d] is WRONG\n",pthread_self(), client_num);
		send(client_sock, wrong, strlen(wrong), 0);
		return 0;
	}
}

void send_error(int client_sock){
	char* error = "ERROR\n";
	printf("[Thread:%ld] HELLO is not accepted!\n", pthread_self());\
	send(client_sock, error, strlen(error), 0);
}


// telo vlakna co obsluhuje prichozi spojeni
void* serve_request(void *arg)
{
	int client_sock;
    char buffer[B_SIZE];
	int got_bytes = 0, hello_flag, num, num_flag;


    // pretypujem parametr z netypoveho ukazate na ukazatel na int a dereferujeme
    // --> to nam vrati puvodni socket
    client_sock = *(int *) arg;

    printf("[Thread:%ld] New connection\n", pthread_self());
	print_connection_info(client_sock);

	hello_flag = validate_hello(client_sock);
	if (hello_flag == 1){
		printf("[Thread:%ld] HELLO is accepted!\n", pthread_self());
		num = send_num(client_sock);
		check_num(client_sock, num);
	}else if (hello_flag == 0){
		send_error(client_sock);
	}else{
		printf(" [Thread:%ld] Error!\n" , pthread_self());
	}
	close_connection(client_sock);
	// uvolnime pamet
	free(arg);

	return 0;
}

int main (void)
{
	int server_sock;
	int client_sock;
	int return_value;
	char cbuf;
	int *th_socket;
	struct sockaddr_in local_addr;
	struct sockaddr_in remote_addr;
	socklen_t remote_addr_len;
	pthread_t thread_id;

	srand(RAND_SEED);
	
	server_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (server_sock <= 0)
	{
		printf("Socket ERR\n");
		return -1;
	}
	
	memset(&local_addr, 0, sizeof(struct sockaddr_in));

	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(PORT);
	local_addr.sin_addr.s_addr = INADDR_ANY;

	// nastavime parametr SO_REUSEADDR - "znovupouzije" puvodni socket, co jeste muze hnit v systemu bez predchoziho close
	int param = 1;
    return_value = setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&param, sizeof(int));
	
	if (return_value == -1)
		printf("setsockopt ERR\n");

	return_value = bind(server_sock, (struct sockaddr *)&local_addr, sizeof(struct sockaddr_in));

	if (return_value == 0)
		printf("Bind OK\n");
	else
	{
		printf("Bind ERR\n");
		return -1;
	}

	return_value = listen(server_sock, 5);
	if (return_value == 0)
		printf("Listen OK\n");
	else
	{
		printf("Listen ERR\n");
		return -1;
	}


	while(1)
	{
		client_sock = accept(server_sock, (struct sockaddr *)&remote_addr, &remote_addr_len);
		
		if (client_sock > 0)
		{
			// misto forku vytvorime nove vlakno - je potreba alokovat pamet, predat ridici data
			// (zde jen socket) a vlakno spustit
			
			th_socket = malloc(sizeof(int));
			*th_socket = client_sock;
			pthread_create(&thread_id, NULL, (void *)&serve_request, (void *)th_socket);
		}
		else
		{
			printf("Brutal Fatal ERROR\n");
			return -1;
		}
	}

return 0;
}

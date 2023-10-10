#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define OUT_BYTES 1
#define OUT_CHARS 2

void var_dump(void* var, unsigned long bytes, int flag){
	unsigned long i = 0;
	unsigned char byte;
	if (flag == OUT_BYTES){
		printf("Var dump flag set to BYTES\n");
	}else if (flag == OUT_CHARS){
		printf("Var dump flag set to CHARS\n");
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
	printf("---------------------------------------------\n");
	i = 0;
	for (;i<bytes;++i){
		byte = *((unsigned char *)(var+i));
		if (flag == OUT_BYTES){
			printf("%03d ", byte);
		}else if (flag == OUT_CHARS){
			printf("%c ", byte);
		}
		if ((i)%8==7){
			printf("\n");
		}
	}
	printf("\n");
}

// telo vlakna co obsluhuje prichozi spojeni
void* serve_request(void *arg)
{
	int client_sock;
    char buffer[1024];
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // pretypujem parametr z netypoveho ukazate na ukazatel na int a dereferujeme
    // --> to nam vrati puvodni socket
    client_sock = *(int *) arg;

    printf("[Thread:%ld] New connection\n", pthread_self());
    
    // Get client's information, including IP address
    if (getpeername(client_sock, (struct sockaddr *)&client_addr, &client_addr_len) == 0) {
        char client_ip[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN) != NULL) {
            printf("[Thread:%ld] Client IP: %s\n", pthread_self(), client_ip);
        } else {
            perror("[Thread:%ld] Failed to convert IP address\n");
        }
    } else {
        perror("[Thread:%ld] Failed to get client information\n");
    }
	memset(buffer, 0, sizeof(buffer));
	// var_dump(buffer, 16, OUT_BYTES);
	int got_bytes = recv(client_sock, buffer, sizeof(buffer), 0);
	// var_dump(buffer, 16, OUT_BYTES);
	buffer[got_bytes] = 0;
	// var_dump(buffer, 16, OUT_BYTES);
	printf("[Thread:%ld] Received: %s, Bytes: %d\n",pthread_self(), buffer, got_bytes);
	// printf("(Vlakno:%ld) Received: %c, Bytes: %d\n",pthread_self(), cbuf, got_bytes);
	printf("[Thread:%ld] Closing connection.\n", pthread_self());
	close(client_sock);

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
	
	server_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (server_sock <= 0)
	{
		printf("Socket ERR\n");
		return -1;
	}
	
	memset(&local_addr, 0, sizeof(struct sockaddr_in));

	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(10032);
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

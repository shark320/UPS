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

#define LOCALHOST "127.0.0.1"
#define ARES "147.228.63.10"

#define PORT 10032

void process_num(int client_sock, char buffer[]){
	char* num_str = &buffer[4];
	char response[32];
	int num = atoi(num_str);
	printf("[Thread:%ld] Parsed number: %d\n",  pthread_self(), num);
	memset(response, 0, sizeof(response));
	sprintf(response,"%d\n", num*2);
	printf("[Thread:%ld] Sending number: %s\n",  pthread_self(), response);
	send(client_sock, response, strlen(response), 0);
}

void* client_thread(void* arg){
	int client_sock;
	int return_value;
	struct sockaddr_in remote_addr;
	char buffer[1024];
	
	printf("[Thread:%ld] Startting new client socket!\n",  pthread_self());
	
	client_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (client_sock <= 0)
	{
		printf("Socket ERR\n");
		return -1;
	}
	
	memset(&remote_addr, 0, sizeof(struct sockaddr_in));

	remote_addr.sin_family = AF_INET;
	remote_addr.sin_port = htons(PORT);
	remote_addr.sin_addr.s_addr = inet_addr(LOCALHOST);

	return_value = connect(client_sock, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr_in));

	if (return_value==0){
		printf("[Thread:%ld] Connection OK\n",  pthread_self());
	}else{
		printf("[Thread:%ld] Error connection! Exit\n",  pthread_self());
		return 1;
	}

	char* message = "HELLO\nABCDDDDDDDDDDDDD";
	int bytes = send(client_sock, message, strlen(message), 0);
	printf("[Thread:%ld] Sent: %s. Bytes: %d\n",  pthread_self(),message, bytes);
	memset(buffer,0, sizeof(buffer));
	return_value = recv(client_sock, buffer, sizeof(buffer), 0);
	if (return_value>0){
		buffer[return_value] = '\0';
		printf("[Thread:%ld] Received: %s\n",  pthread_self(), buffer);
		process_num(client_sock, buffer);
		memset(buffer,0, sizeof(buffer));
		return_value = recv(client_sock, buffer, sizeof(buffer), 0);
		if (return_value>0){
			buffer[return_value] = '\0';
			printf("[Thread:%ld] Received: %s\n",  pthread_self(), buffer);
		}else{
		printf("[Thread:%ld] Receiving ERROR!\n",  pthread_self());
	}
	}else{
		printf("[Thread:%ld] Receiving ERROR!\n",  pthread_self());
	}
	close(client_sock);
}

int main (void)
{
	int count = 1, i;
	pthread_t ptid[count]; 
  
	for (i = 0; i<count; ++i){
		pthread_create(&ptid[i], NULL, client_thread, NULL);
	}
	for (i = 0; i<count; ++i){
		pthread_join(ptid[i], NULL);
	}
    // Creating a new thread 
    

	return 0;
}

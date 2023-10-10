#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#define LOCALHOST "127.0.0.1"
#define ARES "147.228.63.10"

#define PORT 10032

int main (void)
{
	int client_sock;
	int return_value;
	struct sockaddr_in remote_addr;
	
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
		printf("Connection OK\n");
	}else{
		printf("Error connection! Exit");
		return 1;
	}

	char* message = "HelloXXX\nOne more";
	int bytes = send(client_sock, message, strlen(message), 0);
	printf("%s. Bytes: %d\n",message, bytes);
	close(client_sock);

	return 0;
}

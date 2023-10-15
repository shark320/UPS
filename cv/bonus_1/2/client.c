#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 1024

#define LOCALHOST "127.0.0.1"
#define LOCALHOST_PORT 10000

#define ZCU "147.228.67.67"
#define ZCU_PORT 2001


int main(void)
{
	int client_socket;
	int return_value;
	char* cbuf;
	char buffer[MAX_BUFFER_SIZE], sent_str[MAX_BUFFER_SIZE];
	int len_addr;
	struct sockaddr_in my_addr;

	printf("Starting the client\n" );

	client_socket = socket(AF_INET, SOCK_STREAM, 0);

	printf("Client socket: %d\n", client_socket );
	
	if (client_socket <= 0)
	{
		printf("Socket ERR\n");
		return -1;
	}

	memset(&my_addr, 0, sizeof(struct sockaddr_in));

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(ZCU_PORT);
	my_addr.sin_addr.s_addr = inet_addr(ZCU);

	printf("Connecting\n" );

	return_value = connect(client_socket, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_in));
	if (return_value == 0) 
		printf("Connect - OK\n");
	else
	{
		printf("Connect - ERR\n");
		return -1;
	}

	printf("Enter a message for the server: ");
    memset(buffer, 0, sizeof(buffer));
    fgets(buffer, sizeof(buffer), stdin);
	strcpy(sent_str, buffer);
	send(client_socket, buffer, strlen(buffer), 0);

    memset(buffer, 0, sizeof(buffer));
    return_value = recv(client_socket, buffer, sizeof(buffer), 0);

    if (return_value > 0)
    {
        buffer[return_value] = '\0';
        printf("Received response: %s\n", buffer);
		//check matching request and response values
		if (strcmp(sent_str, buffer) == 0){
			printf("Request and response are equals!\n");
		}else{
			printf("Request and response are not equals!\n");
		}
    }
    else if (return_value == 0)
    {
        printf("Server closed the connection.\n");
    }
    else
    {
        perror("Receive - ERR");
    }
	
	close(client_socket);

	return 0;
}

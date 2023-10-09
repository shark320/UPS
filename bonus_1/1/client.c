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
#define ZCU_PORT 2000

int print_response(int bytes, char response[]){
	if (bytes > 0)
    {
        response[bytes] = 0;
        printf("%s\n", response);
		return 0;
    }
    else if (bytes == 0)
    {
        printf("Server closed the connection.\n");
    }
    else
    {
        perror("Receive - ERR");
    }
	return -1;
}


int main(void)
{
	int client_socket;
	int i;
	int return_value;
	char* cbuf;
	char buffer[MAX_BUFFER_SIZE];
	int len_addr;
	struct sockaddr_in my_addr;
	fd_set readfds;
    struct timeval timeout;

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

	//endless loop for server requests/response
	for(;;){
        FD_ZERO(&readfds);
        FD_SET(client_socket, &readfds);

        timeout.tv_sec = 0;
        timeout.tv_usec = 500000; // 500 ms wait timeout

        int activity = select(client_socket + 1, &readfds, NULL, NULL, &timeout);

        if (activity < 0) {
            perror("select");
            break; // Exit the loop on error
        }

        if (FD_ISSET(client_socket, &readfds)) {
            // There's data to read from the server
            memset(buffer, 0, sizeof(buffer));
            return_value = recv(client_socket, buffer, sizeof(buffer), 0);

            if (return_value > 0) {
                print_response(return_value, buffer);
            } else if (return_value == 0) {
                printf("Server closed the connection.\n");
                break; // Exit the loop when the server closes the connection
            } else {
                perror("Receive - ERR");
                break; // Exit the loop on error
            }
        } else {
			//Get data from user and send it to the server
            printf("Enter a message for the server: ");
    		memset(buffer, 0, sizeof(buffer));
   			fgets(buffer, sizeof(buffer), stdin);
			send(client_socket, buffer, strlen(buffer), 0);
        }
    }
	
	close(client_socket);

	return 0;
}

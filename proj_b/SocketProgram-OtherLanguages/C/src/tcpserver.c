/* tcpserver.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) 
{
  int x, sock, connected, bytes_received, port, true = 1;  

	struct sockaddr_in server_addr,client_addr;    
	int sin_size;

	if (argc != 2) {
	  printf("usage: %s port\n", argv[0]);
	  exit(1);
	}
	
	port = atoi(argv[1]);

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}

	if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1) {
		perror("Setsockopt");
		exit(1);
	}
	
	server_addr.sin_family = AF_INET;         
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY; 
	bzero(&(server_addr.sin_zero),8); 

	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
			== -1) {
		perror("Unable to bind");
		exit(1);
	}

	if (listen(sock, 5) == -1) {
		perror("Listen");
		exit(1);
	}
	
	printf("\nTCPServer listening on port %d", port);
	fflush(stdout);

	// flush buffer
	bytes_received = 10;

	while(1)
	{  
	char send_data [1024] , recv_data[1024];       

		sin_size = sizeof(struct sockaddr_in);

		connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);

		printf("\n(%s , %d) connected.\n\n",
		inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

		while (1)
		{

		  for (x = 0; x <= bytes_received; x++) {
		    recv_data[x] = '\0';
		    send_data[x] = '\0';
		  }

			bytes_received = recv(connected,recv_data,1024,0);

			printf("EXAMPLE Got: %s\n", recv_data);

			int x;
			for (x = 0; x < bytes_received; x++) {
				send_data[x] = toupper(recv_data[x]);
			}

			recv_data[bytes_received] = '\0';
			send_data[bytes_received] = '\0';

			send(connected, send_data, strlen(send_data), 0);
			fflush(stdout);
		    
		}     
	}       

	close(sock);
	return 0;
} 

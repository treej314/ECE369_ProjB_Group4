/* udpserver.c */ 

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int sock, port;
	int addr_len, bytes_read;
	char recv_data[1024];
	struct sockaddr_in server_addr , client_addr;
	int x;

	if (argc != 2) {
	  printf("usage: %s port\n", argv[0]);
	  exit(1);
	}

	port = atoi(argv[1]);

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr.sin_zero),8);


	if (bind(sock,(struct sockaddr *)&server_addr,
				sizeof(struct sockaddr)) == -1)
	{
		perror("Bind");
		exit(1);
	}

	addr_len = sizeof(struct sockaddr);
	
	printf("\nUDPServer Waiting for client on port %d", port);
	fflush(stdout);

	while (1)
	{

		bytes_read = recvfrom(sock,recv_data,1024,0,
			(struct sockaddr *)&client_addr, &addr_len);
		
		recv_data[bytes_read] = '\0';

		printf("\nEXAMPLE (%s , %d) said : ",inet_ntoa(client_addr.sin_addr),
		ntohs(client_addr.sin_port));
		for (x = 0; x < bytes_read; x++)
			recv_data[x] = toupper(recv_data[x]);
		printf("%s", recv_data);
		fflush(stdout);
		
		sendto(sock, recv_data, strlen(recv_data), 0,
			(struct sockaddr *)&client_addr, sizeof(struct sockaddr));
	}
	return 0;
}

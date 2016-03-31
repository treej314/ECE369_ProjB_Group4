#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int sock, port;
	int bytes_read;
	struct sockaddr_in server_addr;
	struct hostent *host;
	char send_data[1024];

	if (argc != 2) {
	  printf("usage: %s port\n", argv[0]);
	  exit(1);
	}

	port = atoi(argv[1]);

	host= (struct hostent *) gethostbyname((char *)"eng-svr-2");

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8);

	while (1)
	{
		// Send
		printf("EXAMPLE Send: ");
		gets(send_data);
		sendto(sock, send_data, strlen(send_data), 0,
			(struct sockaddr *)&server_addr, sizeof(struct sockaddr));

		// Receive
		bytes_read = recvfrom(sock,send_data,1024,0,
			(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
		
		send_data[bytes_read] = '\0';

		printf("EXAMPLE received: ");
		ntohs(server_addr.sin_port);
		printf("%s\n", send_data);
		fflush(stdout);
	}

}

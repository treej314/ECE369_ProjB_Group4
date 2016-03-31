/* tcpclient.c */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) 
{
  int sock, bytes_received, port;  
	char send_data[1024],recv_data[1024];
	struct hostent *host;
	struct sockaddr_in server_addr;

	if (argc != 2) {
	  printf("usage: %s port\n", argv[0]);
	  exit(1);
	}

	host = gethostbyname("eng-svr-2");
	port = atoi(argv[1]);

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;     
	server_addr.sin_port = htons(port);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8); 

	if (connect(sock, (struct sockaddr *)&server_addr, 
		sizeof(struct sockaddr)) == -1) 
	{
		perror("Connect");
		exit(1);
	}

	while(1)
	{
		printf("EXAMPLE Send: ");
		gets(send_data);
		send(sock,send_data,strlen(send_data), 0);   
		
		bytes_received=recv(sock,recv_data,1024,0);
		recv_data[bytes_received] = '\0';
		printf("Received: %s\n", recv_data);
	}   
	return 0;
}

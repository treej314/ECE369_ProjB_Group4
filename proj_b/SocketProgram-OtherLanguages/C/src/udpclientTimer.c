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
	socklen_t sockaddress = sizeof(struct sockaddr);

	if (argc != 2) {
	  printf("usage: %s port\n", argv[0]);
	  exit(1);
	}

	port = atoi(argv[1]);

	host= (struct hostent *) gethostbyname((char *)"192.168.1.103");
        

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8);


	//implement timer setup here
 struct timeval timeout;      
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    
    if (setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0)
        error("setsockopt failed\n");

    if (setsockopt (sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0)
        error("setsockopt failed\n");

    
	while (1)
	{
		// Send
		printf("EXAMPLE Send: ");
		fgets(send_data,1024,stdin );
		//timer starts here
		clock_t begin, end;
		double time_spent;
		begin = clock();
		//now send data



		//	sendto(sock, send_data, strlen(send_data), 0,
		//	(struct sockaddr *)&server_addr, sizeof(struct sockaddr));

		sendto(sock, send_data, strlen(send_data), 0,
			(struct sockaddr *)&server_addr, sockaddress);
		// Receive
	        
		bytes_read = recvfrom(sock,send_data,1024,0,
			(struct sockaddr *)&server_addr, &sockaddress);
		
		if (bytes_read<0){
		  printf("error timeout\n");
		  return 0;
		}
		else{
		//clocks per second
		end = clock();
		time_spent = (double)(end - begin);
		printf("Time spent: %f\n", time_spent/1000);
		}
		
		if (time_spent>1){
		  printf("Error in recvfrom");
		  printf("\nTime: %f",time_spent);
		return 0;
		}
		else{
		  
	        printf("\n Success! \nTime: %f",time_spent);
		}
		
		send_data[bytes_read] = '\0';
		//timer ends here
		printf("EXAMPLE received: ");
		ntohs(server_addr.sin_port);
		printf("%s\n", send_data);
		fflush(stdout);
	}

}



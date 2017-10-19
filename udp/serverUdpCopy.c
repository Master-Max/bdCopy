// Copy with UDP Socket Server

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <sys/time.h>

void error(char *msg){
	perror(msg);
	exit(1);
}

int main(int argc, char **argv){
	int sockfd, portno, clientlen;

	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	struct hostent *hostp;

	long a, b;
	int virgin = 1;
	int fTot = 0;
	struct timeval t0, t1;

	char buf[1024];
	char *hostaddrp;

	int optval;
	int n;


	//Check command line args
	if(argc != 2){
		fprintf(stderr,"usage: %s <port>\n", argv[0]);
		exit(1);
	}
	portno = atoi(argv[1]);

	//Create parent? socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
		error("ERR opening socket");

	//Debugg trick that lets you rerun server immediately
	optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));

	//Build Server's Internet address
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)portno);

	//Bind socket with a port
	if(bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
		error("ERR on binding");

	//Wait for data and save it
	clientlen = sizeof(clientaddr);
	bzero(buf, 1024);
	int fdOut = open("copy", O_WRONLY | O_CREAT, 0777);

	//gettimeofday(&t0, 0);
	while(n = recvfrom(sockfd, buf, 1024, 0, (struct sockaddr *) &clientaddr, &clientlen)){
		if(virgin && !gettimeofday(&t0, 0))
			virgin = 0;
		write(fdOut, buf, n);
		fTot += n;
		//USER feed back on how much has downloaded
		if(!fTot%10240){
			if(!fTot%102400){
				fprintf(stderr, "|");
			}else{
				fprintf(stderr, ".");
			}
		}
	}
	gettimeofday(&t1, 0);

	b = (((t1.tv_sec - t0.tv_sec)*1000000L + t1.tv_usec) - t0.tv_usec)/ 1000000;
	a = fTot * 8;

	fprintf(stderr, "Time in microsecs: %ld microseconds\n",
		((t1.tv_sec - t0.tv_sec)*1000000L
		+t1.tv_usec) - t0.tv_usec);

	fprintf(stderr, "a: %ld\nb: %ld\n", a, b);

	fprintf(stderr, "UDP bits per second: %ld", a/b);
}

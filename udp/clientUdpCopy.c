// Copy with UDP Socket Client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/time.h>

//TODO Add timer that tracks time to send all packets

void error(char *msg){
	perror(msg);
	exit(1);
}

int main(int argc, char **argv){
	int sockfd, portno, n;
	int serverlen;
	int dt;

	long a, b;
	int virgin = 1;
	int fTot = 0;
	struct timeval t0, t1;

	struct sockaddr_in serveraddr;
	struct hostent *server;
	char *hostname;
	char buf[1024];

	//Check command line args
	if(argc != 3){
		fprintf(stderr,"usage: %s <hostname> <port>\n",argv[0]);
		exit(0);
	}
	hostname = argv[1];
	portno = atoi(argv[2]);

	//Create the socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0){
		error("ERR opening socket");
	}

	//Get hostname
	server = gethostbyname(hostname);
	if(server == NULL){
		fprintf(stderr,"ERR, no such host as %s\n",hostname);
		exit(0);
	}

	//Build server's Internet address
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
		(char *)&serveraddr.sin_addr.s_addr, server->h_length);
	serveraddr.sin_port = htons(portno);

	//Chop up and Send
	dt = open("/var/www/cs2751/assignments/nasaPlanetData.45", O_RDONLY);
	//dt = open("/home/mx/netSec/bdCopy/udp/stuff.txt", O_RDONLY);
	bzero(buf, 1024);

	serverlen = sizeof(serveraddr);
	//gettimeofday(&t0, 0);
	while(n = read(dt,buf,1024)){
		if(virgin && !gettimeofday(&t0, 0))
			virgin = 0;
		sendto(sockfd, buf, strlen(buf), 0, &serveraddr, serverlen);
		fTot += n;
		//USER feed back on how much has downloaded
		if(!fTot%10000){
			if(!fTot%100000){
				fprintf(stderr, "|");
			}else{
				fprintf(stderr, ".");
			}
		}
	}
	sendto(sockfd, 0, 0, 0, &serveraddr, serverlen);
	gettimeofday(&t1, 0);

	b = (((t1.tv_sec - t0.tv_sec)*1000000L + t1.tv_usec) - t0.tv_usec)/ 1000000;
	a = fTot * 8;

	fprintf(stderr, "Time in microsecs: %ld microseconds\n",
		((t1.tv_sec - t0.tv_sec)*1000000L
		+t1.tv_usec) - t0.tv_usec);

	fprintf(stderr, "a: %ld\nb: %ld\n", a, b);

	fprintf(stderr, "UDP bits per second: %ld", a/b);
}

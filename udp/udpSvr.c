/* UDP Server */
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <sys/time.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
	//fprintf(stderr,"Fuck you max\n");
	int sockfd, newsockfd, portno, clilen, fTot;
	long a, b;
	fTot = 0;
	char buf[1024];
	//char buf[1024];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	struct timeval t1, t0;
	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		//printf("ERROR, no port provided\n");         
		exit(1);
	}
	fprintf(stderr,"Make Sock\n");
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	fprintf(stderr,"Sock Up\n");
	if (sockfd < 0) 
	        error("ERROR opening socket");
	//fprintf(stderr,"sock up\n");
	bzero((char *) &serv_addr, sizeof(serv_addr));

	portno = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	

     	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		error("ERROR on binding");

//     	listen(sockfd, 5);

//	clilen = sizeof(cli_addr);
//	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
//	if (newsockfd < 0) 
//		error("ERROR on accept");
//	fprintf(stderr,"connection established\n");
	
	gettimeofday(&t0, 0);
	
     	bzero(buf,1024);

	int fdOut = open("copy", O_WRONLY | O_CREAT, 0777);	

	while(n){
		n = read(newsockfd, buf, 1024);
		//write(1, buf, 1024);
		write(fdOut, buf, n);
		if(!fTot%102400)
			fprintf(stderr,".");
		fTot += n;
     	}
	gettimeofday(&t1, 0);
	
	b = (((t1.tv_sec - t0.tv_sec)*1000000L + t1.tv_usec) - t0.tv_usec)/ 1000000;
	a = fTot * 8;

	fprintf(stderr, "\nTime in microsecs: %ld microseconds\n",
		((t1.tv_sec - t0.tv_sec)*1000000L
		+t1.tv_usec) - t0.tv_usec);

	fprintf(stderr, "UDP bits per second: %ld", a/b);

	return 0; 
}

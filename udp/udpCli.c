/* UDP Client */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>


void error(char *msg){
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[]){
	int n, dt, fd, fTot, serverlen;
	long a, b;
	struct sockaddr_in dest;
	char buf[1024];
	struct timeval t1, t0;
	struct hostent *server;
	fTot = 0;
	if (argc < 3){
		fprintf(stderr, "ERROR, no IP or PORT provieded\n[IP] [PORT]\n");
		exit(1);
	}
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	server = gethostbyname(argv[1]);
	if (server == NULL){
		fprintf(stderr, "ERROR, no such host as %s\n", argv[1]);
		exit(0);
	}

	

	bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;

	bcopy((char *) server->h_addr,
		(char *)&dest.sin_addr.s_addr, server->h_length);


	dest.sin_port = htons(atoi(argv[2]));



//	inet_aton(argv[1], (struct in_addr *) &dest.sin_addr.s_addr);

//	if(connect(fd, (struct sockaddr*)&dest,sizeof(dest)) < 0)
//		error("Error connecting\n");

//	fprintf(stderr,"connected\n");

	
	


	



	dt = open("/var/www/cs2751/assignments/nasaPlanetData.45", O_RDONLY);

	serverlen = sizeof(dest);
	
	gettimeofday(&t0, 0);
	
	while(n = read(dt, buf, 1024)){
		sendto(sockfd, buf, 1024, 0, &dest, serverlen);
		fTot+=n;
	}



	//fprintf(stderr,"file opened\n");
	//int n = read (dt, buf, 1024);
	//while(n = read(dt, buf, 1024)){
	//	write(fd,buf,n);
	//	fTot += n;
	//}

	gettimeofday(&t1, 0);

	b = (((t1.tv_sec - t0.tv_sec)*1000000L + t1.tv_usec) - t0.tv_usec)/ 1000000;
	a = fTot * 8;

	fprintf(stderr, "Time in microsecs: %ld microseconds\n",
		((t1.tv_sec - t0.tv_sec)*1000000L
		+t1.tv_usec) - t0.tv_usec);

	fprintf(stderr, "UDP bits per second: %ld", a/b);

	return 0;
}

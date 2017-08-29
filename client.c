#include "MyHeader.h"

// Function that returns the ip of the client.
char  *udp_server_ip() {

int fd;
struct ifreq ifr;
fd = socket(AF_INET,SOCK_DGRAM,0);

// Defining that the wanted ip is an IPv4 address.
ifr.ifr_addr.sa_family = AF_INET;

// IP address attached to "eth0"
strncpy(ifr.ifr_name, "eth0",IFNAMSIZ-1);
ioctl(fd,SIOCGIFADDR, &ifr);
close(fd);

return inet_ntoa(((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr);
}

int main (int argc, char *argv[]) {

int sockfd,listenfd,sockudpserv;
socklen_t udp_clilen;
short conf;
struct sockaddr_in servaddr,udp_servaddr,udp_cli_addr;
char message[512];
data inf;

// Checking the number of the arguments
if (argc<4){
	printf("Error: Not enough arguments\n");
	exit(1);
	}

// Checking if the first argument is valid.
if (argv[1][0]!='-' || argv[1][1]!='s') {
	printf("Error: Missing argument -s\n");
	exit(1);
	}

inf.ip=argv[2];  // Storing argument 2, which is the IP address, into variable ip of struct inf.
inf.tcp_port=atoi(argv[3]); // Same for argument3, which is tcp port.
inf.udp_port=atoi(argv[4]); // Same for argument4, which is udp port.

// Checking if the tcp port is valid.
if (inf.tcp_port<10770 || inf.tcp_port>10779) {
	printf("Error: invalid tcp port number. Tcp port must be between 10770 and 10779\n");
	exit(1);
	}

// Checking if the udp port is valid.
if (inf.udp_port<10770 || inf.udp_port>10779) {
	printf("Error: invalid udp port. Udp port must be between 10770 and 10779\n");
	exit(1);
	}

//udp port should be different from tcp port.
if (inf.udp_port==inf.tcp_port) {
	printf("Error: invalid udp_port. Udp port must be different from tcp_port\n");
	exit(1);
	}

//Create the socket for the client.
sockfd = socket(AF_INET,SOCK_STREAM,0);
//Define the characteristics of connection with server.
bzero (&servaddr,sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_port=htons(inf.tcp_port);
servaddr.sin_addr.s_addr=inet_addr(inf.ip);

//Connecting with the server and checking at the same time.
if(connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))==0) {
printf("Client was successfully connected to server\n");
inf.ip=udp_server_ip(); //Storing the ip of client into variable ip of struct inf.
printf("My ip is: %s\n",inf.ip);
//Sending information about the client to server.
write(sockfd,&inf,sizeof(inf));
}else printf ("There is no server to connect\n");

// Receiving edited information from server.
read(sockfd,&inf,sizeof(data));

//Finding out who is the udp server and printing information.
if (inf.udp_server==1){
	printf("\nI am the udp server\n");
	printf("\nUdp port: %d\n",inf.udp_port);
	//ending the connection with server.
	close(sockfd);
	//Initializing the udp server as done in server.c document.
	listenfd=socket(AF_INET,SOCK_STREAM,IPPROTO_IP);
	bzero(&udp_servaddr,sizeof(udp_servaddr));
	udp_servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	udp_servaddr.sin_family=AF_INET;
	udp_servaddr.sin_port=htons(inf.udp_port);
	bind(listenfd,(struct sockaddr*) &udp_servaddr,sizeof(udp_servaddr));
	listen(listenfd,1);
	
	//Calculating the size of udp client's address for use in accept.
	udp_clilen=sizeof(udp_cli_addr);
	//Connection with the udp client.
	sockudpserv=accept(listenfd,(struct sockaddr*) &udp_cli_addr,&udp_clilen);
	//Checking if the connection is succesfull.
	if (sockudpserv<0) printf("Error: Could not connect with udp client\n");
	else {
	printf("\nSuccessfully connected with udp client\n");
	
	//Receiving messages from udp client until udp client types "exit".
		do {
		read(sockudpserv,&message,sizeof(message));
		printf("%s\n",message);
		} while (strcmp(message,"exit")!=0);
		

	}
//Knowing who is udp server, the other one is udp client.
}else if (inf.udp_server==0){
	//Printing information for udp client.
	printf("I am the udp client\n");
	printf("\nServer's ip: %s\n",inf.ip);
	printf("\nUdp port: %d\n",inf.udp_port);
	//Ending connection with server.
	close(sockfd);
	
	//Waiting for 2seconds for the udp server to be initialized.
	sleep(2);	

	//Create socket for udp client
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	//Defining characteristics about the connection with udp server.
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port=htons(inf.udp_port);
	servaddr.sin_addr.s_addr=inet_addr(inf.ip);

	//Checking if the connection is succesfull.
	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))==0) {
		printf("\nUdp Client was succefully connected to udp server\n");
	
			//Reading messages from keyboard and sending them to udp server.
			do {
			printf("> ");
			scanf("%s",message);
			write(sockfd, &message, sizeof(message));
			} while (strcmp(message,"exit")!=0);	
		
		}else printf("\nThere is no server to connect\n");


}else {
//The case that variable udp_port has negative value.
printf("Error:Connection can not be accomplished. Different udp ports\n");
printf("Udp port: %d\n",inf.udp_port);
	}

}


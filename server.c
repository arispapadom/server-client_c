#include "MyHeader.h"

int main(int argc, char *argv[]) {

	int portnumber,listenfd,sockclient1,sockclient2,r,pid;
	struct sockaddr_in servaddr,cli_addr1,cli_addr2;
	socklen_t clilen1,clilen2;
	data cli_inf1,cli_inf2;
	char *server_ip;

	// Checking the number of arguments
	if(argc<2) {					
		printf("Error: missing arguments\n");
		exit(1);
	}

	// Checking if the first argument is valid
	if (argv[1][0]!='-' || argv[1][1]!='p') {
		printf("Error: Missing arguments -p\n");
		exit(1);
	}
	
	// Store argument 2,which is tcp port, into variable portnumber
	portnumber=atoi(argv[2]);

	//checking if the portnumber is valid
	if (portnumber<10770 || portnumber>10779){
		printf("Error: invalid port number. Port number must be between 10770 and 10779\n");
		exit(1);
	}else {		
		printf("Initialized successful. Port in use: %d\n",portnumber);
	}

	// Create a socket for the server
	listenfd=socket(AF_INET,SOCK_STREAM,IPPROTO_IP); 
	//Defining the  caracteristics of the server
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port=htons(portnumber);
	bind(listenfd, (struct sockaddr* ) &servaddr,sizeof(servaddr));
	listen(listenfd, 4);

	while(1) {


		// Calculating the size of clients one's address for use in accept.
		clilen1=sizeof(cli_addr1);
		// Connection with the client one.
		sockclient1=accept(listenfd,(struct sockaddr *) &cli_addr1, &clilen1);

		// Checking if the connection is successfull.
		if (sockclient1<0) printf("Error: Could not connect with the client\n");
		else { 
			printf("Client one has connected succesfully\n");
			// Read information from client one.
			read(sockclient1,&cli_inf1,sizeof(data));
		}

		// Calculating the size of client2's address for use in accept. 
		clilen2=sizeof(cli_addr2);
		// Connection with client2.
		sockclient2=accept(listenfd,(struct sockaddr *) &cli_addr2, &clilen2);


		// Checking if the connection is successfull.
		if (sockclient2<0) printf("Error: Could not connect with the client\n");
		else {
			printf("Client two has connected succesfully\n");
			// Read information from client2.
			read(sockclient2,&cli_inf2,sizeof(data));
		}

		pid=fork();

		if (!pid) {

			// Random number (0 or 1) and store it to variable r.
			srand(time(NULL));
			r=rand()%2;

			printf("Udp port client's one: %d\n",cli_inf1.udp_port);
			printf("Udp port client's two: %d\n",cli_inf2.udp_port);

			// Checking if the udp ports of the clients are the same
			if (cli_inf1.udp_port==cli_inf2.udp_port) {
				printf("Udp server is client %d\n",r+1);	// Print who is the udp server
				if (r==0){ 			// if r=0, client1 is the udp server
					cli_inf1.udp_server=1;  // variable used as flag for being udp server or not
					cli_inf2.udp_server=0;
					server_ip=cli_inf1.ip;  
					cli_inf1.ip=server_ip;
				}else {			//if r=1, client2 is the udp server
					cli_inf1.udp_server=0;
					cli_inf2.udp_server=1;
					server_ip=cli_inf1.ip;
					cli_inf2.ip=server_ip;
				}
				// Send information back to clients.
				write(sockclient1,&cli_inf1,sizeof(cli_inf1));
				write(sockclient2,&cli_inf2,sizeof(cli_inf2));
			} else { 					//if udp ports are different
				printf("Connection can not be accomplished. Clients have different udp ports\n");
				cli_inf1.udp_server=-1;		//variable used as flag
				cli_inf2.udp_server=-1;
				// send information back to clients that connection has failed.
				write(sockclient1,&cli_inf1,sizeof(cli_inf1));
				write(sockclient2,&cli_inf2,sizeof(cli_inf2));
				// close connections with clients if connection has failed.
				close(sockclient1);
				close(sockclient2);
				exit(1);
			}
			// Close connection with clients if connection has succeed.
			close(sockclient1);
			close(sockclient2);
			exit(1);
		} // if (!pid)

	} //end of while

}





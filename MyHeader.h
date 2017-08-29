#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <time.h>


typedef struct {
	int udp_server;
	char *ip;
	int tcp_port;
	int udp_port;
	} data;

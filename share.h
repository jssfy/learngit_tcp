/* share.h*/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <ctype.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#define SERV_UDP_PORT 9999

#define reqType       short int
#define ADDR_REQ      0
#define ADDR_ACK      1
#define FILESIZE_REQ  2
#define FILESIZE_ACK  3

typedef struct dataUnit {
	reqType type;
	long int data;
	char str[2560000];
} meg;

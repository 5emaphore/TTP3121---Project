#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <sys/stat.h>


#define SERV_TCP_PORT 25000
#define SERV_UDP_PORT 35001
#define CLI_UDP_PORT 35002

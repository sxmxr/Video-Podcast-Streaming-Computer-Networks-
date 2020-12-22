#include <net/if.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>
#define _XOPEN_SOURCE 600 
#include <libgen.h>

int main(int argc, char * argv[])
{
    system("figlet PODCAST");
    printf("\n\nTo access this service follow these steps: ");
    printf("\n 2. RUN this command in any of 1 window --> cd P/Server  ");
    printf("\n 1. RUN this command in that window --> ./server 127.0.0.1 ");
    printf("\n 4. RUN this command in the remaining window --> cd P/Client  ");
    printf("\n 2. RUN this command that window --> ./client 127.0.0.1");
    printf("\n 3. Enjoy the show! \n\n");
    return 0;
}

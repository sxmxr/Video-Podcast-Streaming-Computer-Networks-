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


int main(int argc, char * argv[])
{
    system("figlet MOVIE_MANIA");
    printf("\n\nTo access this service follow these steps: ");
    printf("\n 1. Conver all the video files in the server into stremable format using this command -->  ffmpeg -i input.mp4 -f mpegts output.mp4");
    printf("\n 2. RUN this command in any of 1 window --> cd M/Server  ");
    printf("\n 3. RUN this command in that window --> ./server 8000 ");
    printf("\n 4. RUN this command in the remaining window --> cd M/Client  ");
    printf("\n 5. RUN this command in that window --> ./client 127.0.0.1 8000");
    printf("\n 6. Enjoy the show! \n\n");
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h> 
#include <pthread.h> 

#define SERVER_PORT 5430
#define MC_DATA_PORT_1 5450
#define MC_STREAM_PORT_1 5451
#define MC_DATA_PORT_2 5452
#define MC_STREAM_PORT_2 5453
#define MC_LIVE_STREAM_PORT 5454
#define BUF_SIZE 1024

int pauseFlag = 0;
char *if_name = "enp0s3";
int currentStation = 1;
int streamFlag = 0;
double tSec = 1.0;

struct station_info {
    uint8_t station_number;
    uint8_t station_name_size;
    char station_name[200];
    char multicast_address[16];
    uint16_t data_port;
    uint16_t info_port;
    uint32_t bit_rate;
};

struct site_info {
    uint8_t type;
    uint8_t site_name_size;
    char site_name[200];
    uint8_t site_desc_size;
    char site_desc[200];
    uint8_t station_count;
    struct station_info station_list[2];
};

struct data {
  char buffer[BUF_SIZE];
};

struct streamInfo {
  char songName[200];
  char nextSongName[200];
  int secLeft;
};

struct receiverArguments{
    int socket;
    struct sockaddr_in sin;
    int sLength;
};

void delay(int t)
{
    long pause;
    clock_t now,then;

    pause = t*(CLOCKS_PER_SEC/1000000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}

// Function for receiving stream info.
void *receiveStreamInfo(struct receiverArguments *rStreamStruct)
{
    struct receiverArguments temp;
    temp.socket = rStreamStruct->socket;
    temp.sin = rStreamStruct->sin;
    temp.sLength = rStreamStruct->sLength;

    struct streamInfo sInfo;
    int flag = 1, flag2 = 0;
    int nbrecv = 0;   
    int count = 0; 
    int length = sizeof(temp.sin);
    while(flag == 1)
    {
        if(streamFlag == 1)
            break;
        memset(sInfo.songName, '\0', sizeof(sInfo.songName));
        memset(sInfo.nextSongName, '\0', sizeof(sInfo.nextSongName));
        sInfo.secLeft = 0;

        if((nbrecv = recvfrom(temp.socket, &sInfo, sizeof(sInfo), 0, (struct sockaddr *)&temp.sin, &length)) < 0){
            fprintf(stderr, "fail while receiving data! \n");
            exit(-1);
            return 0;
        }
        printf("Stream Info Received!!\n");
        printf("Song Name:      %s\n", sInfo.songName);
        printf("Next Song Name: %s\n", sInfo.nextSongName);
        printf("Seconds Left:   %d\n\n", sInfo.secLeft);
        usleep(100000);
    }
  
    close(temp.socket);
}

void *receiveLiveData(struct receiverArguments *temp)
{
    clock_t start, end;
    double executionTime = 0.0;
    struct receiverArguments rDataStruct;
    rDataStruct.socket = temp->socket;
    rDataStruct.sin = temp->sin;
    rDataStruct.sLength = temp->sLength;

    struct data d;
    FILE *fp;
    fp = fopen("output.mp4", "w");    // Can recieve any kind of file (txt, jpg, mp4)
    if( fp == NULL) 
    {
        printf("Error has occurred. File could not be opened\n");
        return -1; 
    }

    int flag = 1, flag2 = 0;
    int nbrecv = 0;   
    int count = 0, packet_index = 0; 
    int length = sizeof(rDataStruct.sin);

    start = clock();

    while(flag == 1)
    {
        memset(d.buffer, '\0', sizeof(d.buffer));
        if(pauseFlag == 0)
        {
            if((nbrecv = recvfrom(rDataStruct.socket, &d, sizeof(d), 0, (struct sockaddr *)&rDataStruct.sin, &length)) < 0){
                fprintf(stderr, "fail while receiving data! \n");
                exit(-1);
                return 0;
            }

            count += nbrecv;
            //printf("Packet Number: %i\n\n",packet_index);
            packet_index++;
            fwrite(d.buffer, 1, nbrecv, fp);
            delay(1000);
        }
        else
        {
          printf("Transmission Paused!!\n");
          fclose(fp);
          fp = fopen("output.mp4","w");
          fclose(fp);
          break;
        }
    }

    printf("Packets received: %d\n", packet_index);
    close(rDataStruct.socket);

}

// Function for receiving data.
void *receiveData(struct receiverArguments *temp)
{
    clock_t start, end;
    double executionTime = 0.0;
    struct receiverArguments rDataStruct;
    rDataStruct.socket = temp->socket;
    rDataStruct.sin = temp->sin;
    rDataStruct.sLength = temp->sLength;

    struct data d;
    FILE *fp;
    fp = fopen("output.mp4", "w");    // Can recieve any kind of file (txt, jpg, mp4)
    if( fp == NULL) 
    {
        printf("Error has occurred. File could not be opened\n");
        return -1; 
    }

    int flag = 1, flag2 = 0;
    int nbrecv = 0;   
    int count = 0, packet_index = 0; 
    int length = sizeof(rDataStruct.sin);

    start = clock();

    while(flag == 1)
    {
        memset(d.buffer, '\0', sizeof(d.buffer));
        if(pauseFlag == 0)
        {
            if((nbrecv = recvfrom(rDataStruct.socket, &d, sizeof(d), 0, (struct sockaddr *)&rDataStruct.sin, &length)) < 0){
                fprintf(stderr, "fail while receiving data! \n");
                exit(-1);
                return 0;
            }

            end = clock();
            executionTime = (double)(start-end) / CLOCKS_PER_SEC;

            if(executionTime > tSec)
            {
              fclose(fp);
              fp = fopen("output.mp4","w");    
              start = clock();
            }

            count += nbrecv;
            //printf("Packet Number: %i\n\n",packet_index);
            packet_index++;
            fwrite(d.buffer, 1, nbrecv, fp);
        }
        else
        {
          printf("Transmission Paused!!\n");
          fclose(fp);
          fp = fopen("output.mp4","w");
          fclose(fp);
          break;
        }
    }

    printf("Packets received: %d\n", packet_index);
    close(rDataStruct.socket);

}

struct receiverArguments setUpReceiver(char* mcast_addr, int tempPort)
{
  struct sockaddr_in sin;
  struct ifreq ifr;
  struct ip_mreq mcast_req;
  int s, reuse = 1, len=0;
  char buf[BUF_SIZE];
  int sLength = sizeof(sin);

  printf("Receiver connected to Multicast Group1: %s\n", mcast_addr);
  if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("receiver: socket");
    exit(1);
  }

  // Assign the respective values to the udp family structure. 
  memset((char *)&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_port = htons(tempPort);

  // Use the interface specified 
  memset(&ifr, 0, sizeof(ifr));
  strncpy(ifr.ifr_name , if_name, sizeof(if_name)-1);

  // For Reusing the same port for udp transmission. 
  if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0) {
    perror("setting SO_REUSEADDR");
    close(s);
    exit(1);
  }

  if ((setsockopt(s, SOL_SOCKET, SO_BINDTODEVICE, (void *)&ifr, sizeof(ifr))) < 0)
  {
    perror("receiver: setsockopt() error");
    close(s);
    exit(1);
  }

  // Binding to Socket. 
  if ((bind(s, (struct sockaddr *) &sin, sizeof(sin))) < 0) {
    perror("receiver: bind()");
    exit(1);
  }

  mcast_req.imr_multiaddr.s_addr = inet_addr(mcast_addr);
  mcast_req.imr_interface.s_addr = htonl(INADDR_ANY);

  // Adding Membership to Multicast Group.
  if ((setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*) &mcast_req, sizeof(mcast_req))) < 0) {
    perror("mcast join receive: setsockopt()");
    exit(1);
  }
  printf("Ready to listen!\n");

  // Creating Structure and passing values required for transmission. 
  struct receiverArguments rArg;
  rArg.socket = s;
  rArg.sin = sin;
  rArg.sLength = sLength;

  return rArg;
}

void connectToMulticastGroup(char* mcast_addr)
{
  struct receiverArguments rDataStruct, rStreamStruct, rLiveStruct;
  int dataPort = 0, streamPort = 0;
  if(strcmp(mcast_addr, "225.1.1.1")==0)
  {
      dataPort = MC_DATA_PORT_1;
      streamPort = MC_STREAM_PORT_1;
  }
  else if(strcmp(mcast_addr, "225.2.1.1")==0)
  {
      dataPort = MC_DATA_PORT_2;
      streamPort = MC_STREAM_PORT_2;
  }
  else if(strcmp(mcast_addr, "225.3.1.1")==0)
  {
      dataPort = MC_LIVE_STREAM_PORT;
  }

  if(strcmp(mcast_addr, "225.3.1.1")==0)
  {
    rLiveStruct = setUpReceiver(mcast_addr, dataPort);
    pthread_t thread_id3;
    pthread_create(&thread_id3, NULL, receiveLiveData, &rLiveStruct);
  }
  else
  {
    rDataStruct = setUpReceiver(mcast_addr, dataPort);
    rStreamStruct = setUpReceiver(mcast_addr, streamPort);    
    pthread_t thread_id1, thread_id2;
    pthread_create(&thread_id1, NULL, receiveStreamInfo, &rStreamStruct);
    pthread_create(&thread_id2, NULL, receiveData, &rDataStruct);

  // pthread_t thread_id1, thread_id2;
  // pthread_create(&thread_id1, NULL, receiveStreamInfo, &rStreamStruct);
  // pthread_create(&thread_id2, NULL, receiveData, &rDataStruct);

    char c1, c2;
    int temp;
    while(1)
    {
      scanf("%c", &c1);
      if(c1=='p')
      {
        pauseFlag=1;
        usleep(10);
        //sleep(0.01);
      }

      if(c1=='r')
      {
        pauseFlag = 0;
        usleep(10);
        // printf("Before creating structure!\n");
        rDataStruct = setUpReceiver(mcast_addr, dataPort);
        // printf("After creating structure!\n");
        //rStreamStruct = setUpReceiver(mcast_addr, streamPort);
        //pthread_create(&thread_id1, NULL, receiveStreamInfo, &rStreamStruct);
        pthread_create(&thread_id2, NULL, receiveData, &rDataStruct);
      }

      if(c1=='s')
      {
        //printf("Station change requested\n");
        //while(c2 = getc(stdin))
        //{
        //  printf("While ni andarr!!\n");
          //scanf("%c", &c2);
        //  if(c2=='\n')
        //      break;
          if(currentStation==1)
          {
            temp=2;
          }
          if(currentStation==2)
          {
            temp = 1;
          }

          // if(currentStation == ((int)c2 - (int)('0')))
          // {
          //     printf("same station!");
          // }
          // else
          // {
            printf("diff station\n");
            pauseFlag=1;
            streamFlag=1;
            usleep(10);

            pthread_cancel(thread_id1);
            pthread_cancel(thread_id2);

            //temp = (int)c2 - (int)('0');
            if(temp==1)
            {
              dataPort = MC_DATA_PORT_1;
              streamPort = MC_STREAM_PORT_1;
              strcpy(mcast_addr, "225.1.1.1");
            }
            if(temp==2)
            {
              dataPort = MC_DATA_PORT_2;
              streamPort = MC_STREAM_PORT_2;
              strcpy(mcast_addr, "225.2.1.1");
            }

            currentStation = temp;
            rDataStruct = setUpReceiver(mcast_addr, dataPort);
            rStreamStruct = setUpReceiver(mcast_addr, streamPort);

            pauseFlag = 0;
            streamFlag = 0;
            printf("station changed!\n");
            pthread_create(&thread_id1, NULL, receiveStreamInfo, &rStreamStruct);
            pthread_create(&thread_id2, NULL, receiveData, &rDataStruct);
          // }
          //break;
        //}
      }

    }
  }

}

int main(int argc, char * argv[]){

  struct sockaddr_in sin;
  struct hostent *hp;
  char *host;
  char buf[BUF_SIZE];
  int s, len;
  int serverPort;

  if (argc==3) {
    host = argv[1];
    sscanf(argv[2], "%d", &serverPort);
  }
  else {
    fprintf(stderr, "usage: simplex-talk host\n");
    exit(1);
  }
  /* translate host name into peer's IP address */
  hp = gethostbyname(host);
  if (!hp) {
    fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
    exit(1);
  }
  else
    printf("Client's remote host: %s\n", argv[1]);
  /* build address data structure */
  bzero((char *)&sin, sizeof(sin));
  sin.sin_family = AF_INET;
  bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
  sin.sin_port = htons(serverPort);
  /* active open */
  if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("simplex-talk: socket");
    exit(1);
  }
  else
    printf("Client created socket.\n");

  if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
      perror("simplex-talk: connect");
      close(s);
      exit(1);
    }
  else
    printf("Client connected.\n");

  struct site_info site1;
  len = recv(s, &site1, sizeof(site1), 0);

  printf("Station list received: \n");
  printf("Station Number:    %u\n", site1.station_list[0].station_number);
  printf("Station Name Size: %u\n", site1.station_list[0].station_name_size);
  printf("Multicast Name:    %s\n", site1.station_list[0].station_name);
  printf("Multicast Address: %s\n", site1.station_list[0].multicast_address);
  printf("Data Port:         %u\n", site1.station_list[0].data_port);
  printf("Info Port:         %u\n", site1.station_list[0].info_port);
  printf("Bit Rate:          %u\n\n", site1.station_list[0].bit_rate);

  printf("Station Number:    %u\n", site1.station_list[1].station_number);
  printf("Station Name Size: %u\n", site1.station_list[1].station_name_size);
  printf("Multicast Name:    %s\n", site1.station_list[1].station_name);
  printf("Multicast Address: %s\n", site1.station_list[1].multicast_address);
  printf("Data Port:         %u\n", site1.station_list[1].data_port);
  printf("Info Port:         %u\n", site1.station_list[1].info_port);
  printf("Bit Rate:          %u\n\n", site1.station_list[1].bit_rate);

  printf("Station Number: 3\n");
  printf("Description: Live Stream\n");
  printf("Multicast Address: 225.3.1.1\n");
  printf("Data Port: %d\n", MC_LIVE_STREAM_PORT);

  int receivedStation[site1.station_count];
  int stationSelect, flag = 0;

  while(flag==0)
  {
    printf("Select Station Number: ");
    scanf("%d", &stationSelect);
    if(stationSelect==3)
    {
        connectToMulticastGroup("225.3.1.1");
        flag=1;
        break;
    }
    else
    {
      for(int i = 0; i < site1.station_count; i++)
      {
        if(site1.station_list[i].station_number == stationSelect)
        {
          close(s);
          connectToMulticastGroup(&site1.station_list[i].multicast_address);
          flag=1;
          break;
        }
      }
      if(flag==0)
        printf("No Such Station Exists!\n");  

    }

  }
}

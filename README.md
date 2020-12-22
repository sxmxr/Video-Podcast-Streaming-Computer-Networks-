# Video-Podcast-Streaming- (Computer-Networks)

# Defination #
Markup : 1. Video Streaming over IP : The connections of sockets, to initially receive the information about available stations, is done using TCP. The video files are sent over UDP socket.
         2. Podcast Streaming over IP : An application of internet radio which involves multicasting of data in the form of multimedia over IP. The connection of sockets is initially done using TCP which establishes control channel. Then the multimedia is sent using UDP socket since it is more efficient for multicast type of transmission.

# Execution of the Project
Commands needed to run the source code:
Step-1 Compile Project File : gcc `pkg-config gtk+-3.0 --cflags` project.c -o project `pkg-config gtk+-3.0 --libs`
Step-2 Run compiled File : ./project
Step-3 Follow the instructions given by ./moreinfo_m or ./moreinfo_p
*******************************************************************************************************************
Running Podcast  indvidually : 
SERVER -
1 Compiling: gcc server.c
2 Running: ./a.out 127.0.0.1

Client -
1 Compiling: gcc `pkg-config gtk+-3.0 --cflags` client.c -o client `pkg-config gtk+-3.0 --libs`
2 Running: ./client 127.0.0.1
*******************************************************************************************************************
Running Video indvidually : 
SERVER -
1 Compiling: gcc server.c -o server -lpthread
2 Running: ./server PORTNO

Client -
1 Compiling: gcc client.c -o client -lpthread
2 Running: sudo ./client SERVER_IP PORT

*******************************************************************************************************************
List of all the dependencies and libraries:
Install VLC : sudo apt install vlc
Install GTK3 : sudo apt install libgtk-3-dev
Install ffmpeg : sudo apt install ffmpeg
Converting Videos into streamable format : ffmpeg -i inputfilename.mp4 -f mpegts outputfilename.mp4

ECEN602 Assignment Programming Assignment
----------------------------------

Team Number: 10
Member 1 # Sama, Avani
Member 2 # Li, Wan
---------------------------------------

Design:
--------------------
For the client.c


Use socket() to get the File Descriptor.
Use connect() to connect to server.
After connect, prepare the content of the message struct with username in JOIN function, pack it in JOIN() and use Send_message() to send the message to server.
Then add both the keyboard and the file descriptor into select()read_fds set.
Use select() to check if there is something to read and
     if there is something to read from keyboard,prepare the content of  the message struct with the input message from console and send the message to server; 
     if  there is something to read from file descriptor, receive message from server and show it to screen.

For the server.c
Use socket() to get the File Descriptor.
Use bind() to associate that socket with a port.
Use listen() to see if there is someone want to connect to server from the File Descriptor
use fwd_message() function to forward the message to other clients. 
   Server is able to accept accept clients, get their username and forward the message to others.
---------------------------------------

Description/Comments:
--------------------
1. This package can provide a simple chat service for client and server based on socket programming.
   This package contains three files: server.c, client.c and makefile,
   to generate object files, use: "make -f makefile" in the path of these files in a linux environment.
2. To use the service, first start server by ./server SERVER_IP SERVER_PORT
   SERVER_IP is the IP address of server, SERVER_PORT is the port of server,
   MAX_CLIENTS is the max number of clients is fed as 50 into the server code
   Then clients can enter the chat room by connecting to server using ./client SERVER_IP SERVER_PORT.
thena sk to admit username.
   USERNAME is the username the client choose, SERVER_IP is the IP address of server, 
   SERVER_PORT is the port of server.
3. After server run, number of MAX_CLIENTS clients can connect to the server and enter the chat room to chat.
   Any client successfully enter the chat room can receive a message of the existed clients in the chat room
   as "names of clients in the chat room: username1 username2 ..."
   JOIN and FWD are implemented.
   room will receive the username and message as "<username>:message"
   



Unix command for starting server:
------------------------------------------
./server SERVER_IP SERVER_PORT

Unix command for starting client:
------------------------------------------
./client SERVER_IP SERVER_PORT

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>

#define JOIN 2
#define SEND 4
#define FWD 3
#define STDIN fileno(stdin)
/* structure for the message header */
typedef struct {
unsigned int vrsn:9;
unsigned int type:7;
unsigned int Msg_len:16;
//struct Msg_attribute attribute;
}Msg_header;

typedef struct{
unsigned int type:16;
unsigned int len:16;
char  payload[512];
}Msg_attribute;

typedef struct{
Msg_header header;
Msg_attribute attribute;
}message;

void error(char *msg)
{
perror(msg);
exit(0);
}

int32_t pack(message *msg)
{}

void join(char *buf,int socketfd,int size_buf)
{
//struct Msg_header header;
//struct Msg_attribute attribute;
message Msg;
Msg.header.vrsn=3;
printf("Vrsn is %d\n",Msg.header.vrsn);
Msg.header.type=JOIN;
printf("Type is %d\n",Msg.header.type);
Msg.attribute.type=2;
Msg.attribute.len=(4+size_buf);
//memcpy(Msg.attribute.payload,buf,(sizeof(buf)+1));
bzero((char*)&Msg.attribute.payload,sizeof(Msg.attribute.payload));
int n=0;
for (n=0;n<size_buf;n++)
Msg.attribute.payload[n]=buf[n];
printf("payload is %s\n",Msg.attribute.payload);
Msg.header.Msg_len=(8+size_buf);
printf("Msg is : %s\n",Msg);
/*attribute.type=2;
attribute.len=(4+sizeof(buf));
mencpy(attribute.payload,buf,(sizeof(buf)+1));*/
if(write(socketfd,&Msg,sizeof(Msg))<0)
{
error("ERROR write to socket");
}
printf("your JOIN message has been sent\n");
}


void send_message(int socketfd)
{
message Msg;
char buf[512];
int size_buf;
fgets(buf,sizeof(buf)-1,stdin);
size_buf=strlen(buf)-1;
if (buf[size_buf]=='\n')
buf[size_buf]='\0';
// pack the JOIN information
Msg.header.vrsn=3;
printf("Vrsn is %d\n",Msg.header.vrsn);
Msg.header.type=SEND;
printf("Type is %d\n",Msg.header.type);
Msg.attribute.type=4;
Msg.attribute.len=(4+size_buf);
bzero((char*)&Msg.attribute.payload,sizeof(Msg.attribute.payload));
int n=0;
for (n=0;n<size_buf;n++)
Msg.attribute.payload[n]=buf[n];
printf("payload is %s\n",Msg.attribute.payload);
Msg.header.Msg_len=(8+size_buf);
printf("Msg is : %s\n",Msg);
if(write(socketfd,&Msg,sizeof(Msg))<0)
{
error("ERROR write to socket");
}
printf("You have recieved a message sucessfully\n");
}

int main(int argc, char *argv[])
{
int sockfd, portno, n;
struct sockaddr_in serv_addr;
struct hostent *server;
struct timeval tv;
message msg;
//int msglen=sizeof(msg);

fd_set master;
fd_set read_fds;
int fdmax;
int i,j,rv,templen;
int msglen=sizeof(msg);

tv.tv_sec=10;
tv.tv_usec=500000;

char buffer[512],username[16],temp[16];
int buf_len;

FD_ZERO(&master);
FD_ZERO(&read_fds);
FD_SET(fileno(stdin),&read_fds);

if (argc < 3)
{
fprintf(stderr,"usage %s hostname port\n", argv[0]);
exit(0);
}

portno = atoi(argv[2]);

sockfd = socket(AF_INET, SOCK_STREAM, 0);


if (sockfd < 0)
{
error("ERROR opening socket");
                                     }
server = gethostbyname(argv[1]);
if (server==NULL)
{
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
}
bzero((char *) &serv_addr, sizeof(serv_addr));
serv_addr.sin_family = AF_INET;

bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);

serv_addr.sin_port = htons(portno);

if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0)
{
error("ERROR connecting");
}
printf("Please enter the username: ");
bzero(username,16);
fgets(username,15,stdin);
/*new add code to join the chat */
buf_len=strlen(username);
join(username,sockfd,buf_len);
FD_SET(STDIN,&read_fds);
FD_SET(sockfd,&read_fds);
//fdmax=sockfd;

while(1)
{
//read_fds=master;
printf("You are already in a while loop\n");
fdmax=sockfd;
if (select(fdmax+1,&read_fds,NULL,NULL,NULL)==-1)
{
perror("select");
exit(4);}
printf("You have select successufly\n");
for (i=0;i<=fdmax;i++)
{
if(FD_ISSET(i,&read_fds)){
if (i==sockfd){
// handle the message recieved from the server;display the message i have recieved success
n=read(sockfd,(struct message*)&msg,msglen);
//n=recv(sockfd,buffer,520,0);
//printf("buffer is :::%s\n",buffer);
if (n<0)
{error("ERROR reading from socket");}
printf("VRSN is %d\n",msg.header.type);
if (msg.header.vrsn!=FWD)
{error("Message Version is wrong");}
if (msg.header.type=='3')
{
if (msg.attribute.type=='2')
{
printf("The user on chat is:%s",msg.attribute.payload);
templen=strlen(msg.attribute.payload);
msg.attribute.payload[templen]='\0';
strcpy(temp,msg.attribute.payload);
}
if (msg.attribute.type==4)
printf("User %s say: %",temp,msg.attribute.payload);
}
}
if (i==STDIN)
{
//deal with the message we write
send_message(sockfd);
}
}
}
FD_SET(STDIN,&read_fds);
FD_SET(sockfd,&read_fds);
}

close(sockfd);
return 0;
}



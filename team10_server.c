#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>


#define MAX_CLIENTS    50
#define JOIN 2
#define SEND 4
#define FWD 3




/* structure for the message header */

struct msg_header{
unsigned int vrsn:9;
unsigned int type:7;
unsigned int Msg_len:16;
} ;

/*structure for the message attribute*/

struct msg_attribute{
unsigned int type:16;
unsigned int len:16;
char  payload[512];
};

struct message{
struct msg_header header;
struct msg_attribute attribute;
};

void error(char *msg)
{
    perror(msg);
    exit(0);
}
/*void join(char *buf,int socketfd,int size_buf)
{
    //struct Msg_header header;
    //struct Msg_attribute attribute;
    struct message Msg;
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

    if(write(socketfd,&Msg,sizeof(Msg))<0)
    {
        error("ERROR write to socket");
    }
    printf("your JOIN message has been sent\n");
}*/
void fwd_message(char *buf,int socketfd,int size_buf)
{
    struct message Msg;
// char buf[512];
//  int size_buf;
//   fgets(buf,sizeof(buf)-1,stdin);
    size_buf=strlen(buf)-1;
    if (buf[size_buf]=='\n')
        buf[size_buf]=='\0';
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
    printf("You have sent a message sucessfully\n");
}



int main(int argc, char *argv[])
 {
fd_set main;
fd_set readfds;
int fdmax;
int listenfd;
int sockfd, newsockfd, portno, clilen;
struct message msg;
char buffer[256];
struct sockaddr_in serv_addr, cli_addr;
int yes=1;
int i,j,rv;
int n;
struct addrinfo ping, *ai, *p;


FD_ZERO(&main);
FD_ZERO(&readfds);

/*memset(&ping,0,sizeof(ping));
ping.ai_family = AF_INET;
ping.ai_socktype = SOCK_STREAM;
if ((rv= getaddrinfo(argv[1],argv[2],&ping,&ai)) !=0)
{error("ERROR");
exit(1);
}*/
     

if (argc < 2)
 {
 fprintf(stderr,"ERROR, no port provided\n");
 exit(1);
 }
/*Socket Settings*/

/*for(p=ai;p!=NULL;p = p->ai_next) {
        listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listenfd < 0)
        {       continue;}

        setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));

        if (bind(listenfd,p->ai_addr, p->ai_addrlen) <0)
        {       close(listenfd);
                continue;
        }
        break;
}

     if (p==NULL) {fprintf(stderr,"server fail to bind\n");
         exit(2);
     }
     freeaddrinfo(ai);
if (listen(listenfd,10)==-1)
{
    perror("listen");
    exit(3);
}*/

sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd < 0)
 {
 error("ERROR opening socket");
 }

bzero((char *) &serv_addr, sizeof(serv_addr));

portno = atoi(argv[1]);

serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = INADDR_ANY;
serv_addr.sin_port = htons(portno);

if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
 {
 error("ERROR on binding");
 }

if(listen(sockfd,5)<0)
{error("socket listening failed");
return 1;
}
printf("<[Server Started]>\n");

/*Accept Clients*/
     FD_SET(sockfd,&main);
     fdmax=sockfd;

//while(1)
//{
    /*readfds=main;
    if(select(fdmax+1,&readfds,NULL,NULL,NULL)==-1){
        perror("select");
        exit(4);
    }
    for(i=0;i<fdmax;i++){
        
        if(FD_ISSET(i,&readfds)){
            if(i==sockfd){*/
                clilen = sizeof(cli_addr);
                
                newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
                
                if (newsockfd < 0)
                {
                    error("ERROR on accept");
                }
                
                int msglen= sizeof(msg);
                bzero((struct message *) &msg,msglen);
                n=read(newsockfd,(struct message *) &msg,msglen);
 //               int i;
                printf("username is ");
                for(i=0;i<(msg.attribute.len-4);i++)
                    printf("%c",(msg.attribute.payload[i]));
                
                printf("%d \n",msg.header.vrsn);
                if (n < 0)
                {
                    error("ERROR reading from socket");
                }
                if (msg.header.vrsn !=3)
                {       error("version not compatible");
                }
                
                if (msg.attribute.type ==2)
                {
                    char join[]="You have joined the comversation\n";
                    printf("used fwd_message\n");
                    fwd_message(join,newsockfd,strlen(join));
                    /*n=write(newsockfd,"You have joined the conversation",34);
                    if (n<0)
                    {error("error writing to socket");
                    }*/
                    
                }
                
                
                n = write(newsockfd,"I got your message",18);
                
                if (n < 0)
                {
                    error("ERROR writing to socket");
                }
            //}
            /*else{FD_SET(newsockfd,&main);
            if(newsockfd>fdmax)
            {fdmax=sockfd;}
            }
        }
    }*/
    
 //   }
     close(sockfd);
     return 0;
     
}

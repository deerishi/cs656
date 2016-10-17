#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "bits/stdc++.h"
using namespace std;
void error(const char *msg)
{
    perror(msg);
    exit(1);
}
int findUsablePort(int lastPort)
{
    int sockfd, newsockfd, portno;
    struct sockaddr_in serv_addr, cli_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //-----------------------------------
    if (sockfd < 0) 
        error("ERROR opening socket");
        
    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    portno = lastPort+1;
    serv_addr.sin_family = AF_INET; //-----------------------------------
    serv_addr.sin_addr.s_addr = INADDR_ANY; //-----------------------------------
    int r=0;
    do
    {

        portno++;
        serv_addr.sin_port = htons(portno);
        r=bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr));
        cout<<"r is "<<r<<"\n";
     
     }while(r!=0);
     cout<<"the usable port num is "<<portno<<"\n";
     close(sockfd);
     return portno;
}

int main(int argc, char *argv[])
{
     if (argc < 2) {
         fprintf(stderr,"ERROR, no request code provided\n");
         exit(1);
     }
     
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     sockfd = socket(AF_INET, SOCK_STREAM, 0); //-----------------------------------
     if (sockfd < 0) 
        error("ERROR opening socket");
        
     bzero((char *) &serv_addr, sizeof(serv_addr));
    
     portno = 1234;
     cout<<"portno is "<<portno<<"\n";
     serv_addr.sin_family = AF_INET; //-----------------------------------
     serv_addr.sin_addr.s_addr = INADDR_ANY; //-----------------------------------
    int r=0;
     do
     {

        portno++;
        serv_addr.sin_port = htons(portno);
        r=bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr));
        cout<<"r is "<<r<<"\n";
     }while(r!=0);
     cout<<"the usable port num is "<<portno<<"\n";
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,256);
     
     
     int n = read(newsockfd,buffer,255);
     
     if (n < 0) error("ERROR reading from socket");
     
     int clientInt=atoi(buffer);
     
     cout<<"client sent "<<clientInt<<"\n";
     if(clientInt!=atoi(argv[1])) 
     {
        cout<<"code not same , closing the tcp connection\n";
        close(newsockfd);
     close(sockfd);
     return 0;
     }
     cout<<"code same\n";
     //Now we check for a random port 
     int newPort=findUsablePort(portno);
     cout<<"the new random port is "<<newPort<<"\n";
     char mes[100],*ptr;
     ptr=mes+95;
     *ptr--='\0';
     int temp=newPort;
     while(temp!=0)
     {
        *ptr--=temp%10 +'0';
        
        temp/=10;
     }
     ptr++;
     n = write(newsockfd,ptr,strlen(ptr));
     
    
     
}

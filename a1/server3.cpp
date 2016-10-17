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
int main(int argc, char *argv[])
{

    if (argc < 2) {
       fprintf(stderr,"usage %s key\n", argv[0]);
       exit(0);
    }
    
    int reqKey=atoi(argv[1]);

    int sockfdTcp,sockfdUdp,sockfdAcceptTcp;
    struct sockaddr_in serv_addrTcp,serv_addrUdp;
    bzero((char *)&serv_addrTcp,sizeof(serv_addrTcp));
    bzero((char *)&serv_addrUdp,sizeof(serv_addrUdp));
    struct sockaddr_in clientTcp,clientUdp;
    
    
    
    sockfdTcp=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    
    if(sockfdTcp<0)
    {
        error("Unable to open the TCP socket\n");
    }
    
    //First find a fixed port num
    int portNumTcp=1233,portNumUdp,r=0;
    //Setting up server configration
    
    serv_addrTcp.sin_family=AF_INET;
    serv_addrTcp.sin_addr.s_addr=INADDR_ANY;
    do
    {
        portNumTcp++;
        serv_addrTcp.sin_port=htons(portNumTcp);
        r=bind(sockfdTcp,(struct sockaddr *) &serv_addrTcp,sizeof(serv_addrTcp));
        
    }while(r!=0);
    cout<<"SERVER_PORT="<<portNumTcp<<"\n";
   listen( sockfdTcp,5);
   socklen_t clilenTcp,clilenUdp;
   clilenTcp=sizeof(clientTcp);

   sockfdAcceptTcp=accept(sockfdTcp,( struct sockaddr *)&clientTcp,&clilenTcp);
   
   if (sockfdAcceptTcp < 0) 
          error("ERROR on accept");
    
    char dataFromClient[256];
    bzero(dataFromClient,256);
    
    int n=read(sockfdAcceptTcp,dataFromClient,255);
    if (n < 0) error("ERROR reading from socket");
    
    //cout<<"dataFromClient is :"<<dataFromClient<<"\n";
    
    if(atoi(dataFromClient)!=reqKey)
    {
        close(sockfdAcceptTcp);
        close(sockfdTcp);
        error("Invalid key from client\n");
    }
    
    //NOw if the key was valid then we generate another port number for the udp connection , give that to the client 
    // then close the TCP connection
    
    
    sockfdUdp=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    
    if(sockfdUdp<0)
        error("Unable to set up a UDP socket\n");
    
    
    serv_addrUdp.sin_family=AF_INET;
    serv_addrUdp.sin_addr.s_addr=INADDR_ANY;
    
    portNumUdp=portNumTcp;
    r=0;
    do
    {
        portNumUdp++;
        serv_addrUdp.sin_port=htons(portNumUdp);
        r=bind(sockfdUdp,(struct sockaddr *)&serv_addrUdp,sizeof(serv_addrUdp));
    }while(r!=0);
    
    //cout<<"The UDP transaction port is "<<portNumUdp<<"\n";
    
    char dataToClient[256];
    bzero(dataToClient,256);
    
    char *ptr=dataToClient +200;
    int temp1=portNumUdp;

    
    while(temp1!=0)
    {
        *ptr--=temp1%10 +'0';
        temp1/=10;
    }
    ptr++;
    n=write(sockfdAcceptTcp,ptr,strlen(ptr));
     if (n < 0) error("ERROR writing to socket");
     
    close(sockfdAcceptTcp);
    close(sockfdTcp);
    //Now we go into an infinite UDP loop
    //cout<<"\n TCP is now closed, now we start the UDP transaction\n";
    clilenUdp=sizeof(clientUdp);
    int flag2,flag3;
    char dataFromClientUdp[2048];
    clilenUdp=sizeof(clientUdp);
    char reversedString[2048];
    
    while(1)
    {
        bzero(dataFromClientUdp,2048);
        bzero(reversedString,2048);
        flag2=recvfrom(sockfdUdp,dataFromClientUdp,2048,0,( struct sockaddr *)&clientUdp,&clilenUdp);
        //cout<<"recieved from UDP : "<<dataFromClientUdp<<"\n";
        //Now we need to reverse the string sent from UDP by the  client
        ptr=reversedString;
        /*for(auto i=0;i<strlen(dataFromClientUdp)/2;i++)
        {
            swap(dataFromClientUdp[i],dataFromClientUdp[strlen(dataFromClientUdp)-i-1]);
        } */ 
        for(int i=strlen(dataFromClientUdp)-1,j=0;i>=0;i--,j++)
        {
            //swap(dataFromClientUdp[i],dataFromClientUdp[strlen(dataFromClientUdp)-i-1]);
            reversedString[j]=dataFromClientUdp[i];
        }  
        //cout<<"the reversed string is  "<<reversedString<<"\n";
        flag3=sendto(sockfdUdp,reversedString, strlen(reversedString), 0, (struct sockaddr *)&clientUdp, sizeof(clientUdp));

    }
   
    return 0;   
}

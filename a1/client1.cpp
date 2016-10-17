#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "bits/stdc++.h"
using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    /*sockaddr in
The second structure that helps you to reference to the socket's elements is as follows −

struct sockaddr_in {
   short int            sin_family;
   unsigned short int   sin_port;
   struct in_addr       sin_addr;
   unsigned char        sin_zero[8];
};
Here is the description of the member fields −

Attribute	Values	Description
sa_family	
AF_INET

AF_UNIX

AF_NS

AF_IMPLINK

It represents an address family. In most of the Internet-based applications, we use AF_INET.
sin_port	Service Port	A 16-bit port number in Network Byte Order.
sin_addr	IP Address	A 32-bit IP address in Network Byte Order.
sin_zero	Not Used	You just set this value to NULL as this is not being used.*/
    struct sockaddr_in serv_addr;
    
    /*hostent
This structure is used to keep information related to host.

struct hostent {
   char *h_name; 
   char **h_aliases; 
   int h_addrtype;  
   int h_length;    
   char **h_addr_list
	
#define h_addr  h_addr_list[0]
};
Here is the description of the member fields −

Attribute	Values	Description
h_name	ti.com etc.	It is the official name of the host. For example, tutorialspoint.com, google.com, etc.
h_aliases	TI	It holds a list of host name aliases.
h_addrtype	AF_INET	It contains the address family and in case of Internet based application, it will always be AF_INET.
h_length	4	It holds the length of the IP address, which is 4 for Internet Address.
h_addr_list	in_addr	For Internet addresses, the array of pointers h_addr_list[0], h_addr_list[1], and so on, are points to structure in_addr.
char *h_addr;
This is a synonym for h_addr_list[0]; in other words, it is the first host address.
NOTE − h_addr is defined as h_addr_list[0] to keep backward compatibility.*/
    struct hostent *server;

    char buffer[2048];
    if (argc < 5) {
       fprintf(stderr,"usage %s hostname port req-code string\n", argv[0]);
       exit(0);
    }
    
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //-----------------------------------
    
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);//-----------------------------------
    
    
    
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; //-----------------------------------
    
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);//-----------------------------------
         
    serv_addr.sin_port = htons(portno); //-----------------------------------
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) //-----------------------------------
        error("ERROR connecting");
    
    //printf("Please enter the message: ");
    bzero(buffer,2048);
    //fgets(buffer,255,stdin);
    
    bcopy(argv[2],buffer,strlen(argv[2]));
    
    n = write(sockfd,buffer,strlen(buffer));//-----------------------------------
    if (n < 0) 
         error("ERROR writing to socket");
    
    bzero(buffer,256);
    n = read(sockfd,buffer,255);//-----------------------------------
    if (n < 0) 
         error("ERROR reading from socket");
    //printf("Server sent port for transaction = %d\n",atoi(buffer));
    close(sockfd);
    
    //cout<<"\n now beginnning transaction stage by UDP\n";
    //Now we have the random port for transaction for the client for UDP transaction
    
    int sockfdUdp,portNumUdp=atoi(buffer);
    
    const char *my_message = "this is a test message";
    
    char dataFromServerUDP[2048];
    
    struct sockaddr_in serv_addrUdp;
    bzero((char* )&serv_addrUdp,sizeof(serv_addrUdp));
    sockfdUdp=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    
    if(sockfdUdp<0)
        error("Unable to set up a UDP socket\n");
        
    
    socklen_t serverLen=sizeof(serv_addrUdp);
    
    
    serv_addrUdp.sin_port=htons(portNumUdp);
    serv_addrUdp.sin_family=AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addrUdp.sin_addr.s_addr,server->h_length);
     
    //Now we just need to send the string to the server by UDP 
    if (sendto(sockfdUdp,my_message, strlen(my_message), 0, (struct sockaddr *)&serv_addrUdp, sizeof(serv_addrUdp)) < 0)
	error("UDP sendto server failed");
	
	bzero(dataFromServerUDP,2048);
	int flag2=recvfrom(sockfdUdp,dataFromServerUDP,2048,0,( struct sockaddr *)&serv_addrUdp,&serverLen);
	cout<<"Received from server via UDP protocol : "<<dataFromServerUDP<<"\n";
	close(sockfdUdp);
	return 0;
    
}

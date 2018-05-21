#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#define SERVNAME 3847

void printsin(struct sockaddr_in *s_in, char *pname, char *msg) {
  printf("%s\n", pname);
  printf("%s ", msg);
  printf("ip= %s , port= %d", inet_ntoa(s_in->sin_addr), s_in->sin_port);//gets the IP address and port respectively
  printf("\n");
}

int main(int argc, char *argv[])
{
  int socket_fd, cc, fsize; //creates a new socket
  struct sockaddr_in  s_in, from; 
  struct { char head; u_long  body; char tail;} msg; 

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); //AF_INET: IPV4 protocol, SOCK_DGRAM: for UDP protocol, 0:default

  memset((char *) &s_in,0, sizeof(s_in));  /* They say you must do this    */

  s_in.sin_family = (short)AF_INET; //address family
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD *///internet address
  s_in.sin_port = htons((u_short)0x3333); //port number

  printsin( &s_in, "RECV_UDP", "Local socket is:"); 
  fflush(stdout);//clears the space used by the print function

  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in)); //creates the socket with the socket descriptor, 
  //s_in: point to the sockadder with the IP and port in which you connect to

  while(1) {
    fsize = sizeof(from);
    cc = recvfrom(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize);//recives a message and stores it in msg
    printsin( &from, "recv_udp: ", "Packet from:");//prints the message using the printsin function
    printf("Got data ::%c%ld%c\n",msg.head,(long) ntohl(msg.body),msg.tail); 
    fflush(stdout);
   
    msg.head = '<';
    msg.body = htonl(SERVNAME);//creates the message to send
    msg.tail = '>';

    // Send the message to the socket 
    sendto(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from, fsize);
    printf("Server sent data ::%c%ld%c\n",msg.head, (long) ntohl(msg.body), msg.tail); 
    fflush(stdout);
   
  }
  
  return 0;
}

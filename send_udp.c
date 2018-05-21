#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#define CLNAME 2192

int main(int argc, char *argv[])
{
  int socket_fd; //socket descriptor, returned by the socket() function
  struct sockaddr_in  dest; //creates a socket with the name dest. in stands for internet 
  struct hostent *hostptr;
  struct { char head; u_long body; char tail; } msgbuf;

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);//sets up the socket with IPV4 protocol, UDP protocol and 0 for default
  memset((char *) &dest,0, sizeof(dest)); /* They say you must do this */ //deletes everything so it will have room for the messege
  hostptr = gethostbyname(argv[1]);//
  dest.sin_family = (short) AF_INET;//IPV4
  memcpy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);
  dest.sin_port = htons((u_short)0x3333);//port number, host to network short

  msgbuf.head = '<';
  msgbuf.body = htonl(getpid()); /* IMPORTANT! */ //the messege you want to send
  msgbuf.tail = '>';

  sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,
                  sizeof(dest)); //sends the info with UDP protocol, through the socket descriptor we defined earlier, 
                  //&msgbuf point to the messege we want to send, sizeof: size of the messege in bytes, 0 for flags,
                  //the rest points to the socket with the information
                  
  msgbuf.head = '<';
  msgbuf.body = htonl(CLNAME);
  msgbuf.tail = '>';

  // Send the message to the socket 
  sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest, sizeof(dest));
  printf("Client sent data ::%c%ld%c\n",msgbuf.head, (long) ntohl(msgbuf.body), msgbuf.tail); 
  // Receives a message and store it into msgbuf varibale
  recvfrom(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest, sizeof(dest));

  // Prints the message
  printf("Client got data ::%c%ld%c\n",msgbuf.head,(long) ntohl(msgbuf.body),msgbuf.tail); 


  return 0;
}
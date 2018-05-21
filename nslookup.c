
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
  struct addrinfo* res;
  char* hostname;
  char* hostaddr;
  struct sockaddr_in* saddr;
  
  if (argc != 2) {
    perror("Usage: hostnamelookup <DESKTOP-LOLERP>\n");//this check if the hostname was sent
    exit(1);
  }

  hostname = argv[1];//this is how you get the hostname
  
  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) {
    fprintf(stderr, "Error in resolving hostname %s\n", hostname);//gets the address using the string above (hostname)
    exit(1);
  }
  
  saddr = (struct sockaddr_in*)res->ai_addr;//puts the hostname as a string
  hostaddr = inet_ntoa(saddr->sin_addr);

  printf("Address for %s is %s\n", hostname, hostaddr);
  exit(0);
}
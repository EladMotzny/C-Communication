#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>    
#include <strings.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>    
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#define SIM_LENGTH 10 
#define IP_ADDRESS "127.0.0.1"
#define PORT 1337 

int main(int argc, char* argv[])
{ 
  struct addrinfo* res;
  char* hostname;
  char* hostaddr;
  struct sockaddr_in* saddr;
  int sock; 
  struct sockaddr_in cli_name; 
  int count;
  int value; 
//this if checks if the client name was sent, if not exit the program
  if (argc != 2) {
    perror("Usage: net_client.o <hostname>\n");
    exit(1);
  }
//get the hostname
  hostname = argv[1];
  
  //get the address using the hostname string
  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) {
    fprintf(stderr, "Error in resolving hostname %s\n", hostname);
    exit(1);
  }

  saddr = (struct sockaddr_in*)res->ai_addr;
  hostaddr = inet_ntoa(saddr->sin_addr);//resolves the hostname as a string


  printf("Client is alive and establishing socket connection.\n");
  
  
  sock = socket(AF_INET, SOCK_STREAM, 0);//create a socket stream
  if (sock < 0)
    { perror ("Error opening channel");
      close(sock);
      exit(1);
    }
      

  bzero(&cli_name, sizeof(cli_name)); //these lines defines the properties you need to connect to the socket
  cli_name.sin_family = AF_INET; 
  cli_name.sin_addr.s_addr = inet_addr(hostaddr); 
  cli_name.sin_port = htons(PORT);


  if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0)//return 0 if you connected to the socket, print error and exit if you didnt
    { perror ("Error establishing communications");
      close(sock);
      exit(1);
    }


  for (count = 1; count <= SIM_LENGTH; count++)//loop 10 times
    { read(sock, &value, 4);//put the value from the socket into value variable and print it
      printf("Client has received %d from socket.\n", value);
    }

  printf("Exiting now.\n");

  close(sock); //close the socket after the program has ended
  exit(0); 

} 
//Team Memebers: Prakash Kothapalli and Sai Sireesha Tirumala Pudi

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
#include <time.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd)
{
  int number_to_send; // Put your value
  int32_t converted_number;
  int size=sizeof(converted_number);
  //long int ss=0;
  char buff[MAX];
  srand(time(0));
  for (;;)
  {
      sleep(2);
      if (read(sockfd, buff, sizeof(buff)) < 0)
      {
              fprintf(stderr, "read() error\n");
      }
      if(strncmp("you can now play",buff,12) == 0)
      {
            printf("\nReferee: %s \n", buff);
            number_to_send = rand() % 6 + 1;
            printf("My Score: %d \n",number_to_send);
            printf("\n-----###-----\n");
            converted_number = htonl(number_to_send);
            write(sockfd, &converted_number, size);
      }
      else if(strncmp("Game over: you won the game",buff,27) == 0)
      {
          printf("I won the game\n\n");
          exit(0);
      }
      else if(strncmp("Game over: you lost the game",buff,27) == 0)
      {
          printf("I lost the game\n\n");
          exit(0);
      }
  }
  exit(0);
}

int main()
{
  char buffer[1024];
  int sockfd, connfd;
  struct sockaddr_in servaddr, cli;
  // Socket creation and verification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
  {
    printf("Socket creation failed...\n");
    exit(0);
  }
  else
  printf("Socket successfully created..\n");
  bzero(&servaddr, sizeof(servaddr));
  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servaddr.sin_port = htons(PORT);
  // connect the client socket to server socket
  if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0)
  {
    printf("Connection with the server failed...\n");
    exit(0);
  }
  else
  printf("Connected to the server..\n");
  // function for chat
  func(sockfd);
  // close the socket
  close(sockfd);
}

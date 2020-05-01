//Team Memebers: Prakash Kothapalli and Sai Sireesha Tirumala Pudi

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// Function designed for chat between client and server.

void servicePlayers(int fd1[])
{
  char buff[MAX];
  int n;
  int32_t received_int=0;
  int32_t received_int_client=0;
  int TOTO_TOTAL =0;
  int TITI_TOTAL =0;
  int size=sizeof(int32_t);
// infinite loop for chat
  printf("\n------Game Started------ \n \n");
  for (;;)
  {
      sleep(1);
      strcpy (buff, "you can now play");
      write(fd1[0], buff, sizeof(buff));
      //printf("**------Server------**:%s \n",buff);
      printf("\nReferee: %s \n",buff);
      read(fd1[0], &received_int, size);
      printf("TOTO: %d \n", ntohl(received_int));
      TOTO_TOTAL+=ntohl(received_int);
      printf("TOTO's total: %d \n\n",TOTO_TOTAL);
      printf("------****------\n");
      sleep(2);
      if(TOTO_TOTAL >= 100)
      {
          printf("Game Over: TOTO won the game \n\n");
          strcpy (buff, "Game over: you won the game");
          write(fd1[0], buff, sizeof(buff));
          //bzero(buff, sizeof(buff));
          strcpy (buff, "Game over: you lost the game");
          write(fd1[1], buff, sizeof(buff));
          exit(0);
      }
      strcpy (buff, "you can now play");
      write(fd1[1], buff, sizeof(buff));
      printf("Referee: %s \n",buff);
      read(fd1[1], &received_int_client, size);
      printf("TITI: %d \n", ntohl(received_int_client));
      TITI_TOTAL+=ntohl(received_int_client);
      printf("TITI's total: %d \n\n",TITI_TOTAL);
      printf("------****------\n");
      if(TITI_TOTAL >= 100)
      {
            printf("Game Over: TITI won the game \n\n");
            strcpy (buff, "Game over: you won the game");
            write(fd1[1], buff, sizeof(buff));
            bzero(buff, sizeof(buff));
            strcpy (buff, "Game over: you lost the game");
            write(fd1[0], buff, sizeof(buff));
            //break;
            exit(0);
      }
      else if(TOTO_TOTAL == 100 && TITI_TOTAL == 100)
      {
          servicePlayers(fd1);
      }
}
close(fd1[0]);
close(fd1[1]);
exit(0);
}

// Main function

int main()
{
  int sockfd, connfd;
  pid_t childpid;
  socklen_t addr_size;
  int fd1[3];
  struct sockaddr_in servaddr, cli;

  // Socket creation and verification

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    printf("socket creation failed...\n");
    exit(0);
    }
  else
    printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
    // Assign IP, PORT
    memset(&servaddr, '\0', sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    // Binding newly created socket to given IP and verification
    int bind_check;
    bind_check = bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if(bind_check < 0)
    {
        printf("[-]Error in binding.\n");
        exit(1);
    }
    printf("[+]Bind to port \n");
    if(listen(sockfd, 10) == 0)
    {
      printf("[+]Listening....\n");
    }
    else
    {
      printf("[-]Error in binding.\n");
    }
    int k =0;
    int a=0;
    while(1)
    {
      connfd = accept(sockfd, (struct sockaddr*)&cli, &addr_size);
      if(connfd < 0)
      {
        exit(1);
      }
      printf("Connection accepted from %s:%d\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
      fd1[a]=connfd;
      a++; //fd number
      k++; //client count
      if(k == 2)
      {
          if((childpid = fork()) == 0)
          {
              servicePlayers(fd1);
          }
      }
    }
    // After game close the socket
    close(sockfd);

}

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
  int serversocket, port;
  struct sockaddr_in serveraddr, clientaddr;
  socklen_t len;
  char message[100], reply[100];

  serversocket = socket(AF_INET, SOCK_DGRAM, 0);
  if (serversocket < 0)
  {
    perror("Socket creation failed");
    return 1;
  }

  bzero((char *)&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;

  printf("Enter port number: ");
  scanf("%d", &port);

  serveraddr.sin_port = htons(port);
  serveraddr.sin_addr.s_addr = INADDR_ANY;

  if (bind(serversocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
  {
    perror("Bind failed");
    close(serversocket);
    return 1;
  }

  printf("Waiting for client...\n");

  len = sizeof(clientaddr);

  while (1)
  {
    bzero(message, 100);

    int n = recvfrom(serversocket, message, sizeof(message) - 1, 0,
                     (struct sockaddr *)&clientaddr, &len);

    if (n < 0)
    {
      perror("Receive failed");
      continue;
    }
    message[n] = '\0';

    if (strcmp(message, "exit") == 0)
    {
      printf("Client exited\n");
      break;
    }

    printf("Client: %s\n", message);

    printf("Server: ");
    scanf(" %[^\n]", reply);

    if (sendto(serversocket, reply, strlen(reply) + 1, 0,
               (struct sockaddr *)&clientaddr, len) < 0)
    {
      perror("Send failed");
    }

    if (strcmp(reply, "exit") == 0)
      break;
  }

  close(serversocket);

  return 0;
}
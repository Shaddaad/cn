#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
  int clientsocket, port;
  struct sockaddr_in serveraddr;
  socklen_t len;
  char message[100], reply[100];

  clientsocket = socket(AF_INET, SOCK_DGRAM, 0);
  if (clientsocket < 0)
  {
    perror("Socket creation failed");
    return 1;
  }

  bzero((char *)&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;

  printf("Enter port number: ");
  scanf("%d", &port);

  serveraddr.sin_port = htons(port);
  serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  len = sizeof(serveraddr);

  while (1)
  {
    printf("Client: ");
    scanf(" %[^\n]", message);

    if (sendto(clientsocket, message, strlen(message) + 1, 0,
               (struct sockaddr *)&serveraddr, len) < 0)
    {
      perror("Send failed");
    }

    if (strcmp(message, "exit") == 0)
      break;

    int n = recvfrom(clientsocket, reply, sizeof(reply) - 1, 0,
                     (struct sockaddr *)&serveraddr, &len);

    if (n < 0)
    {
      perror("Receive failed");
      continue;
    }
    reply[n] = '\0';

    if (strcmp(reply, "exit") == 0)
      break;

    printf("Server: %s\n", reply);
  }

  close(clientsocket);

  return 0;
}
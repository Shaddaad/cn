#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

int main()
{
    int serversocket, clientsocket, port;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t len;
    char message[100], reply[100];

    serversocket = socket(AF_INET, SOCK_STREAM, 0);

    bzero((char*)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;

    printf("Enter port number: ");
    scanf("%d",&port);

    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    bind(serversocket,(struct sockaddr*)&serveraddr,sizeof(serveraddr));

    listen(serversocket,5);

    printf("Waiting for client connection...\n");

    len = sizeof(clientaddr);
    clientsocket = accept(serversocket,(struct sockaddr*)&clientaddr,&len);

    printf("Client connected!\n");

    while(1)
    {
        bzero(message,100);

        recv(clientsocket,message,sizeof(message),0);

        if(strcmp(message,"exit")==0)
        {
            printf("Client left the chat.\n");
            break;
        }

        printf("Client: %s\n",message);

        printf("Server: ");
        scanf(" %[^\n]",reply);

        send(clientsocket,reply,sizeof(reply),0);

        if(strcmp(reply,"exit")==0)
        break;
    }

    close(clientsocket);
    close(serversocket);

    return 0;
}
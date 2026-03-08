#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

int main()
{
    int clientsocket, port;
    struct sockaddr_in serveraddr;
    char message[100], reply[100];

    clientsocket = socket(AF_INET, SOCK_STREAM, 0);

    bzero((char*)&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;

    printf("Enter port number: ");
    scanf("%d",&port);

    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    connect(clientsocket,(struct sockaddr*)&serveraddr,sizeof(serveraddr));

    printf("Connected to server\n");

    while(1)
    {
        printf("Client: ");
        scanf(" %[^\n]",message);

        send(clientsocket,message,sizeof(message),0);

        if(strcmp(message,"exit")==0)
        break;

        recv(clientsocket,reply,sizeof(reply),0);

        if(strcmp(reply,"exit")==0)
        break;

        printf("Server: %s\n",reply);
    }

    close(clientsocket);

    return 0;
}
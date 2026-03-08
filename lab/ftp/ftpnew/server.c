#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 9000
#define SIZE 1024

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in server_addr;
    char filename[100], buffer[SIZE];
    FILE *fp;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0)
    {
        printf("Socket creation failed\n");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0)
    {
        printf("Bind failed\n");
        return 1;
    }

    listen(server_fd,5);
    printf("Server waiting for connection...\n");

    new_socket = accept(server_fd,NULL,NULL);
    printf("Client connected\n");

    recv(new_socket, filename, sizeof(filename), 0);

    fp = fopen(filename,"r");

    if(fp == NULL)
    {
        send(new_socket,"File not found",14,0);
        printf("Requested file not found\n");
    }
    else
    {
        printf("Sending file...\n");

        while(fgets(buffer,SIZE,fp)!=NULL)
        {
            send(new_socket,buffer,strlen(buffer),0);
        }

        printf("File sent successfully\n");
    }

    fclose(fp);
    close(new_socket);
    close(server_fd);

    return 0;
}
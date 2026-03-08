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
    int sock;
    struct sockaddr_in serv_addr;
    char filename[100], buffer[SIZE];
    FILE *fp;
    int n;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        printf("Socket creation failed\n");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
    {
        printf("Connection failed\n");
        return 1;
    }

    printf("Connected to server\n");

    printf("Enter filename to download: ");
    scanf("%s",filename);

    send(sock,filename,sizeof(filename),0);

    fp = fopen("received.txt","w");

    while((n = recv(sock,buffer,SIZE-1,0)) > 0)
    {
        buffer[n] = '\0';
        printf("%s",buffer);
        fprintf(fp,"%s",buffer);
    }

    printf("\nFile received successfully\n");

    fclose(fp);
    close(sock);

    return 0;
}
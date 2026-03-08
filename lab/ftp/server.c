#include<stdio.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define SERV_TCP_PORT 5035
#define MAX 60

char buff[4096];
FILE *f1;

int main(int argc, char *argv[])
{
    int sockfd, newsockfd;
    socklen_t clength;

    struct sockaddr_in serv_addr, cli_addr;
    char str[MAX];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(SERV_TCP_PORT);

    printf("\nBinded");

    bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    printf("\nListening...\n");

    listen(sockfd, 5);

    clength = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clength);

    close(sockfd);

    read(newsockfd, str, MAX);

    printf("\nClient message\nFile Name : %s\n", str);

    f1 = fopen(str, "r");

    if(f1 == NULL)
    {
        printf("File not found\n");
        close(newsockfd);
        return 0;
    }

    while(fgets(buff, sizeof(buff), f1) != NULL)
    {
        write(newsockfd, buff, strlen(buff));
    }

    fclose(f1);

    printf("\nFile Transferred\n");

    close(newsockfd);

    return 0;
}

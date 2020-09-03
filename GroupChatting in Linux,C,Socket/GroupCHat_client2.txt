#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/mman.h>
#define MAXLINE 1024              //def buffsize
#define SERV_IP "220.149.128.100"  
#define SERV_PORT 4120
#define MY_IP "220.149.128.102"
#define MY_PORT "4122"
#define OK "o" // def connect success 
#define WP "p" // def passwd error 
#define WI "i" // def id error
int main(int argc, char **argv) {
	struct sockaddr_in clientaddr;
	int client_sockfd;
	int client_len;
	int rcv_byte;
	char buf[MAXLINE];   //buff, buffer size 
	char id[20];         //  id recieve buff
	char pw[20];         // passwd recieve bff
	char last[MAXLINE];  // error or success buff
	char msg[MAXLINE];
	pid_t pid;
	if ((client_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {  // create server socket
		perror("error : ");
		return 1;
	}
	else printf("Client_socket() sockfd is OK...\n");

	clientaddr.sin_family = AF_INET;
	clientaddr.sin_addr.s_addr = inet_addr(SERV_IP); //init server ip
	clientaddr.sin_port = htons(SERV_PORT); // init server port
	memset(&(clientaddr.sin_zero), 0, 8);

	client_len = sizeof(clientaddr);

	if (connect(client_sockfd, (struct sockaddr*)&clientaddr, client_len) == -1) { //connect part
		perror("connect error : ");
		exit(1);
	}
	else printf("Client_connect() is OK...\n\n");

	rcv_byte = recv(client_sockfd, buf, sizeof(buf), 0);
	printf("%s", buf);

	printf("ID: ");
	scanf("%s", id);
	send(client_sockfd, id, strlen(id) + 1, 0);    // send id

	printf("PW: ");
	scanf("%s", pw);
	send(client_sockfd, pw, strlen(pw) + 1, 0);    // send pw

	rcv_byte = recv(client_sockfd, last, sizeof(last), 0);   //recieve errors

	if (strcmp(last, OK) == 0)   //connect success
	{
		printf("Log-in success!! [%s] *^^*\n", id);
		printf("-------Chatting Room------\n");
	}
	else if (strcmp(last, WP) == 0)   //wrong passwd 
	{
		printf("Log-in fail: Incorrect password...\n");
		close(client_sockfd);
		exit(1);
	}
	else if (strcmp(last, WI) == 0)   //worng id
	{
		printf("Log-in fail: ID does not exist...\n");
		close(client_sockfd);
		exit(1);
	}
	send(client_sockfd, MY_IP, strlen(MY_IP) + 1, 0);
	send(client_sockfd, MY_PORT, strlen(MY_PORT) + 1, 0);
	pid = fork();
	// buf : 1024 msg : 1024
	if (pid == 0)
	{
		while (1)
		{
			fgets(buf,MAXLINE,stdin);
			buf[strlen(buf)-1] = '\0';
			send(client_sockfd, buf, strlen(buf) + 1, 0);
			send(client_sockfd, id, strlen(id) + 1, 0);
			if (strcmp(buf, "/exit") == 0)
			{
				close(client_sockfd);
				exit(1);
			}
		}
	}
	else if (pid > 0)
	{
		while (1)
		{
			rcv_byte = recv(client_sockfd, msg, sizeof(msg), 0);
			if (strcmp(msg, "/exit") == 0)
			{
				close(client_sockfd);
				exit(1);
			}
			else
				printf("%s\n", msg);
		}
	}
	return 0;
}

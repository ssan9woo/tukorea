#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/select.h>
#define BUFF_SIZE 1024			//buff size
#define SEV_IP "220.149.128.100" // 
#define SERV_PORT 4180
#define INIT_MSG "======================\nHello! I`m P2p File Sharing Server..\n Please, Log-In!\n======================\n" // hello msg
#define User1_ID "user1" 
#define User1_PW "passwd1"
#define User2_ID "user2"
#define User2_PW "passwd2"
#define User3_ID "user3"
#define User3_PW "passwd3"
#define Wrong_PW "Log-in fail: Incorrect password\n\n"
#define FALSE 0
#define TRUE 1
int* sockets=NULL;
int* cnt_socket = NULL;
int* clients_port = NULL;
char** clients_ID = NULL;
char** clients_IP = NULL;
char** clients_PORT = NULL;
void mystop(int signo)
{
	munmap(cnt_socket,sizeof *cnt_socket);
	munmap(sockets,sizeof (*sockets)*5);
	for(int i=0; i<5;i++)
		munmap(*(clients_ID+i),sizeof *(*(clients_ID+i)) * 20);
	for(int i=0; i<5;i++)
		munmap(*(clients_IP+i),sizeof *(*(clients_IP+i)) * 20);
	for(int i=0; i<5;i++)
		munmap(*(clients_PORT+i),sizeof *(*(clients_PORT +i)) *5);
	exit(1);
}
int main(void)
{
	int p2p_flag = FALSE;
	int flag = FALSE;
	int server_socket;  //server socket
	int client_socket;  //client socket 
	int client_addr_size; // addr size
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int rcv_byte;
	char buff[BUFF_SIZE];
	char socket_pid[BUFF_SIZE];
	char p2p_ip_buff[BUFF_SIZE];
	char p2p_port_buff[BUFF_SIZE];
	char msg_file[BUFF_SIZE];
	char id[20];
	char pw[20];
	char target[20];
	char yorn[BUFF_SIZE];
	char msg[BUFF_SIZE];	
	int option;

	fd_set reads,cpy_reads;
	struct timeval timeout;
	int fd_max,fd_num;
	int i;
	/*------------------------------------------Shared Memory Mapping Part-----------------------------------------------*/
	sockets = mmap(NULL,sizeof (*sockets)*5, PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
	cnt_socket = mmap(NULL,sizeof *cnt_socket,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
	clients_ID = mmap(NULL,sizeof (*clients_ID)*5,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
	for(int i=0; i<5;i++)
		*(clients_ID+i) = mmap(NULL,sizeof (*clients_ID)*20,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
	clients_IP = mmap(NULL,sizeof (*clients_IP)*5,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
	for(int i=0; i<5;i++)
		*(clients_IP+i) = mmap(NULL,sizeof (*clients_IP)*20,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
	clients_PORT = mmap(NULL,sizeof (*clients_PORT)*5,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
	for(int i=0; i<5;i++)
		*(clients_PORT+i) = mmap(NULL,sizeof (*clients_PORT)*20,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);

	signal(SIGINT,(void*)mystop);

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	option = 1;
	setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,(char*)&option,sizeof(option));
	if(-1 == server_socket) // create a server socket
	{
		printf( "server socket fail\n");
		exit(1);
	}
	else printf("Server-socket() sockfd is OK...\n");

	server_addr.sin_family     = AF_INET;
	server_addr.sin_port       = htons(SERV_PORT);
	server_addr.sin_addr.s_addr= htonl(INADDR_ANY); //32bit IPv4 address

	memset(&(server_addr.sin_zero), 0, 8);
	if(setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&option,sizeof(option)) <0 ) // set a socket option
	{
		perror("setsockopt");
		close(server_socket);
		return -1;
	}
	if(-1 == bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr))) //Associate a socket with a port and address
	{
		perror("bind() fail!!");
		exit(1);
	}
	else printf("Server-bind() is OK...\n"); 

	if(-1 == listen(server_socket, 10)){ // listen for connection
		perror( "listen fail \n");
		exit(1);
	}
	else printf("Server-listen() is OK...\n");

	client_addr_size  = sizeof(client_addr);
	FD_ZERO(&reads);
	FD_SET(server_socket,&reads);
	fd_max = server_socket;
	while(1)
	{
		cpy_reads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 50000;
		if((fd_num = select(fd_max+1,&cpy_reads,0,0,&timeout)) == -1)
		{
			perror("select() error");
			break;
		}
		if(fd_num == 0) continue;

		for(i=0; i <fd_max+1;i++)
		{
			if(FD_ISSET(i,&cpy_reads))
			{
				if(server_socket == i)
				{
					client_socket = accept( server_socket, (struct sockaddr*)&client_addr, &client_addr_size);

					if(client_socket == -1) perror("accept() error!");
					FD_SET(client_socket,&reads);
					if(fd_max < client_socket)
						fd_max = client_socket;
					send(client_socket,INIT_MSG,strlen(INIT_MSG)+1,0); // send hello msg
					rcv_byte = recv(client_socket,id,sizeof(id),0); // recv_id
					rcv_byte = recv(client_socket,pw,sizeof(pw),0); //recv_pw
					printf("========================\n");	//print information
					printf("User Information\n");
					printf("ID : %s, PW : %s\n",id,pw);
					printf("========================\n");

					/*------------------------------Compare ID,Compare PW-------------------------------*/
					if((strcmp(id,User1_ID)==0) || (strcmp(id, User2_ID) == 0) || (strcmp(id, User3_ID) == 0))
					{
						if((strcmp(pw,User1_PW)==0)&& (strcmp(id, User1_ID) == 0))
						{
							printf("Log-in success!! [%s] *^^*\n",id);
							printf("-------Chatting Room-------\n");
							send(client_socket,"o",strlen("o")+1,0);
							flag = TRUE;
						}
						else if ((strcmp(pw, User2_PW) == 0)&&(strcmp(id, User2_ID) == 0))
						{
							printf("Log-in success!! [%s] *^^*\n", id);
							printf("-------Chatting Room-------\n");
							send(client_socket, "o", strlen("o") + 1, 0);
							flag =TRUE;
						}
						else if ((strcmp(pw, User3_PW) == 0)&&(strcmp(id, User3_ID) == 0))
						{
							printf("Log-in success!! [%s] *^^*\n", id);
							printf("-------Chatting Room-------\n");
							send(client_socket, "o", strlen("o") + 1, 0);
							flag = TRUE;
						}
						else
						{
							printf("%s\n",Wrong_PW);
							send(client_socket,"p",strlen("p")+1,0);
							FD_CLR(client_socket,&reads);
							fd_max--;
							close(client_socket);
						}
					}
					else
					{
						printf("Log-in fail: ID Does not exist..\n\n");
						send(client_socket,"i",strlen("i")+1,0);
						FD_CLR(client_socket,&reads);
						fd_max--;
						close(client_socket);
					}
					/*--------------------------------------------GET <ID>,<IP>,<PORT>--------------------------------------------*/
					if(flag == TRUE)
					{
						*(sockets + *cnt_socket )  = client_socket;
						recv(client_socket,buff,sizeof(buff),0);
						strcpy(*(clients_IP+*cnt_socket),buff);	
						recv(client_socket,buff,sizeof(buff),0);
						strcpy(*(clients_PORT+*cnt_socket),buff);
						strcpy(*(clients_ID+*cnt_socket),id);
					//	printf("[%s] : <%s> , <%s> \n",*(clients_ID+*cnt_socket),*(clients_PORT+*cnt_socket),*(clients_IP+*cnt_socket));
						flag == FALSE;
						*cnt_socket+=1;
					}
				}
				else
				{
					rcv_byte = recv(i,msg,sizeof(msg),0);
					strcpy(id,*(clients_ID+(i-4)));
					if(strcmp(msg,"[FILE]") == 0 )
					{
						rcv_byte = recv(i,target,sizeof(target),0);
						int myself;
						for(int j=0; j<*cnt_socket;j++)
						{
							if(strcmp(id,*(clients_ID+j))== 0)
								myself = j;
						}
						for(int j=0; j<*cnt_socket; j++)
						{
							if(strcmp(target,*(clients_ID+j)) == 0)
							{
								strcpy(msg_file,"[FILE]");
								send(*(sockets+j),"Do you accept P2P MODE?[/Y or /N]",strlen("Do you accept P2P MODE?[/Y or /N]")+1,0);
								rcv_byte =  recv(*(sockets+j),yorn,sizeof(yorn),0);
								if((strcmp(yorn,"/N")==0) || (strcmp(yorn,"/n")==0))
								{
									send(i,yorn,strlen(yorn)+1,0);
								}
								else if((strcmp(yorn,"/Y")==0) || strcmp(yorn,"/y")==0)
								{
									send(i,yorn,strlen(yorn)+1,0);
									send(*(sockets+j),msg_file,strlen(msg_file)+1,0);
									sleep(1);
									send(*(sockets+j),*(clients_PORT+myself),strlen(*(clients_PORT+myself))+1,0);
									send(*(sockets+j),*(clients_IP+myself),strlen(*(clients_IP+myself))+1,0);
								}
								else
								{
									send(i,yorn,strlen(yorn)+1,0);
								}
							}
						}
						p2p_flag = TRUE;
					}
					else if(strcmp(msg,"/exit") == 0)
					{
						for(int j=0; j<5;j++)
						{
							if(*(sockets+j) == i)
							{
								send(*(sockets+j),msg,strlen(msg)+1,0);
								*(sockets+j) = 0;
								*cnt_socket--;
							}
						}
						FD_CLR(i,&reads);
						close(i);
						sprintf(buff,"[%s] is left the chat...",id);
					}
					else
					{
						sprintf(buff,"[%s] : %s",id,msg);
					}
					if(p2p_flag == FALSE)
					{
						printf("%s\n",buff);
						for(int j=0; j<5;j++)
						{
							if(i != *(sockets+j))
							{
								send(*(sockets+j),buff,strlen(buff)+1,0);	
							}
						}
					}
					p2p_flag = FALSE;
				}
			}
		}
	}

	for(int i=0; i<5;i++)
		munmap(*(clients_ID+i),sizeof *(*(clients_ID+i)) * 20);
	for(int i=0; i<5;i++)
		munmap(*(clients_IP+i),sizeof *(*(clients_IP+i)) * 20);
	for(int i=0; i<5;i++)
		munmap(*(clients_PORT+i),sizeof *(*(clients_PORT +i)) *5);
	munmap(cnt_socket,sizeof *cnt_socket);
	munmap(sockets,sizeof (*sockets)* 5);
}

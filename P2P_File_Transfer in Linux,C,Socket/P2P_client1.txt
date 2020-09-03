#include <stdio.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <dirent.h>
#define MAXLINE 1024              //def buffsize
#define SERV_IP "220.149.128.100"  
#define SERV_PORT 4180
#define MY_IP "220.149.128.101"
#define MY_PORT "4181"
#define OK "o" // def connect success 
#define WP "p" // def passwd error 
#define WI "i" // def id error
int *mutex = NULL;
char* p2p_ip = NULL;
char* p2p_port = NULL;
void p2p_server();
void p2p_client();
void mystop(int signo)
{
	munmap(p2p_ip,sizeof (*p2p_ip)*20);
	munmap(p2p_port,sizeof (*p2p_port)*20);
	munmap(mutex,sizeof *mutex);
	exit(1);
}
int main(int argc, char **argv)
{
	struct sockaddr_in clientaddr;
	int client_sockfd;
	int client_len; 
	int rcv_byte;
	int pipe_[2];
	char pipe_buff[MAXLINE];
	char id[20];         //  id recieve buff
	char pw[20];         // passwd recieve bff
	char buf[MAXLINE];   //buff, buffer size 
	char last[MAXLINE];  // error or success buff
	char msg[MAXLINE];
	char p2p_id[20];
	char p2p[10];
	char p2p_ip_buff[MAXLINE];
	char p2p_port_buff[MAXLINE];
	pid_t pid;

	mutex = mmap(NULL,sizeof *mutex,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);

	p2p_ip = mmap(NULL,sizeof (*p2p_ip)*20,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
	p2p_port = mmap(NULL,sizeof (*p2p_port)*20,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
	*mutex = 1;
	signal(SIGINT,(void*)mystop);
	if(-1 == pipe(pipe_))
	{
		perror("create PIPE() failed..");
		exit(1);
	}
	if((client_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {  // create server socket
		perror("error : ");
		return 1;
	}
	else printf("Client_socket() sockfd is OK...\n");

	clientaddr.sin_family = AF_INET;
	clientaddr.sin_addr.s_addr = inet_addr(SERV_IP); //init server ip
	clientaddr.sin_port = htons(SERV_PORT); // init server port
	memset(&(clientaddr.sin_zero),0,8);

	client_len = sizeof(clientaddr);

	if(connect(client_sockfd, (struct sockaddr*)&clientaddr, client_len) == -1) { //connect part
		perror("connect error : ");
		exit(1);
	}
	else printf("Client_connect() is OK...\n\n");

	rcv_byte = recv(client_sockfd,buf,sizeof(buf),0);
	printf("%s",buf);

	printf("ID: ");
	scanf("%s", id);
	send(client_sockfd,id,strlen(id)+1,0);	 // send id

	printf("PW: ");
	scanf("%s", pw);
	send(client_sockfd, pw,strlen(pw)+1,0);	 // send pw

	rcv_byte = recv(client_sockfd,last,sizeof(last),0);	//recieve errors

	if(strcmp(last,OK) == 0)   //connect success
	{
		printf("Log-in success!! [%s] *^^*\n",id);
		printf("-------Chatting Room------\n");
	}
	else if(strcmp(last,WP) == 0)	//wrong passwd 
	{
		printf("Log-in fail: Incorrect password...\n");
		close(client_sockfd);
		exit(1);
	}
	else if(strcmp(last,WI) == 0 )	//worng id
	{
		printf("Log-in fail: ID does not exist...\n");
		close(client_sockfd);
		exit(1);
	}
	send(client_sockfd,MY_IP,strlen(MY_IP)+1,0);
	send(client_sockfd,MY_PORT,strlen(MY_PORT)+1,0);
	pid = fork();
	// buf : 1024 msg : 1024
	if(pid == 0)
	{
		close(pipe_[1]);
		while(1)
		{
			while(*mutex<=0);
			//scanf("%s",buf);
			fgets(buf,MAXLINE,stdin);
			buf[strlen(buf)-1] = '\0';
			send(client_sockfd,buf,strlen(buf)+1,0);
			if(strcmp(buf,"[FILE]") == 0)
			{
				printf("Please Enter ID : ");
				scanf("%s",p2p);
				send(client_sockfd,p2p,strlen(p2p)+1,0);
				read(pipe_[0],pipe_buff,MAXLINE);
				if((strcmp(pipe_buff,"/N")==0) || (strcmp(pipe_buff,"/n")==0))
				{
					*mutex = 1;
				}
				else if((strcmp(pipe_buff,"/Y")==0) || (strcmp(pipe_buff,"/y")==0))
				{
					p2p_server();
					*mutex = 1;
				}
			}
			else if(strcmp(buf,"/exit")==0)
			{
				close(client_sockfd);
				exit(1);
			}	
			else if((strcmp(buf,"/Y")==0) || (strcmp(buf,"/y")==0))
			{
				*mutex = 0;
			}
		}
	}
	else if(pid > 0)
	{
		close(pipe_[0]);
		while(1)
		{
			while(*mutex<=0);
			rcv_byte = recv(client_sockfd,msg,sizeof(msg),0);
			if(strcmp(msg,"[FILE]") == 0)
			{
				printf("%s\n",msg);
				rcv_byte = recv(client_sockfd,p2p_port_buff,sizeof(p2p_port_buff),0);
				rcv_byte = recv(client_sockfd,p2p_ip_buff,sizeof(p2p_ip_buff),0);
				printf("%s\n%s\n",p2p_port_buff,p2p_ip_buff);
				strcpy(p2p_port,p2p_port_buff);
				strcpy(p2p_ip,p2p_ip_buff);
				p2p_client();
				*mutex = 1;
			}
			else if(strcmp(msg,"/exit")==0)
			{
				close(client_sockfd);
				exit(1);
			}
			else if((strcmp(msg,"/Y")==0) || (strcmp(msg,"/y")==0))
			{
				strcpy(pipe_buff,msg);
				write(pipe_[1],pipe_buff,strlen(pipe_buff));
			}
			else if((strcmp(msg,"/N")==0) || (strcmp(msg,"/n")==0))
			{
				strcpy(pipe_buff,msg);
				write(pipe_[1],pipe_buff,strlen(pipe_buff));
			}
			else
				printf("%s\n",msg);
		}
	}
	else;
	munmap(p2p_ip,sizeof (*p2p_ip)*20);
	munmap(p2p_port,sizeof (*p2p_port)*20);
	munmap(mutex,sizeof *mutex);
	return 0;
}
void p2p_server()
{
	DIR *dir_ptr = NULL;
	struct dirent *file = NULL;
	char my_dirname[MAXLINE];
	int sockfd, new_fd;
	struct stat buf;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	unsigned int sin_size;
	char p2p_buff[MAXLINE];
	char p2p_msg[MAXLINE];
	char txt_lists[10][MAXLINE];
	char ch_num[5];
	char filename[MAXLINE];
	char p2p_path[256];
	int get_list_num;
	int p2p_rcv_byte;
	int val = 1;
	int err;
	pid_t pid2;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		perror("Server-socket() error lol!");
		return;
	}
	else printf("P2P-Server-socket() sockfd is OK...\n");

	my_addr.sin_family = AF_INET;
	strcpy(p2p_buff,MY_PORT);
	int my_port_ = atoi(p2p_buff);
	my_addr.sin_port = htons(my_port_);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(my_addr.sin_zero), 0, 0);

	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,(char*)&val,sizeof(val)) <0)
	{
		perror("P2P-setsockopt");
		close(sockfd);
		return;
	}

	if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("P2P-Server-bind() error lol!");
		return;
	}
	else printf("P2P-Server-bind() is OK...\n");

	if(listen(sockfd, 5) == -1)
	{
		perror("P2P-listen() error lol!");
		return;
	}

	sin_size = sizeof(struct sockaddr_in);
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr,&sin_size);
	if(new_fd == -1) perror("P2P-accept() error lol!");
	else {printf("P2P-accept() is OK...\n\n");}

	printf("------------------<P2P SERVER>-----------------\n");	
	strcpy(my_dirname,"/home/st2014146034/tcp/p2pclient");
	if((dir_ptr = opendir(my_dirname)) ==NULL)
	{
		printf("<%s> can not read information...\n",my_dirname);
		return;
	}
	int i=0;
	while((file = readdir(dir_ptr)) != NULL)
	{
		sprintf(filename,"%s/%s",my_dirname,file->d_name);
		err = lstat(filename,&buf);

		if(err == -1) perror("error");
		else
		{
			if(S_ISREG(buf.st_mode))
			{
				strcpy(txt_lists[i],file->d_name);
				i++;
			}
		}
	}
	while(1)
	{
		send(new_fd,"[FILE LIST]",strlen("[FILE LIST]")+1,0);
		for(int j=0; j<i;j++)
		{
			sleep(1);
			sprintf(p2p_msg,"%d. %s",j+1,txt_lists[j]);
			send(new_fd,p2p_msg,strlen(p2p_msg)+1,0);
		}
		recv(new_fd,p2p_buff,sizeof(p2p_buff),0);

		if(strcmp(p2p_buff,"/q") == 0)
		{

			send(new_fd,"/q",strlen("/q")+1,0);
			break;
		}
		get_list_num = atoi(p2p_buff);
		send(new_fd,"file",strlen("file")+1,0);
		sleep(1);
		send(new_fd,txt_lists[get_list_num-1],strlen(txt_lists[get_list_num-1])+1,0);
		sprintf(p2p_path,"%s/%s", my_dirname,txt_lists[get_list_num-1]);
		char sdbuf[512];
		FILE *fs = fopen(p2p_path,"r");
		if(fs == NULL)
		{
			printf("P2Pfile path error\n");
			break;
		}
		bzero(sdbuf, 512);
		int fs_block_sz;
		sleep(1);
		while((fs_block_sz = fread(sdbuf, sizeof(char),512,fs))>0)
		{
			if(send(new_fd,sdbuf,fs_block_sz,0)<0)
			{
				printf("P2P can not send file\n");
				break;
			}
			bzero(sdbuf,512);
			sleep(1);
		}
		printf("Success P2P file tranfer!!\n");
	}
	printf("-------------------<END  P2P>------------------\n");
	close(new_fd);
	close(sockfd);
	closedir(dir_ptr);
	return;
}
void p2p_client()
{
	int count=0;
	int sockfd;
	struct sockaddr_in dest_addr;
	char p2p_buff[MAXLINE];
	char p2p_msg[MAXLINE];
	char p2p_file_list[10][MAXLINE];
	char p2p_path[512];
	char revbuf[512];
	char filename[256];
	int p2p_rcv_byte;
	int p2p_server_port = atoi(p2p_port);
	char path[256] = "/home/st2014146034/tcp/getfile";
	pid_t pid2;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1)
	{
		perror("P2P-Client-socket() error lol!");
		return;
	}
	else printf("P2P-Client-socket() sockfd is OK...\n");

	dest_addr.sin_family = AF_INET;

	dest_addr.sin_port = htons(p2p_server_port);
	dest_addr.sin_addr.s_addr = inet_addr(p2p_ip);

	memset(&(dest_addr.sin_zero),0,0);

	if(connect(sockfd, (struct sockaddr *)&dest_addr,sizeof (struct sockaddr)) == -1)
	{
		perror("P2P-Client-connect() error lol!");
		return;
	}
	else printf("P2P-Client-connect() is OK...\n\n");
	printf("------------------<P2P CLIENT>-----------------\n");

	pid2 = fork();

	if(pid2 == 0)
	{
		while(1)
		{
			scanf("%s",p2p_buff);
			send(sockfd,p2p_buff,strlen(p2p_buff)+1,0);
			if(strcmp(p2p_buff,"/q") == 0)
			{
				close(sockfd);
				exit(1);
			}
		}
	}
	else if(pid2 > 0)
	{
		while(1)
		{
			int i=0;
			p2p_rcv_byte = recv(sockfd,p2p_msg,sizeof(p2p_msg),0);
			if(strcmp(p2p_msg,"/q") == 0)
			{
				break;
			}
			else if(strcmp(p2p_msg,"file") == 0)
			{
				count =0;
				p2p_rcv_byte = recv(sockfd,filename,sizeof(filename),0);
				sprintf(p2p_path,"%s/%s",path,filename);
				FILE *fr = fopen(p2p_path,"a");

				if(fr == NULL)
					printf("P2P cannot be opened\n");
				else
				{
					bzero(revbuf, 512);
					int fr_block_sz = 0;
					while((fr_block_sz = recv(sockfd,revbuf, 512,0))>0)
					{
						int write_sz = fwrite(revbuf, sizeof(char),fr_block_sz,fr);
						if(write_sz < fr_block_sz)
						{
							perror("P2P file write failed");
						}
						bzero(revbuf,512);
						if(fr_block_sz==0 || fr_block_sz != 512)
						{
							break;
						}
					}
					if(fr_block_sz < 0)
					{
						if(errno == EAGAIN)
						{
							printf("P2P recv() timed out\n");
						}
						else
						{
							printf("P2P recv() faild due to error");
						}
					}
					printf("P2P Success received file!\n");
					fclose(fr);
				}
			}
			else
			{
				printf("%s\n",p2p_msg);
			}
		}//end while
	}//end parents_process
	else
		perror("P2P fork() error!");

	printf("-------------------<END P2P>-------------------\n");
	close(sockfd);
	return;
}

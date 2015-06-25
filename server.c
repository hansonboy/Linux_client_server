#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define DEFAULT_PORT 8000
#define MAXLINE 4096
 int main(int argc, char const *argv[])
{
	int socket_fd,connect_fd,listen_fd;
	struct sockaddr_in servaddr;
	char buff[MAXLINE];
	int n;
	//init socket
	if ((socket_fd = socket(AF_INET,SOCK_STREAM,0))== -1)
	{		
		printf("create socket error:%s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	//init
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//ip address set as INADDR_ANY to make system auto get native IP
	servaddr.sin_port = htons(DEFAULT_PORT);
	if(bind(socket_fd,(struct sockaddr*)&servaddr,sizeof(servaddr))== -1){
		printf("bind socket error:%s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	if (listen(socket_fd,10) == -1)
	{	
		printf("listen socket error:%s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	printf("==========waiting for client's request =========\n");
	while(1){
		if((connect_fd = accept(socket_fd,(struct sockaddr*)NULL,NULL))==-1){

			printf("accept socet error: %s(errno:%d)",strerror(errno),errno);
			continue;
		}
		n = recv(connect_fd,buff,MAXLINE,0);

		if(!fork()){
			if(send(connect_fd,"Hello,you are connected!\n",27,0) == -1){
				perror("send error");
				close(connect_fd);
				exit(0);
			}
			buff[n] = '\0';
			printf("recv msg from client:%s\n",buff);
			close(connect_fd);
		}
	}
	close(socket_fd);
	return 0;
}

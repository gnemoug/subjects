#include "main_sub_windows.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define BUFFER_SIZE 1024

int client( char buffer[1024] )
{
	int sockfd;
	struct sockaddr_in  server_addr;
	struct hostent *host;
	int portnumber,nbytes;
	int to_fd;
	int nbyte;
	int bytes_write;
	char bbuffer[BUFFER_SIZE] = "./wh_hit/";
	char cbuffer[BUFFER_SIZE] = "./wh_hit/";
	char *ptr;	
	char abuffer[BUFFER_SIZE]="";
	struct timeval tpstart,tpend;
	float timeuse;

	printf("%s\n",buffer);
	
	if((host = gethostbyname("172.29.142.40"))==NULL)	//得到主机名字和地址信息
	{
		fprintf(stderr,"Gethostname error\n");
		exit(1);
	}
	if((portnumber=atoi( "8888" ))<0)	//得到端口号
	{
		exit(1);
	}
	
	/* 客户程序开始建立 sockfd 描述符 */
	if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		fprintf(stderr,"Socket Error:%s\a\n",strerror(errno));
		exit(1);
	}
	/* 客户程序填充服务端的资料 */
	bzero(&server_addr,sizeof(server_addr));	//将此套接字清零
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portnumber);	//填写端口，将本机上的short类型的端口号改变为网络上的short类型端口号
	server_addr.sin_addr = *((struct in_addr *)host -> h_addr);	//填写要向发送请求的IP地址
	/* 客户程序发起连接请求 */
	if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
	{
		fprintf(stderr,"Connect Error:%s\a\n",strerror(errno));
		exit(1);
	}
	printf("%s\n",buffer);	

	if((nbytes = write( sockfd,buffer,strlen(buffer)))==-1)	//若写成功的话，返回写的字节数
	{
		fprintf(stderr,"write Error:%s\n",strerror(errno));
		exit(1);
	}
	memset(buffer,'\0',1024);
	if(( nbytes = read( sockfd,buffer,1024)) == -1 )	//若读取成功的话，返回读的字节数
	{
		fprintf(stderr,"Read Error:%s\n",strerror(errno));
		exit(1);
	}	
	buffer[nbytes]='\0';
	
	strcat(bbuffer,buffer);
	printf("%s\n",bbuffer);
	if((to_fd = open(bbuffer,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR))==-1)		//此处或既可间或，又可不间或
	{
		fprintf(stderr,"Open %s Error:%s\n",bbuffer,strerror(errno));
		exit(1);
	}
	sleep(2);
	gettimeofday(&tpstart,NULL);
	while( nbyte = read( sockfd,abuffer,1024) )	//若读取成功的话，返回读的字节数
	{	
		if( nbyte== -1 )
		{
			fprintf(stderr,"Read Error:%s\n",strerror(errno));
			exit(1);
		}
		else if( nbyte > 0 )
		{
			ptr = abuffer;
			while(bytes_write = write(to_fd,abuffer,nbyte))
			{
				/* 一个致命错误发生了*/
				if((bytes_write==-1)&&(errno==EINTR))break;
				/* 写完了所有读的字节 */
				else if(bytes_write==nbyte) break;
				/* 只写了一部分,继续写 */
				//write 从 buffer 中写 count 字节到文件 fd 中,成功时返回实际所写的字节数
				else if(bytes_write>0)
				{	
					ptr+=bytes_write;
					nbyte-=bytes_write;
				}
			}
				/* 写的时候发生的致命错误 */
			if(bytes_write==-1)break;
			memset(abuffer,'\0',1024);
		}
		gettimeofday(&tpend,NULL);
		timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;
		timeuse/=1000000;
		if( timeuse > 1.0 )
			break;
	}
	close(to_fd);
	sleep(2);
	memset(buffer,'\0',1024);
	memset(bbuffer,'\0',1024);
	strcpy( bbuffer,  cbuffer );
	if(( nbytes = read( sockfd,buffer,1024)) == -1 )	//若读取成功的话，返回读的字节数
	{
		fprintf(stderr,"Read Error:%s\n",strerror(errno));
		exit(1);
	}	
	buffer[nbytes]='\0';

	strcat(bbuffer,buffer);
	while( 1 )
	{
		if((to_fd = open(bbuffer,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR))==-1)		//此处或既可间或，又可不间或
		{
			return 0;
		}
		sleep(2);
		gettimeofday(&tpstart,NULL);
		while( nbyte = read( sockfd,abuffer,1024) )	//若读取成功的话，返回读的字节数
		{	
			if( nbyte== -1 )
			{
				fprintf(stderr,"Read Error:%s\n",strerror(errno));
				exit(1);
			}
			else if( nbyte > 0 )
			{
				ptr = abuffer;
				while(bytes_write = write(to_fd,abuffer,nbyte))
				{
					/* 一个致命错误发生了*/
					if((bytes_write==-1)&&(errno==EINTR))break;
					/* 写完了所有读的字节 */
					else if(bytes_write==nbyte) break;
					/* 只写了一部分,继续写 */
					//write 从 buffer 中写 count 字节到文件 fd 中,成功时返回实际所写的字节数
					else if(bytes_write>0)
					{	
						ptr+=bytes_write;
						nbyte-=bytes_write;
					}
				}
				/* 写的时候发生的致命错误 */
				if(bytes_write==-1)break;
				memset(abuffer,'\0',1024);
			}
			gettimeofday(&tpend,NULL);
			timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;
			timeuse/=1000000;
			if( timeuse > 2.0 )
				break;
		}
		close(to_fd);
		memset(buffer,'\0',1024);
		memset(bbuffer,'\0',1024);
		strcpy( bbuffer,  cbuffer );
		if(( nbytes = read( sockfd,buffer,1024)) == -1 )	//若读取成功的话，返回读的字节数
		{
			fprintf(stderr,"Read Error:%s\n",strerror(errno));
			exit(1);
		}	
		buffer[nbytes]='\0';
		if(!strcmp(buffer,"gone") )
		{
			printf("gone\n");
			close(sockfd);
			exit(0);
		}
		strcat(bbuffer,buffer);
	}
}




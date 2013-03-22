#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <dirent.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define BUFFER_SIZE 1024
#define BUFFER 512

int new_fd;

void sendfile( void *arg)
{
	int from_fd ;
	int bytes_read;
	char abuffer[BUFFER_SIZE] = ""; 
	int *a = (int *)arg;

	from_fd  = *(int *)arg;
	bytes_read = *a;
	while(bytes_read = read(from_fd,abuffer,BUFFER_SIZE))
	{
		/* 一个致命的错误发生了 */
		if((bytes_read==-1)&&(errno==EINTR)) break;
		else if(bytes_read>0)
		{
			if(write(new_fd,abuffer,strlen(abuffer))==-1)
			{
				fprintf(stderr,"Write Error:%s\n",strerror(errno));
				exit(1);
			}	
		}
		memset(abuffer,'\0',1024);	
	}
	printf("hello\n");
	//sleep(5);
	close(from_fd);
	//pthread_exit(NULL);//返回指针
}

int main(int argc, char *argv[])
{
	int sockfd, *file;
	char bbuffer[BUFFER] = "./";
	char abbuffer[BUFFER] = "";
	int bytes, num, i, j; 
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int sin_size,portnumber;
	int byte_copy;
	int *byte_copy_p = NULL;
	char buffer[1024];
	struct dirent **namelist;
	struct stat filestat;
	char filename[BUFFER];
	pthread_t *thread;

	if(argc!=2)
	{
		fprintf(stderr,"Usage:%s portnumber\a\n",argv[0]);
		exit(1);
	}
	if( (portnumber=atoi(argv[1]))<0 )	//判断端口号是否有效
	{
		fprintf(stderr,"Usage:%s portnumber\a\n",argv[0]);
		exit(1);
	}
	/*服务器端开始建立 socket 描述符 */
	if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)	//要实现文件信息的传送，需要利用套接字来实现
	{
		fprintf(stderr,"Socket error:%s\n\a",strerror(errno));
		exit(1);
	}
	/* 服务器端填充 sockaddr 结构 */
	bzero(&server_addr,sizeof(struct sockaddr_in));	//将此套接字清零
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);		//填写IP地址限制，表示可以与任意IP建立联系
	server_addr.sin_port = htons(portnumber);		//填写端口，将本机上的short类型的端口号改变为网络上的short类型端口号
	/* 捆绑 sockfd 描述符 */
	//设定绑定套接字
	if(bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
	{
		fprintf(stderr,"Bind error:%s\n\a",strerror(errno));
		exit(1);
	}
	/* 监听 sockfd 描述符 */
	if(listen(sockfd,5)==-1)
	{
		fprintf(stderr,"Listen error:%s\n\a",strerror(errno));
		exit(1);
	}




	while(1)
	{
		/* 服务器阻塞,直到客户程序建立连接 */
		sin_size = sizeof(struct sockaddr_in);
		if((new_fd = accept(sockfd,(struct sockaddr *)(&client_addr),&sin_size))==-1)
		{
			fprintf(stderr,"Accept error:%s\n\a",strerror(errno));
			exit(1);
		}
		

		if(( bytes = read( new_fd,buffer,1024)) == -1 )	//若读取成功的话，返回读的字节数
		{
			fprintf(stderr,"Read Error:%s\n",strerror(errno));
			exit(1);
		}
		buffer[bytes]='\0';
		printf("I have received:%s\n",buffer);

		//sendfile(new_fd,buffer);
		strcat( bbuffer, buffer);
		/* 得到当前路径下面所有的文件(包含目录)的个数 */
		if( (num = scandir( bbuffer,&namelist,0,alphasort)) < 0 )
		{
			fprintf(stderr,"Get File Num Error:%s\n\a",strerror(errno));
			exit(1);
		}
		printf("%d\n", num);
		strcat( bbuffer, "/");
		strcpy( abbuffer , bbuffer );
		/*给线程分配空间,其实没有必要这么多的 */
		if(((thread = ( pthread_t *) malloc(sizeof(pthread_t) * num)) == NULL)||((file = (int *)malloc(sizeof(int)*num))==NULL))
		{
			fprintf(stderr,"Out Of Memory!\n\a"); 
			exit(1);
		}
		for( i = 0,j = 0; i < num; i++ )
		{
			memset(filename,'\0',BUFFER);
			strcpy( bbuffer , abbuffer );
			printf("%s\n",bbuffer);
			strcpy(filename,namelist[i] -> d_name);
			printf("%s\n",filename);
			strcat( bbuffer , filename );
			printf("%s\n",bbuffer);

			if(stat(bbuffer,&filestat)==-1)	//获取文件属性
			{
				fprintf(stderr,"Get File Information:%s\n\a",strerror(errno));
				exit(1);
			}
			/* 我们忽略目录*/
			if(!S_ISREG(filestat.st_mode))
				continue;
			printf("world!\n");
			if((bytes = write( new_fd,filename ,strlen(filename) ))==-1)	//若写成功的话，返回写的字节数
			{
				fprintf(stderr,"write Error:%s\n",strerror(errno));
				exit(1);
			}
			sleep(4);


			if( ( file[j] = open(bbuffer,O_RDONLY)) < 0 )
			{
				fprintf(stderr,"Open %s Error:%s\n\a",bbuffer,strerror(errno));
				continue;
			}

			sendfile( ( void *)&file[j] );
			sleep(4);
			j++;
			if( j == ( num-2 ) )
			{
				printf("1111111111111111111111111111world!\n");
				if((bytes = write( new_fd,"gone" ,strlen("gone") ))==-1)	//若写成功的话，返回写的字节数
				{
					fprintf(stderr,"write Error:%s\n",strerror(errno));
					exit(1);
				}
				sleep(4);
			}
		}

		printf("实际标准文件个数j为:%d\n",j);

		free(file);
		/* 这个通讯已经结束 */
		close(new_fd);
		/* 循环下一个 */
	}
	close(sockfd);

	exit(0);
}



/*
	HEE CHEE KEONG  1122700902
	GAN CHERN HOANG 1112703704
*/
#include "inet.h"

int main(int argc, char *argv[])
{
	int sockfd, new_sockfd, clilen;
	struct sockaddr_in serv_addr, cli_addr;
	char buffer[1025];
	time_t timeval;
	int n=0;
	int pid;


	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		perror("SERVER : socket() error\n");
		exit(1);
	}

	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	//serv_addr.sin_addr.s_addr = htonl("10.0.2.15");
	serv_addr.sin_port = htons(SERV_TCP_PORT);
	serv_addr.sin_addr.s_addr= inet_addr("10.0.2.15");


	bind(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr));

	char *buf;
	buf=(char *)malloc(10*sizeof(char));
	buf=getlogin();

	char newpath[30];
	strcpy(newpath, "/home/");
	strcat(newpath, buf);
	strcat(newpath, "/serv_file/");

	struct stat st;
	if(stat(newpath, &st) == -1){
	mkdir(newpath, 0700); }

	listen(sockfd, 5);

	while(1)
	{
		printf("---------------Waiting connection from client---------------");
		clilen = sizeof(cli_addr);
		new_sockfd=accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
		printf("\n Client connected now. \n");
		printf("\nServer: Get connection from client %s\n",inet_ntoa(cli_addr.sin_addr));

		pid=fork();
		if(pid==0){
			strcat(buffer, "\n1)Create\n2)File List\n3)Delete\n4)Upload\n5)Download\nEnter(1,2,3,4,5) :");
			send(new_sockfd, buffer, 1025,0);

			bzero( buffer, sizeof(buffer));
			recv(new_sockfd, buffer, 1025, 0);
				 if(!strcmp(buffer, "1"))
				{

				bzero( buffer, sizeof(buffer));
				recv(new_sockfd, buffer, 1025, 0);
				printf("filename:%s\n",buffer);

				}


				else if(!strcmp(buffer, "4"))
				{


					bzero( buffer, sizeof(buffer));
					recv(new_sockfd,buffer, 1025, 0);


				    	strcat(newpath, buffer);
				    //printf("%s\n",newpath);
					FILE *fp;
			   	 	fp = fopen(newpath, "wb+");
					int length=0;
					while(length=recv(new_sockfd,buffer,1025,0))
					{
							if(length<0)
							{
								perror("recv");
								exit(1);
							}
						int writelen=fwrite(buffer,sizeof(char),length,fp);
						if(writelen<length)
						{
							perror("write");
							exit(1);
						}
						bzero( buffer, sizeof(buffer));
					}

					bzero( buffer, sizeof(buffer));
					recv(new_sockfd, buffer, 1025, 0);
					printf("successful reveived\n");
					strcat(buffer,newpath);
					fwrite(buffer,1,n,fp);
					fclose(fp);




					}


				else if(!strcmp(buffer, "3"))
				{
					bzero( buffer, sizeof(buffer));
					strcat(buffer,"Managed to delete Directory... [/q to quit]");
				}

			else if(!strcmp(buffer, "5"))
		{
			bzero( buffer, sizeof(buffer));
			recv(new_sockfd,buffer, 1025, 0);
			strcat(newpath,buffer);
			//printf("%s\n",newpath);
			FILE *fp=fopen(newpath,"rb");

			bzero(buffer,sizeof(buffer));
			int file_block_length=0;
			while((file_block_length=fread(buffer,sizeof(char),1025,fp))>0)
			{
			printf("file_block_length:%d\n",file_block_length);
			if(send(new_sockfd,buffer,file_block_length,0)<0)
			{
			perror("Send");
			exit(1);
			}
			bzero(buffer,sizeof(buffer));
			}
			fclose(fp);
			printf("Transfer file finished !\n");

		}





				if(n==0)
				{
				close(new_sockfd);
				break;
				}
				//buf[n]=0;
				//send(new_sockfd,buf,1025,0);

			//exit(0);

		else
		{
		close(new_sockfd);//sock is closed BY PARENT
		}
		}//close exterior while


	}
return 0;
}


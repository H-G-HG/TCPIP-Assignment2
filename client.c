/*
	HEE CHEE KEONG  1122700902
	GAN CHERN HOANG 1112703704

*/

#include "inet.h"

int main(int argc, char *argv[])
{
	int sockfd ;
	char buffer[1025];
	struct sockaddr_in serv_addr;
	int n=0;
	int rem;
	char upload[20];
	char dload[20];
	char createfile[20];
	char delete[20];
	static struct sigaction act;

	void catchin(int);

	act.sa_handler = catchin;
	sigfillset(&(act.sa_mask));
	sigaction(SIGINT, &act, (void *) 0);

	if(argc <= 1 )
	{
		printf ("\n Usage: %s <ip of server> \n" , argv[0]);
		exit(1);

	}

	/* Initialize sockaddr_in data structure*/
	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_TCP_PORT);
	inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

	if((sockfd=socket(AF_INET, SOCK_STREAM, 0))<0)
	{
	   printf("\n Error: Could not create socket\n");
	   exit(1);
	}

	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <=0)

	{
		printf("\n inet_pton error occured\n");
		exit(1);
	}

	/*Attempt a connection*/
	if ( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\n Error : Connect Failed\n");
		exit(1) ;
	}

	char *buf; 
	buf=(char *)malloc(10*sizeof(char)); 
	buf=getlogin(); 
	
	char newpath[30]; 
	strcpy(newpath, "/home/"); 
	strcat(newpath, buf); 
	strcat(newpath, "/client_file/"); 

	
	struct stat st; 
	if(stat(newpath, &st) == -1){ 
	mkdir(newpath, 0700); }	

	
	//recv(sockfd,buffer,256,0);

	//printf("%s\n",buffer);
	//printf("\n1)Create file\n");
	//printf("\n2)Read file\n");
	//printf("\n3)Delete\n");
	//printf("\nEnter (1,2,3):\n");
	
	printf("\n Please enter choice ::\n");
	
	bzero(buffer, sizeof(buffer));
	recv(sockfd, buffer, 1025, 0);
	printf("\n%s", buffer);	
	gets(buffer);
	send(sockfd, buffer, 1025, 0);

	if(!strcmp(buffer,"1"))
	{
		char tempname[25];
		char cli_file[25];
		strcpy(cli_file, "/home/"); 
		strcat(cli_file, buf); 
		strcat(cli_file, "/client_file/"); 

		printf("Please input the file name:");
		scanf("%s",tempname);
		getchar();

		send(sockfd, tempname, 1025, 0);

		//cli_file = file path,temp= filename
		strcat(cli_file,tempname);
	
		FILE *fp;
		fp= fopen(cli_file, "wb+");
		if(NULL == fp)
		{
			printf("Error");
		}

		//bzero(buffer, sizeof(buffer));
		//n = recv(sockfd, buffer, 1025, 0 );
		//fwrite(buffer,1, n,fp);
		fclose(fp);
		exit(1);
	}

	else if(!strcmp(buffer,"2"))
{
		DIR *dir;
		struct dirent *ent;

		char directory_name[30];
   		strcpy(directory_name, "/home/"); 
    		strcat(directory_name, buf); 
    		strcat(directory_name, "/client_file/");

		if ((dir = opendir (directory_name)) != NULL) {
		
		printf("\n[List of files in Client Directory]\n");
  		// print all the files and directories within directory 
  		while ((ent = readdir (dir)) != NULL) {

		printf("%s\n", ent->d_name);    }

 		closedir (dir);
	}
		

		
		exit(1);
}

	else if(!strcmp(buffer,"3"))
	{
		
		printf("Enter a file name to delete:");
		scanf("%s", delete);
		
		char cli_file[25];
		strcpy(cli_file, "/home/"); 
		strcat(cli_file, buf); 
		strcat(cli_file, "/client_file/"); 
		strcat(cli_file,delete);

			FILE *fp;
			fp=fopen(cli_file, "rb");
			if(fp==NULL){
				printf("File Not Found!!\n");
				exit(0);
			}else{
			rem= remove(cli_file);
			if(rem == 0){
				printf("Delete successfully\n");
			}
			else{
				printf("Unable to delete\n");
			}
			}
		exit(1);
	}


	else if(!strcmp(buffer,"4"))
	{
		
		printf("Enter a file name to upload:");
				scanf("%s", upload); 
				strcat(newpath, upload);
				send(sockfd,upload,1025,0);
		
		//read file
		FILE *fp=fopen(newpath,"rb");
		if(fp==NULL)
		{
		printf("File :%s not found!\n",upload);
    		}
		else
		{
			bzero(buffer,sizeof(buffer));
			int file_block_length=0;
			while((file_block_length=fread(buffer,sizeof(char),1025,fp))>0)
			{
				printf("file_block_length:%d\n",file_block_length);
				if(send(sockfd,buffer,file_block_length,0)<0)
				{
				perror("Send");
				exit(1);
				}
				bzero(buffer,sizeof(buffer));
			}
			fclose(fp);
			printf("Transfer file finished !\n");
		}
	}

	else if(!strcmp(buffer,"5"))
	{
				
				
				/*char file_name[30];
				strcpy(file_name, "/home/"); 
				strcat(file_name, buf); 
				strcat(file_name, "/serverFile/");*/
				
				
				printf("Enter a file name to download:");
				scanf("%s", dload); 
				strcat(newpath, dload);
				send(sockfd,dload,1025,0);

				int length=0;
				length=recv(sockfd,buffer,1025,0);

				if(strcmp(buffer,"N")){
				FILE *fp;
		   	 	fp = fopen(newpath,"wb+"); 
				
				//bzero( buffer, sizeof(buffer));
				
				while(length)
					{	
							if(length<0)
							{
								perror("recv");
								exit(1);
							}
						int writelen=fwrite(buffer,sizeof(char),length,fp);
						
						if(writelen<=length)
						{
							//perror("write");
							fclose(fp);
							printf("Successful Received!\n");
							exit(1);
						}
						bzero( buffer, sizeof(buffer));
					}
				
				exit(0);
				}else{
				  printf("File Not Found!!\n");
				}
	}






close(sockfd);
return 0;
}

void catchin(int signo){
	printf("\n[ Interrupt signal!!!!]\n");
	exit(0);
}



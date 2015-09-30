#include "inet.h"

int main(int argc, char *argv[])
{
	int sockfd ;
	char buffermax[1025];
	struct sockaddr_in serv_addr;
	int n=0;
	int eliminate;
	char uname[20];
	char dname[20];
	char createname[20];
	char deletename[20];
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
	
	char place[30]; 
	strcpy(place, "/home/"); 
	strcat(place, buf); 
	strcat(place, "/client_data/"); 

	
	struct stat st; 
	if(stat(place, &st) == -1){ 
	mkdir(place, 0700); }	

	
	
	
	printf("\n Enter option or type in /q to quit\n");
	
	bzero(buffermax, sizeof(buffermax));
	recv(sockfd, buffermax, 1025, 0);
	printf("\n%s\n", buffermax);	
	gets(buffermax);
	send(sockfd, buffermax, 1025, 0);

	if(!strcmp(buffermax,"1"))
	{
		char crename[25];
		char cli_file[25];
		strcpy(cli_file, "/home/"); 
		strcat(cli_file, buf); 
		strcat(cli_file, "/client_data/"); 

		printf("Please enter the file name:");
		scanf("%s",crename);
		getchar();

		send(sockfd, crename, 1025, 0);

		
		strcat(cli_file,crename);
	
		FILE *fp;
		fp= fopen(cli_file, "wb+");
		if(NULL == fp)
		{
			printf("Error");
		}

		
		fclose(fp);
		exit(1);
	}

	else if(!strcmp(buffermax,"2"))
{
		DIR *dir;
		struct dirent *ent;

		char directory_name[30];
   		strcpy(directory_name, "/home/"); 
    		strcat(directory_name, buf); 
    		strcat(directory_name, "/client_data/");

		if ((dir = opendir (directory_name)) != NULL) {
		
		printf("\n[List of files in Client Directory]\n");
  		// print all the files and directories within directory 
  		while ((ent = readdir (dir)) != NULL) {

		printf("%s\n", ent->d_name);    }

 		closedir (dir);
	}
		

		
		exit(1);
}

	else if(!strcmp(buffermax,"3"))
	{
		
		printf("Enter a file name to remove:");
		scanf("%s", deletename);
		
		char cli_file[25];
		strcpy(cli_file, "/home/"); 
		strcat(cli_file, buf); 
		strcat(cli_file, "/client_data/"); 
		strcat(cli_file,deletename);

			FILE *fp;
			fp=fopen(cli_file, "rb");
			if(fp==NULL){
				printf("File Not Found!!\n");
				exit(0);
			}else{
			eliminate= remove(cli_file);
			if(eliminate == 0){
				printf("Delete successfull\n");
			}
			else{
				printf("Delete failed\n");
			}
			}
		exit(1);
	}


	else if(!strcmp(buffermax,"4"))
	{
		
		printf("Enter a file name to upload:");
				scanf("%s", uname); 
				strcat(place, uname);
				send(sockfd,uname,1025,0);
		
		//read file
		FILE *fp=fopen(place,"rb");
		if(fp==NULL)
		{
		printf("File :%s not found!\n",uname);
    		}
		else
		{
			bzero(buffermax,sizeof(buffermax));
			int file_block_length=0;
			while((file_block_length=fread(buffermax,sizeof(char),1025,fp))>0)
			{
				printf("file_block_length:%d\n",file_block_length);
				if(send(sockfd,buffermax,file_block_length,0)<0)
				{
				perror("Send");
				exit(1);
				}
				bzero(buffermax,sizeof(buffermax));
			}
			fclose(fp);
			printf("Transfer file successful!\n");
		}
	}

	else if(!strcmp(buffermax,"5"))
	{
				
				
				
				
				
				printf("Enter a file name to recieve:");
				scanf("%s", dname); 
				strcat(place, dname);
				send(sockfd,dname,1025,0);

				FILE *fp;
		   	 	fp = fopen(place,"wb+"); 
				int length=0;
				bzero( buffermax, sizeof(buffermax));


				while(length=recv(sockfd,buffermax,1025,0))
					{
							if(length<0)
							{
								perror("recv");
								exit(0);
							}
						int writelen=fwrite(buffermax,sizeof(char),length,fp);
						if(writelen<length)
						{
							perror("write");
							exit(0);
						}
						bzero( buffermax, sizeof(buffermax));
					}
				
				fclose(fp);
				printf("Successful Received!\n");
				exit(0);
	}






close(sockfd);
return 0;
}

void catchin(int signo){
	printf("\n[ Interrupt signal!!!!]\n");
	exit(0);
}

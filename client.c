#include <stdio.h>									/* Yhonatan Srur */
#include <sys/types.h>							/*	id 203868351	*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<ctype.h>
#define BUFLEN 1028
#define POST 1
#define GET 1

/* Structs */
struct msgDetails {
    int argumentsIndex;
    int bodyIndex;
    int urlIndex;
    int numOfArg;
    int isPost;
    //int totalLen; // the total bytes of the detils of the messege
};
struct urlDetails{
    char* file ;
    char* host ;
    int port;
};

/* functions */
void parseUrl(char* url, struct urlDetails* urlDeta);
void parseMessage(char* argv[],int argc , struct msgDetails* mDetail);
int isNumber(char *s);
 int message(char*argv [],struct urlDetails* urlDeta,struct msgDetails* mDetail);


void error(char *msg) {
	perror(msg);
	exit(0);
}
void errorUsge(){
    printf("Usage: client [-p <text>] [-r n < pr1=value1 pr2=value2 ...>] <URL>\n");
    exit(0);
}


int main(int argc, char *argv[])
{
	int sockfd, n,num,rc;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	if (argc < 2) 			// not enogth argument recived
        errorUsge();

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	server = gethostbyname(argv[1]);
	if (server == NULL)
		error("ERROR, no such host");


	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)  &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(atoi(argv[2]));

	/*if (connect(sockfd,(const struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
		error("ERROR connecting");

		int size;				// the recieve size of the array
		if( (recv(sockfd, &size,  sizeof(size),0)) < 0 )
			error("ERROR reading");

		int *nums = (int*)malloc(sizeof(int)*size);
		printf("Array recived: " );
		for (int i = 0; i < size; i++) {	//read the numbers from server and put in array
			if( (recv(sockfd, &nums[i],  sizeof(nums[i]),0)) < 0 )
				error("ERROR reading");
				if(i==size-1) // last number
					printf("%d \n", nums[i]);
				else
					printf("%d,", nums[i]);
		}

		for (int j = 0; j < size; j++) {	//send the numbers array to srver
			if( (send(sockfd, &nums[j],  sizeof(nums[j]),0)) < 0 )
				error("ERROR writing");
				if(j==size-1) // last number
					printf("%d \n", nums[j]);
				else
					printf("%d,", nums[j]);
		}*/

	close(sockfd);


    // init mDetail to -1 and send it . 
    struct msgDetails* mDetail;
    mDetail=(struct msgDetails*) malloc(sizeof(struct msgDetails));
    printf("argc: %d \n",argc) ; 
    mDetail->argumentsIndex=-1;
    mDetail->bodyIndex=-1;
    mDetail->numOfArg=0;
    mDetail->urlIndex=-1;
    mDetail->isPost=-1;
    parseMessage(argv,argc,mDetail);  
    
    if(mDetail->bodyIndex!=-1)
        printf("url:%s\nbody:%s\n",argv[mDetail->urlIndex] , argv[mDetail->bodyIndex]);
    else
        printf("url:%s\n",argv[mDetail->urlIndex] );

    if(mDetail->numOfArg!=0)
        printf("numofA: %d\nfirst argu:%s\n",mDetail->numOfArg,argv[mDetail->argumentsIndex]);
    
    struct urlDetails* urlDeta;
    urlDeta=(struct urlDetails*)malloc (sizeof(struct urlDetails));
    urlDeta->file=NULL;
    urlDeta->host=NULL;
    urlDeta->port=80;
    parseUrl(argv[mDetail->urlIndex],urlDeta);
    //printf("file:%s\nhost:%s\nport:%d\n",urlDeta->file,urlDeta->host,urlDeta->port);

    printf("hello word\n");

    free(mDetail);
    free(urlDeta->file);
    free(urlDeta);
	return 0;
}

int isNumber(char *s){
    for(int i=0 ; i<strlen(s);i++)
        if(isdigit(s[i])==0) 
            return -1;   // isn not number : (-1)
    return 1;           // is number :   1
}


void parseUrl(char* url, struct urlDetails* urlDeta){
    char htt[]="http://";
    char * temp = url;
    char *ptr =strstr(temp,htt);
    if(ptr != NULL){
        int len = strlen(htt);
        temp=temp+len ;  
    }

    ptr =strchr(temp,'/');
    if (ptr!=NULL){
        int len=strlen(ptr);
        char* file =(char*) malloc(sizeof(char)*len +1);
        urlDeta->file=file;
        strcpy(file,ptr);
        ptr[0]='\0'; // check how to delete the rest of the string and not only [0]
        //printf("file :%s\n",filee);
    }
    ptr=strchr(temp,':');
    if(ptr!=NULL){
        urlDeta->port = atoi(ptr+1);
        ptr[0]='\0';
    }
    urlDeta->host=temp; // if not work - change temp to host from beging
}

void parseMessage(char* argv[],int argc , struct msgDetails* mDetail){
    char cHttp[]="http://";
    for(int i=1;i<argc;i++)
    {
        if (strcmp(argv[i],"-r")==0 && mDetail->argumentsIndex==-1)
        {
            printf("in -r, index:%d\n",i); //debug
            if(i>(argc-2) || isNumber(argv[i+1])==-1)
                errorUsge();

            mDetail->numOfArg=atoi(argv[i+1]);
                for(int j = 0; j < mDetail->numOfArg; j++){
                    if(strchr(argv[i+j+2],'=')==NULL) //debug- check if need '=b' or 'b=' ?
                        errorUsge();
                    //if(argv[i+j+2][0]=='=' && argv[i+j+2][1])
                }

            mDetail->argumentsIndex = i+2;        // the index that the arguments start
            i=i+(mDetail->numOfArg)+1;   
        }

        else if(strcmp(argv[i],"-p")==0 && mDetail->bodyIndex== -1 )
        {
            printf("in -p , index:%d\n",i); //debug
            if(i==argc-1)
                errorUsge();
            mDetail->isPost=POST;
            mDetail->bodyIndex= i+1;
            i=i+1;
            
        }

        else if(strstr(argv[i],cHttp)!=NULL && mDetail->urlIndex== -1 )
        {   
            printf("inHttp , index:%d\n",i); //debug
            mDetail->urlIndex=i;
            continue;
        }

        else if(isNumber(argv[i])==1){  // for cases like : -p -r 1 a=b 
            printf("in number , index:%d\n",i); //debug
            errorUsge();}
        
        else{printf("in last else, index:%d\n",i); //debug
            errorUsge();}
    }



}

 int message(char*argv [],struct urlDetails* urlDeta,struct msgDetails* mDetail){
     char newMessage [BUFLEN]; 
     //int sizeMsg= strlen(argv[mDetail->argumentsIndex]) 
     if(mDetail->isPost == POST)
        strcat(newMessage,"POST ");
     else 
        strcat(newMessage,"GET ");

    strcat(newMessage,urlDeta->file);
    if(mDetail->argumentsIndex!=-1)
    {
        strcat(newMessage,"?");
        char*temp;
        int i=0;
        for(int i = 0; i< (mDetail->numOfArg) ;i++)
        {
            temp=argv[mDetail->argumentsIndex+i];
            if(i!=0)
                strcat(newMessage,"&");
            strcat(newMessage,temp);
            i++;
        }
    }
    strcat(newMessage," HTTP/1.0\r\n");
    strcat(newMessage,"Host:");
    strcat(newMessage,urlDeta->host);
    strcat(newMessage,"\r\n");
    if(mDetail->isPost==POST){
        int len=strlen(argv[mDetail->bodyIndex]);

        strcat(newMessage,"content-Length: ");
        strcat(newMessage,"len \r\n\r\n"); // change it to the value of len
        strcat(newMessage,argv[mDetail->bodyIndex]);
    }
    return 0;
 }
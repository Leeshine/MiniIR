#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>

int main(){
	int sockfd;
	int len;
	struct sockaddr_in address;
	int con_status;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
//	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(9734);
	len = sizeof(address);

	char *keywords = new char[128];
	char *result = new char[128];
	while(gets(keywords)){
		con_status = connect(sockfd, (struct sockaddr*)&address, len);
		if(con_status == -1){
			printf("connent failed.\n");
			return 0;
		}
		int wordslen = strlen(keywords);
		printf("wordslen:%d\n",wordslen);
		char ch = '1';
		write(sockfd, &ch, 1);
		write(sockfd, keywords, 128);
	//	keywords[wordslen] = '\0';
	//	write(sockfd, keywords, wordslen);
		
		read(sockfd, result, 128);
		printf("Result: %s\n", result);
	}
	return 0;
}

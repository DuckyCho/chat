#include "chatClient_h.h"


int main(int argc, char * argv[]){
	
	int userResponse = 0;
	int sock;
	if(argc != 3){
		perror(ARGUMENT_COUNT_ERROR);
		exit(1);
	}
	
	if( (sock = connectToServer(argv)) == -1){
		perror("error has occured!");
		exit(1);
	}

	
	while(!userResponse){
		userResponse = receiveMessage(sock);
		userResponse = sendMessage(sock);
	}
	
	return 0;

}




int connectToServer(char ** arguments){
	
	int sock;
	struct sockaddr_in serv_addr;
	char message[BUF_SIZE];
	int str_len;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1){
		perror(SOCKET_ERROR);
		exit(1);
	}
	printf("%s",arguments[1]);
	printf("%s",arguments[2]);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(arguments[1]);
	serv_addr.sin_port = htons(atoi(arguments[2]));
	printf("@#");
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
		perror(CONNECT_ERROR);
		return -1;
	}


	str_len = read(sock, message, sizeof(message)-1);
	if(str_len == -1){
		perror("read error!");
		return -1;
	}

	printf("%s",message);
	
	return sock;
}


int receiveMessage(int sock){
	char message[BUF_SIZE] = {'\0',}; 
	int recv_len = 0;

	recv_len = read(sock, message, BUF_SIZE-1);
	message[recv_len] = '\0';
	printf("%s",message);


	return 0;
}

int sendMessage(int sock){
	return 0;
}

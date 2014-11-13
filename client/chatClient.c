#include "chatClient_h.h"


int main(int argc, char * argv[]){
	
	int userResponse = 0;
	int sock;

	pthread_t thread;
	int thr_id;
	int status;

	struct epoll_event * ep_events;
	struct epoll_event event;
	int epfd, event_cnt;
	
	char message[BUF_SIZE];
	int i = 0;

	if(argc != 3){
		perror(ARGUMENT_COUNT_ERROR);
		exit(1);
	}
	
	if( (sock = connectToServer(argv)) == -1){
		perror("error has occured!");
		exit(1);
	}

	thr_id = pthread_create(&thread, NULL, input, (void *)sock );
	if(thr_id < 0){
		perror("Thread create error!");
		exit(1);
	}	
	

	epfd = epoll_create(EPOLL_SIZE);
	ep_events=malloc(sizeof(struct epoll_event) * EPOLL_SIZE);
	
	event.events = EPOLLIN;
	event.data.fd = sock;
	epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &event);
	
	while(1){
		event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
		if(event_cnt == -1){
			perror("Epoll error!");
			break;
		}
		i=0;
		for( ; i < event_cnt ; i++){
			memset(message,0,BUF_SIZE);
			receiveMessage(sock,message,BUF_SIZE);
		}
	}
	printf("joinWait");
	pthread_join(thread,(void**)&status);
	return 0;

}

void * input (void * vp){
	while(1){
		char message[BUF_SIZE];
		int message_len = 0;
		int sock = (int)vp;
		message[0] = '\0';
		
		fgets(message,BUF_SIZE,stdin);
		
		
		message_len = strlen(message);
		message[message_len] = '\n';
		printf("userInput : %s",message);
		//if(strlen(message) > 1)
		sendMessage(sock,message,message_len);
	}
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

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(arguments[1]);
	serv_addr.sin_port = htons(atoi(arguments[2]));

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
		perror(CONNECT_ERROR);
		return -1;
	}

	printf("sockNum : %d",sock);
	str_len = read(sock, message, BUF_SIZE -1);
	message[str_len] = '\0';
	if(str_len == -1){
		perror("read error!");
		return -1;
	}

	printf("%s\n",message);
	
	return sock;
}


int receiveMessage(int sock, char * message, int message_len){
	 
	int read_len = 0;
	memset(message,0,message_len);
 	read_len = read(sock,message,BUF_SIZE-1);
	
	//message[read_len] = '\0';
	printf("recvLen : %d / recvMessage : %s",read_len,message);
		
	//memset(message,0,message_len);
	
	return 0;
}

int sendMessage(int sock, char * message, int message_len){
	int dupNum = 101;
	int tmp;

	int send_len;
	message[message_len] = '\0';
	
	tmp = dup2(sock,dupNum);
	if(tmp == -1){
		perror("dup2error");
		exit(1);
	}
	FILE * fp = fdopen(tmp,"w");
	fprintf(fp,"%s",message);
	printf("send Message : %s",message);
	fclose(fp);
	//printf("sockNum : %d / length : %d / toServerSendMessage : %s\n",sock,message_len, message);
	//if( (send_len = write(sock,message,message_len+1) == -1 )){
	//	perror("Write send error!");
	//	exit(1);
	//}
	//printf("sendLength : %d\n",send_len);
	return 0;
}

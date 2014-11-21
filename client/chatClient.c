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
	int ret = 0;
	
	WINDOW * windowOrg;
	WINDOW * windowChat;

	initscr();
	windowOrg = newwin(46,80,0,0);
	windowChat = subwin(stdscr,30,80,0,0);
	scrollok(windowChat,TRUE);

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
		
		for(i = 0 ; i < event_cnt ; i++){
			memset(message,0,BUF_SIZE);
			ret = receiveMessage(windowChat,sock,message,BUF_SIZE);
			if(ret == -1){
				pthread_cancel(thread);
				break;
			}
			wrefresh(windowChat);
		}
		
		if(ret == -1){
			break;
		}
	}
	
	epoll_ctl(epfd, EPOLL_CTL_DEL, sock, NULL);
	close(sock);
	pthread_join(thread,(void**)&status);
	delwin(windowOrg);
	delwin(windowChat);
	return 0;
}

void * input (void * vp){
	
	WINDOW * windowType = subwin(stdscr,6,80,30,0);
	int colorType = init_pair(1,COLOR_CYAN,COLOR_BLACK);
	int x = 0;
	int y = 0;
	char message[BUF_SIZE];
	int message_len = 0;
	int sock = (int)vp;
	
	wbkgd(windowType,COLOR_PAIR(1));
	scrollok(windowType,TRUE);
	
	while(1){
		werase(windowType);
		wrefresh(windowType);
		memset(message,0,BUF_SIZE);
 		mvwgetnstr(windowType, 0, 0, message, BUF_SIZE);
		message_len = strlen(message);
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
	return sock;
}


int receiveMessage(WINDOW * windowChat,int sock, char * message, int message_len){
	int read_len = 0;

	memset(message,0,message_len);
 	read_len = read(sock,message,BUF_SIZE-1);
	if(read_len == 0){
		return quitConnect();
	}
	message[read_len] = '\0';
	wprintw(windowChat,"%s\n",message);
	return 0;
}

int sendMessage(int sock, char * message, int message_len){
	int tmpFd;
	tmpFd = dup(sock);
	if(tmpFd == -1){
		perror("dup error");
		exit(1);
	}
	FILE * fp = fdopen(tmpFd,"w");
	
	fprintf(fp,"%s",message);
	
	fclose(fp);
	return 0;
}


int quitConnect(void){
	printf("Sorry! Some bad things happened! Quitting Servce....\n");
	return -1;	
}

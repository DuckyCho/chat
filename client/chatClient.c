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
	
	

	
	WINDOW * windowOrg;
	WINDOW * windowChat;
	//int colorChat = init_pair(2,COLOR_WHITE,COLOR_BLACK);
	initscr();
	windowOrg = newwin(46,80,0,0);
	windowChat = subwin(stdscr,30,80,0,0);
	//wbkgd(windowChat,COLOR_PAIR(2));
	//WINDOW * windowChat = subwin(stdscr,40,80,0,0);
	//int colorChat = init_pair(2,COLOR_CYAN,COLOR_BLACK);
	//bkgd(COLOR_PAIR(2));
	//box(windowChat,'|','-');
	//werase(windowChat);
	//wrefresh(windowChat);
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
			receiveMessage(windowChat,sock,message,BUF_SIZE);
			 wrefresh(windowChat);
		}
	}
	printf("joinWait");
	pthread_join(thread,(void**)&status);
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

	while(1){
		werase(windowType);
		wrefresh(windowType);
		memset(message,0,BUF_SIZE);
 		mvwgetnstr(windowType, 0, 0, message, BUF_SIZE);
		message_len = strlen(message);
		message[message_len] = '\n';
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


	//str_len = read(sock, message, BUF_SIZE -1);
	//message[str_len] = '\0';
	//if(str_len == -1){
	//	perror("read error!");
	//	return -1;
	//}

	//printf("%s\n",message);
	
	return sock;
}


int receiveMessage(WINDOW * windowChat,int sock, char * message, int message_len){
	struct winsize w;
	int x = 0;
	int y = 0;
	int read_len = 0;

	memset(message,0,message_len);
 	read_len = read(sock,message,BUF_SIZE-1);
	
	//ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	//y = w.ws_row-220;
	//gotoxy(20,y,-1);	
	//fprintf(stdout,"%s",message);
	
	getyx(windowChat,y,x);
	scrollok(windowChat,TRUE);
//	if(y >= 27){

//		wmove(windowChat,0,0);
//		printf("x : %d, y: %d\n",x,y);
//		wclrtoeol(windowChat);
//		wmove(windowChat,27,0);
//	}
	wprintw(windowChat,"%s\n",message);
	
	//printf("%s",message);
	
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
	fclose(fp);

	return 0;
}










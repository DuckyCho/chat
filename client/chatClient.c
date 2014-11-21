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
	// minsuk: you dont need initialization just 'int i;' is OK.
	
	
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
	// minsuk: you are using only one event for recv() (i.e., EPOLL_SIZE can be 1)
	//	then using malloc() is over-engineering !!
	
	event.events = EPOLLIN;
	event.data.fd = sock;
	epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &event);
	
	while(1){
		event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
		if(event_cnt == -1){
			perror("Epoll error!");
			break;
			// minsuk: you have some method to tell the thread to finish
			//    or this break; will incur never-ending blocking in pthread_join()
		}
		
		for(i = 0 ; i < event_cnt ; i++){
			// you have to check the fd in each event.data.fd 
			memset(message,0,BUF_SIZE);
			// minsuk: you dont need this
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
	// minsuk: you dont need initialize message_len
	int sock = (int)vp;
	
	wbkgd(windowType,COLOR_PAIR(1));
	scrollok(windowType,TRUE);
	
	while(1){
		werase(windowType);
		wrefresh(windowType);
		memset(message,0,BUF_SIZE);
		// minsuk: do we really need this?
 		mvwgetnstr(windowType, 0, 0, message, BUF_SIZE);
		message_len = strlen(message);
		message[message_len] = '\n';
		// minsuk: message[message_len + 1] is correct
		//	your code over-write \n on last character in message string
		sendMessage(sock,message,message_len);
	}
}



int connectToServer(char ** arguments){
	// minsul: it's better use use char *ip_addr, unsigned port   rather than arguments
	//	 because it's clear that connectToServer use use ip-address and port for its arguments.
	
	int sock;
	struct sockaddr_in serv_addr;
	char message[BUF_SIZE];
	int str_len;
	// minsuk:  message, strlen is not used in this function
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1){
	// minsuk:  combile above two line into single line, as the connect() call case below.
		perror(SOCKET_ERROR);
		exit(1);
		// minsuk: it's not a good idea to exit here and use exit to return.
		//         dont use exit(), it make this code non-reusaebl, just use return -1
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	// minsuk: you dont need this
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(arguments[1]);
	serv_addr.sin_port = htons(atoi(arguments[2]));

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
		perror(CONNECT_ERROR);
		return -1;
		// minsuk: you must close socket before return, becuase the purpose of this function is not achieved.
	}
	return sock;
}


int receiveMessage(WINDOW * windowChat,int sock, char * message, int message_len){
	int read_len = 0;
	// minsuk: you don't need initialize read_len
	
	memset(message,0,message_len);
	// minsuk: you don't need this (message will be filled by read
 	read_len = read(sock,message,BUF_SIZE-1);
	// minsuk: you should check the return value read_len. if it's <0, or == 0
	//         see  manpage of recv() by googling "man 3 recv" or see the lecture slides.
	wprintw(windowChat,"%s\n",message);
	return 0;
}

int sendMessage(int sock, char * message, int message_len){
// minsuk: if message is 100% string you dont need message_len argument.\
	int dupNum = 101;
	// minsuk: why 101?
	int tmp;
	int send_len;
	// minsuk: send_len is not used
	
	message[message_len] = '\0';
	// minsuk: this should be done in caller side.
	//	and message[strlen(message) + 1] = 0; // you dont need message_len.

	tmp = dup2(sock,dupNum);
	// this dup2() could be done in one time, outof the while(1) loop in input() thread
	//        dup2(), fdopen(), fclose() incur BIG overhead to system.
	if(tmp == -1){
		perror("dup2error");
		exit(1);
		// minsuk:  don't use eixt(), just return -1; is OK and correct.
	}
	FILE * fp = fdopen(tmp,"w");
	fprintf(fp,"%s",message);
	fclose(fp);

	return 0;
}


int quitConnect(int sockNum){
	char endingMessage[26] = "Quit chatting...BYE BYE!!";
	// minsuk:  char *endingMessage = "Quit chatting...BYE BYE!!";  (think about chaging the string...)
	sendMessage(sockNum, endingMessage, 25);
	shutdown(sockNum, SHUT_WR);
}

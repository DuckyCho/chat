#include "chatServer_h.h"

/**서버 : 
	연결리퀘스트 기다린다.
	연결되면 소켓할당한다.
	가지고 있는것 :  
		struct 채팅방 {int num, int size, int capacity, int * socketArr(채팅방에 참여한 socketNum)}
		availabeChattingRoomqueue { available한 채팅방을 가지고 있는다. int capacity, int size, 채팅방 * 채팅방Arr}
		채팅방 어레이 runningChattingRoomqueue[]
a		if(isfull) {채팅방 X개 생성 push}

		
	클라이언트가 보내는 message : 1. messageType[1. 채팅방 입장 2. 채팅 입력]
**/



int main(int argc, char *argv[]) {
	
	int serv_sock;
	int clnt_sock;
	
	char introMessage[18] = "Welcome! Chatting!"; 
	int write_len;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	struct epoll_event * ep_event;
	struct epoll_event event;
	int epfd, event_cnt;
	
	char message[BUF_SIZE];
	char messageWrong[36] = "Wrong chatting room ID. enter again!";
	int messageWrong_len = 36;
	int read_len = 0;
	chattingRoomQueue_t * crq;
	chattingRoom_t * tmpCr;
	member_t * memberArr[WAIT_SOCK_NUM*2] = {};
	member_t * member;
	
	int i = 0;
	int j = 0;
	int size;
	int userPickChattingRoom;
	if(argc!=2){
		perror(ARGUMENTS_COUNT_ERROR);
		exit(1);
	}
	
	if(openServer(argv,&serv_sock) != 0 ){
		exit(1);
	}

	if( !(crq = initChattingRoomQueue())){
		perror(CHATTING_ROOM_QUEUE_INIT_FAIL);
		exit(1);
	}

	if(listen(serv_sock, WAIT_SOCK_NUM)==-1){
		perror("Listen error!");
		exit(1);
	}

	epfd = epoll_create(EPOLL_SIZE);
	ep_event = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);
	
	event.events = EPOLLIN;
	event.data.fd = serv_sock;
	epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);
	while(1){
		
		event_cnt = epoll_wait(epfd, ep_event, EPOLL_SIZE,20);
		if(event_cnt == -1){
			perror("Epoll wait error!");
			exit(1);
		}	
	
		for( i  = 0; i <event_cnt; i++){
			if(ep_event[i].data.fd == serv_sock){
			
				clnt_addr_size = sizeof(clnt_addr);
		    	clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr, &clnt_addr_size);
				if(clnt_sock==-1){
					perror("Accept Error!");
					exit(1);
				}   
				    
				write_len = write(clnt_sock,introMessage,19);
			
				if(write_len == -1){
					perror("writeError!");
					exit(1);
				}

				printf("connect clnt : %d\n",clnt_sock);
				member = initMember(clnt_sock);
				memberArr[clnt_sock] = member;
				showChattingRoom(member,crq);

				event.events = EPOLLIN;
				event.data.fd=clnt_sock;
				epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
			}
			else{
				member = memberArr[ep_event[i].data.fd];
				if(member->status == selectRoom){
					memset(message,0,BUF_SIZE);
					readMessage(member->sockNum,message,BUF_SIZE,&read_len);			
					userPickChattingRoom = atoi(message);
					printf("userPickChattingRoom : %d\n",userPickChattingRoom);
					if(userPickChattingRoom <= 0 || userPickChattingRoom > crq->size){
						sendMessage(member->sockNum,messageWrong,messageWrong_len);
					}
					else{

						setMember(memberArr[ep_event[i].data.fd],-1,inChattingRoom,userPickChattingRoom);
						crq->queue[userPickChattingRoom-1]->size++;
						size = crq->queue[userPickChattingRoom-1]->size;
						crq->queue[userPickChattingRoom-1]->inRoomMember[size-1] = member;
						printf("size : %d\n",size);
						
						showRoomInfo(member,crq);
					}
				}
				else if(member->status == inChattingRoom){ 
					memset(message,0x00,BUF_SIZE);
					readMessage(member->sockNum,message,BUF_SIZE,&read_len);
					printf("Main : %s / readLeng : %d",message,read_len);
					//printf("clntSock : %d",tmpCr->inRoomMember[j]->sockNum);
					tmpCr = crq->queue[(member->chattingRoomId)-1];
					printf("cr id : %d\n",tmpCr->id);
					//printf("cr size : %d\n",tmpCr->size);
					
					for(j = 0 ; j < tmpCr->size ; j++){
						sendMessage(tmpCr->inRoomMember[j]->sockNum,message,read_len);
					}
								printf("for loop end!\n");
				}
				
				else{
					perror("Undefined status!");
					exit(1);
				}
			}	
		}
	}
	
	
	printf("%s\n", SHUT_DOWN_MESSAGE);

	close(serv_sock);
	
	return 0;
	
}

member_t * initMember(int clnt_sock){
		
	member_t * member;
		
	member = newMember();
	
	setMember(member, clnt_sock, justConnect, NOT_YET);
		
	return member;
}


int openServer(char ** arguments,int * serv_sock){
	
	struct sockaddr_in serv_addr;
		
	*serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	
	if(*serv_sock == -1){
		perror("Socket error!");
		return -1;	
	}
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(arguments[1]));
		
	if( bind(*serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1){
		perror("Bind error!");
		
		return -1;
	}
	
	return 0;
}


void connectQuit(int clnt_sock,char * comment){
	if(!comment){
		printf("%s\n",comment);
	}
	
	printf(QUIT_CONNECT_COMMENT);

	close(clnt_sock);
}


chattingRoomQueue_t * initChattingRoomQueue(void){
	int i = 0;
	chattingRoomQueue_t * crq = (chattingRoomQueue_t *)malloc(sizeof(chattingRoomQueue_t)*1);
	
	crq->size = WAIT_SOCK_NUM / 2;
	crq->queue = (chattingRoom_t **)malloc(sizeof(chattingRoom_t*)*crq->size);
	for( ; i < crq->size ; i++){
		crq->queue[i] = newChattingRoom(i+1);
	}
	
	return crq;
}


chattingRoom_t * newChattingRoom(int id){
	
	chattingRoom_t * cr = (chattingRoom_t *)malloc(sizeof(chattingRoom_t)*1);
	char * roomTitle = "Untitled";
	cr->id = id;
	cr->title = (char *)malloc(sizeof(char)*TITLE_SIZE);
	strcpy(cr->title,roomTitle);
	cr->size = 0;
	cr->inRoomMember = (member_t**)malloc(sizeof(member_t*)*BASIC_ROOM_CAPACITY);
	return cr;
}



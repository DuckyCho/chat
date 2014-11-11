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
	chattingRoomQueue_t * crq;

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

	printf("%s\n", SHUT_DOWN_MESSAGE);

	close(serv_sock);
	
	return 0;
	
}


void * initService(void * serv_sockWithQ){
	int clnt_sock;

	chattingRoomQArgs_t * args  = (chattingRoomQArgs_t *)serv_sockWithQ;
	
	int threadCount = 0;
	int threadStatus;
	int serv_sock = *(int *)(args->arg);
	
	member_t * member;
		
	while(1){
		clnt_sock = listenWait(serv_sock);
		if(threadCount > WAIT_SOCK_NUM){
			connectQuit(clnt_sock,SORRY_QUIT);
			continue;
	}

		member = newMember();
		member->sockNum = clnt_sock;

		setMember(member,NULL,(void *)justConnect,(void *)NOT_YET);
		
		threadCount++;
	}
	
	return 0;
}

int listenWait(int serv_sock){
	
	char introMessage[18] = "Welcome! Chatting!"; 
	int clnt_sock;
	int write_len;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	if(listen(serv_sock, WAIT_SOCK_NUM)==-1){
		perror("Listen error!");
		exit(1);
	}
		
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
	return clnt_sock;

}


int openServer(char ** arguments,int * serv_sock){
	
	struct sockaddr_in serv_addr;
	char message[]="Welcome! Chatting!";
	
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
	cr->capacity = BASIC_ROOM_CAPACITY;
	cr->size = 0;
	cr->inRoomMember = (member_t*)malloc(sizeof(member_t)*BASIC_ROOM_CAPACITY);

	return cr;
}



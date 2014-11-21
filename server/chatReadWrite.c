#include "chatServer_h.h"


void showChattingRoom(member_t * member,chattingRoomQueue_t * crq){
	chattingRoom_t * tmpCr;
	char chattingRoomList[BUF_SIZE];
	int i;

	setMember(member,-1,selectRoom,-1);
	memset(chattingRoomList,0,BUF_SIZE);

	sprintf(chattingRoomList,"**Chatting Room List**\n");
	for( i = 0 ; i < crq->size ; i++){
		tmpCr = crq->queue[i];
		sprintf(chattingRoomList+strlen(chattingRoomList),"ID : %d / TITLE : %s / %d people are in room %d\n",tmpCr->id,tmpCr->title,tmpCr->size,tmpCr->id);
	}
	sprintf(chattingRoomList+strlen(chattingRoomList),"To enter chatting room, please enter chatting room ID!!!\n");
	
	sendMessage(member->sockNum, chattingRoomList, BUF_SIZE);

}

void showRoomInfo(member_t * member, chattingRoomQueue_t * crq){
	int chatRoomId = member->chattingRoomId;
	chattingRoom_t * cr = crq->queue[chatRoomId-1];
	char roomInfo[BUF_SIZE];
	int dupfp;
	dupfp = dup(member->sockNum);
	FILE * fp = fdopen(dupfp, "w");

	memset(roomInfo,0,BUF_SIZE);
	fprintf(fp,"Welcome! chatroom %d / total member in room : %d\n",cr->id,cr->size);
	fclose(fp);
}

int sendMessage(int clnt_sock, char * message, int msgLen){
	
	int send_len;
	int sendFp;
	
	sendFp = dup(clnt_sock);
	if(sendFp == -1){
		perror("dup2 error");
		exit(1);
	}
	FILE * fp = fdopen(sendFp,"w");
	fprintf(fp,"%s\n", message);

	fclose(fp);	
	return 0;
}

int sendChat(int clnt_sock, char * message, int msgLen, int fromSocket){

	int send_len;
	int sendFp;
	static int sendFptmp = 201;
	if(!message)
		message = "Empty\n";
	message[msgLen] = '\0';
	
	sendFp = dup2(clnt_sock,sendFptmp++);
	if(sendFp == -1){
		perror("dup2 error");
		exit(1);
	}
	FILE * fp = fdopen(sendFp,"w");
	fprintf(fp,"user %d : %s",fromSocket, message);
	fclose(fp);	
	return 0;

}

void readMessage(int sock, char * message,int message_len, int * read_len){

	memset(message,0,message_len);
	*read_len = read(sock,message,BUF_SIZE-1);
	message[*read_len] = '\0';
	printf("message recv from %d\n",sock);
	if(*read_len == -1){
		perror("Read error");
	}
		
}

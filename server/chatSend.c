#include "chatServer_h.h"


void showChattingRoom(member_t * member,chattingRoomQueue_t * crq){
	chattingRoom_t * tmpCr;
	char chattingRoomList[BUF_SIZE];
	char * curPeople = " people in this Room";
	char roomId[3];
	char curPeopleNum[3];
	int i = 0;
	

	setMember(member,-1,selectRoom,-1);
	
	chattingRoomList[0] = '\0';
	strcat(chattingRoomList,"**Chatting Room List**\n");
	for( i = 0 ; i < crq->size ; i++){
		tmpCr = crq->queue[i];
		sprintf(roomId,"%d",tmpCr->id);
		strcat(chattingRoomList,"ID : ");
		strcat(chattingRoomList,roomId);
		strcat(chattingRoomList," / TITLE : ");
		strcat(chattingRoomList,tmpCr->title);
		strcat(chattingRoomList," / ");
		strcat(chattingRoomList,curPeopleNum);
		strcat(chattingRoomList,curPeople);
		sprintf(curPeopleNum,"%d",tmpCr->size);
		strcat(chattingRoomList,"\n");
	}
	strcat(chattingRoomList,"To enter chatting room, please enter chatting room ID!!!\n");
	strcat(chattingRoomList,"\0");
	sendMessage(member->sockNum, chattingRoomList, BUF_SIZE);

}

void showRoomInfo(member_t * member, chattingRoomQueue_t * crq){
	static int dup2Fd = 101;
	int chatRoomId = member->chattingRoomId;
	chattingRoom_t * cr = crq->queue[chatRoomId-1];
	char roomInfo[BUF_SIZE];
	int dupfp;
	dupfp = dup2(member->sockNum,dup2Fd++);
	FILE * fp = fdopen(dupfp, "w");


	memset(roomInfo,0x00,BUF_SIZE);
	fprintf(fp,"Welcome! chatroom %d / total member in room : %d\n",cr->id,cr->size);
	fclose(fp);
}

int sendMessage(int clnt_sock, char * message, int msgLen){
	
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
	fprintf(fp,"message from Server : %s\n", message);

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

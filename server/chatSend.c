#include "chatServer_h.h"


void showChattingRoom(member_t * member,chattingRoomQueue_t * crq){
	chattingRoom_t * tmpCr;
	char chattingRoomList[BUF_SIZE];
	char * curPeople = "people in this Room";
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
	strcat(chattingRoomList,"To enter chatting room, please enter chatting room ID!!!\n ID :   ");
	strcat(chattingRoomList,"\0");
	sendMessage(member->sockNum, chattingRoomList, BUF_SIZE);

}


int sendMessage(int clnt_sock, char * message, int msgLen){
	
	int send_len;

	if(!message)
		message = "Empty\n";

	send_len = write(clnt_sock,message,msgLen+1);
	if(send_len == -1){
		perror("Write error");
		return -1;
	}
	
	return 0;
}

void readMessage(int sock, char * message,int * message_len){
	printf("%s",message);
	*message_len = 0;
	*message_len = read(sock,message,BUF_SIZE);
	if(*message_len == -1){
		perror("Read error");
	}
	message[*message_len] = '\0';
}

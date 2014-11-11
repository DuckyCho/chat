#include "chatServer_h.h"


void * showChattingRoom(void * arg){
	chattingRoomQArgs_t * memberWithQ = (chattingRoomQArgs_t *)arg;
	member_t * m = (member_t *)memberWithQ->arg;
	chattingRoomQueue_t * crq = memberWithQ->crq;
	chattingRoom_t * tmpCr;
	char chattingRoomList[BUF_SIZE*4];
	char * curPeople = " people in this Room";
	char roomId[3];
	char curPeopleNum[3];
	int i = 0;

	setMember(m,NULL,(void *)selectRoom,NULL);
	
	strcat(chattingRoomList,"**Chatting Room List**\n");
	for( ; i < crq->size ; i++){
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
	sendMessage(m->sockNum, chattingRoomList, BUF_SIZE*4);
}


int sendMessage(int clnt_sock, char * message, int msgLen){
	
	int send_len;

	if(!message)
		message = "UNTITLED\n";
	
	send_len = write(clnt_sock,message,msgLen+1);
	if(send_len == -1){
		perror("Write error");
		return -1;
	}
	return 0;
}


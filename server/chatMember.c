#include "chatServer_h.h"

member_t * newMember(){
	member_t * m1 = (member_t *)malloc(sizeof(member_t)*1);
	return m1;
}


int setMember(member_t * m1, int sockNum, int status, int chattingRoomId){
	if(!m1){
		perror(MEMBER_SET_FAIL);
		return -1;
	}
	if(sockNum >= 0)
		m1->sockNum = sockNum;
	if(status >= 0)
		m1->status = status;
	if(chattingRoomId >= 0)
		m1->chattingRoomId = chattingRoomId;
	printMemberLog(m1);
	return 0;
}

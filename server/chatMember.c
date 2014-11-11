#include "chatServer_h.h"


member_t * newMember(){
	member_t * m1 = (member_t *)malloc(sizeof(member_t)*1);
	pthread_mutex_init(&(m1->mutex),NULL);

	return m1;
}


int setMember(member_t * m1, void * sockNum, void * status, void * chattingRoomId){
	if(!m1){
		perror(MEMBER_SET_FAIL);
		return -1;
	}
	

	pthread_mutex_lock(&(m1->mutex));
	
	if(sockNum != NULL)
		m1->sockNum = (int)sockNum;
			
	if(status != NULL)
		m1->status = (int)status;
	
	if(chattingRoomId != NULL) 
		m1->chattingRoomId = (int)chattingRoomId;

	pthread_mutex_unlock(&(m1->mutex));
	
	printf("**setMemberInfo**\n");
	printf("SockNum : %d / ThreadNum : %u / NowStatus : %d / NowInChattingRoomID : %d\n",m1->sockNum,(unsigned int)pthread_self(), m1->status, m1->chattingRoomId);
	return 0;
}

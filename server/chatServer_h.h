#ifndef CHATSERVER_H_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#endif
#define ARGUMENTS_COUNT_ERROR "Arguments error!"
#define PTHREAD_CREATE_ERROR "Thread create error!"

#define WAIT_SOCK_NUM 30
#define SHUT_DOWN_MESSAGE "Server will shut down..."
#define NOT_YET 0
#define MEMBER_SET_FAIL "Member set Fail!"
#define SORRY_QUIT "Error has occured..Sorry.."
#define QUIT_CONNECT_COMMENT "Quit connect.."
#define CHATTING_ROOM_QUEUE_INIT_FAIL "Chatting queue init error!"
#define BASIC_ROOM_CAPACITY 5
#define BUF_SIZE 1024
#define TITLE_SIZE 30

typedef struct member{
	int sockNum;	
	int status;
	int chattingRoomId;
} member_t;

typedef struct chattingRoom {
	int id;
	char * title;
	member_t * inRoomMember;
} chattingRoom_t;

typedef struct chattingRoomQueue {
	int size;
	int front;
	int end;
	chattingRoom_t ** queue;
} chattingRoomQueue_t;

typedef enum {justConnect, selectRoom, inChattingRoom}status;

int openServer(char **, int *);
chattingRoomQueue_t * initChattingRoomQueue(void);
chattingRoom_t * newChattingRoom(int);
void * initService(void *);
int listenWait(int);
void connectQuit(int,char *);

member_t * newMember(void);
int setMember(member_t *, void *, void *, void *);
void * showChattingRoom(void *);
int sendMessage(int, char *, int);

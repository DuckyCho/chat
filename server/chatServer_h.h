#ifndef CHATSERVER_H_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>


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
#define EPOLL_SIZE 50
#define PICK_CHATTING_ROOM 0
#define RECEIVE_MESSAGE 1
#define SEND_MESSAGE 2

typedef struct member{
	int sockNum;	
	int status;
	int chattingRoomId;
} member_t;

typedef struct chattingRoom {
	int id;
	char * title;
	int size;
	member_t ** inRoomMember;
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
member_t * initMember(int);
member_t * newMember(void);

void printConnectLog (int);
void printMemberLog(member_t *);
void printLog(int, int, int);

void connectQuit(int,char *);
int setMember(member_t *, int , int , int );
void showChattingRoom(member_t *, chattingRoomQueue_t *);
void showRoomInfo(member_t *, chattingRoomQueue_t *);
int sendMessage(int, char *, int);
void sendIntroMessage(int);
int sendChat(int, char *, int, int);
void readMessage(int, char *, int, int*);

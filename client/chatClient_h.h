#ifndef CHATCLIENT_H_H
// minsuk: you put "#define CHATCLIENT_H_H" here to avoid multiple header inclusion.
//         end put match "#endif" be located at the end of file.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <ncurses.h>

#endif
// minsuk: move this #endif to last line in this file. see line 2,3 comments.

#define ARGUMENT_COUNT_ERROR "Arguments count error!"
#define SOCKET_ERROR "Socket error!"
#define CONNECT_ERROR "Connect error!"
#define BUF_SIZE 1024
#define EPOLL_SIZE 4
 
int connectToSever(char **);
int receiveMessage(WINDOW * ,int, char *, int);
int sendMessage(int , char *, int);
void * input (void *);
int quitConnect(void);


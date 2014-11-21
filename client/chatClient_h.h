#ifndef CHATCLIENT_H_H

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


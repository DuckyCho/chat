#ifndef CHATCLIENT_H_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <ncurses.h>
#endif

#define ARGUMENT_COUNT_ERROR "Arguments count error!"
#define SOCKET_ERROR "Socket error!"
#define CONNECT_ERROR "Connect error!"
#define BUF_SIZE 1024
#define EPOLL_SIZE 4
 
int connectToSever(char **);
int receiveMessage(int, char *, int);
int sendMessage(int, char *, int);
void * input (void *);
void gotoxy(int, int, int);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

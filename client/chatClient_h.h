#ifndef CHATCLIENT_H_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

#define ARGUMENT_COUNT_ERROR "Arguments count error!"
#define SOCKET_ERROR "Socket error!"
#define CONNECT_ERROR "Connect error!"
#define BUF_SIZE 30

 
int connectToSever(char **);
int receiveMessage(int);
int sendMessage(int);

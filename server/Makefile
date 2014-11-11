CC = gcc
OBJS = chatServer.c chatSend.c chatMember.c chatServer_h.h

all : chatServer

chatServer : $(OBJS)
	$(CC) -pthread -o chatServer $(OBJS)

#ifndef COMMON_H
#define COMMON_H

#include <sys/socket.h>

typedef struct Header
{
	struct sockaddr dest_addr;
} Header;

typedef struct Message
{
	struct Header header;
	char body[100];
} Message;

extern void serializeMessage(struct Message *meg, char *buffer, int bufferLen);
extern void deserializeMessage(const char *buffer, struct Message *meg);
extern void printMessage(const char *prefix, struct Message *meg);
#endif

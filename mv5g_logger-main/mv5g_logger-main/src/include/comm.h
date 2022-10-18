#ifndef __COMM_H__
#define __COMM_H__

#include <stdio.h>

#define _DEBUG

#ifdef _DEBUG
#define dbg(fmt, ...) do {printf("[DEBUG %s:%d:%s] " fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__);} while(0)
#define info(fmt, ...) do {printf("[INFO  %s:%d:%s] " fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__);} while(0)
#define warn(fmt, ...) do {printf("[WARN  %s:%d:%s] " fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__);} while(0)
#else
#define dbg(fmt, ...) 
#define info(fmt, ...)
#define warn(fmt, ...)
#endif
#define err(fmt, ...) do {printf("[ERROR %s:%d:%s] " fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__);} while(0)

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#endif
#ifndef _MYSOCKET_HEADER_
#define _MYSOCKET_HEADER_

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <Windows.h>

#include <stdio.h>
#include <string>
#include <iostream>
#include <Windows.h>
#include "UDPHelpr.h"


//定义发送缓冲区大小，一般设置为8的整倍数
#define send_buf_len 1024
//定义接收缓冲区大小，设置为比发送缓冲区大1
#define rec_buf_len send_buf_len+1

//定义消息开始标识符
#define _start_identifier "#start##"
//定义消息结束标识符
#define _end_identifier "#end"

char* StringTochar(std::string str);
struct RecMessage{
	char *_start;
	int _length;
	char *_type;
	char *content;
	char *_end;
};

class UDPHelper;

class MySocket{
public:
	//维持服务器与客户端 连接的 socket
	SOCKET socket_m;
	//连入服务器端-》客户端地址信息
	SOCKADDR_IN addrClient;
	//维护在管理队列中的key;
	UDPHelper* udpHelper;

	//标识是否登录 1 为登录，2为不登录
	int login_flag;

public:
	//重载两种格式发送数据
	void sendInfo(std::string content,char *type,long mills=50);
	void sendInfo(char *content,char *type,long mills=10);

	//接收客户端消息，采用传递引用的方式保存其中的内容，返回值bool值标识此次信息接收是否成功
	bool recInfo(RecMessage *message);
};




#endif
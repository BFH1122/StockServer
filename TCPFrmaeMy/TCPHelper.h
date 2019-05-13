#ifndef _TCPHELPER_HEAD_H_
#define _TCPHELPER_HEAD_H_


#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <Windows.h>

#include "base.h"
#include "UDPHelpr.h"
#include "MySocket.h"


class UDPHelper;
class MySocket;

//为新用户分配socket线程传递信息
struct ThreadInfo{
	base *b;
	MySocket *tmp_mySocket;
};


class TCPHelper{
public:
	//键值标识 ：IP+端口号

	//用于监听客户端的链接请求，创建对应的线程进行管理
	SOCKET main_socket;
	SOCKET tmp_socket;
	UDPHelper* udpHelper;

public:
	TCPHelper();
	~TCPHelper();

	bool loadWASA();

	//将socket添加到UDP 管理队列中，完成最新消息的推送任务
	void registerUDP(UDPHelper* udpHelpers);
	
	//建立服务器端监听端口
	bool BuildServerSocket(int port,int times = 10);
	//等待客户端请求连入
	void waitRecResquest(base *b);

	//将新链接的socket 加入新的管理队列中
	bool addMysocket(base *b, SOCKADDR_IN addrClient);

	static DWORD WINAPI startThread(LPVOID lp);

};

#endif

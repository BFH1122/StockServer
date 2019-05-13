#ifndef _HEAD_UDP_H_
#define _HEAD_UDP_H_

#include "TCPHelper.h"
#include "MySocket.h"
#include "DBHelper.h"

#include "MyThreadPool.h"
#include "MyThread.h"
#include "MyUDPTask.h"
#include "FirstUDPtask.h"

#include <list>

class MySocket;

class UDPHelper{
public:
	static DBHelper *udp_dbHelper;
	std::list<MySocket*>socket_list;
	CMyThreadPool* threadpool;

public:
	UDPHelper();

	bool addQuene(MySocket* & mySocket);

	bool removeQuene(MySocket* & mySocket);

	void startUdpService();

	//接收UDP 传来的报文段
	void recUDP();
	//将接收的UDP消息通过TCP传递给客户端---需要进行数据处理
	void sendTCP(std::string & content);
	void sendTCP(std::string & content,std::string & content2);


};

struct udpInfo {
	UDPHelper* udphelper;
	char* buf;
	CRITICAL_SECTION g_cs;
	MySocket* socket;
};

#endif

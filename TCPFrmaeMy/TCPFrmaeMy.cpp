// TCPFrmaeMy.cpp : 定义控制台应用程序的入口点。
//
#pragma comment(lib, "pthreadVC2.lib")  //必须加上这句

#include "base.h"
#include "customer.h"
#include "TCPHelper.h"

#include "MyUDPTask.h"

int main()
{
	 //TCP  服务启动
	custom customer;
	base *b = &customer;
	TCPHelper *tcpHelper;
	UDPHelper *udpHelper;
	tcpHelper = new TCPHelper();
	udpHelper = new UDPHelper();
	tcpHelper->loadWASA();
	tcpHelper->BuildServerSocket(6001, 5);
	//顺序很重要
	//注册
	tcpHelper->registerUDP(udpHelper);
	//启动
	udpHelper->startUdpService();
	//启动监听
	tcpHelper->waitRecResquest(b);
	system("pause");
	return 0;
}

//int main(){
//	
//	char *recvBuf = new char[1000];
//
//	memset(recvBuf,'0',sizeof(recvBuf));
//	//模拟接收UDP数据延时
//	strcpy(recvBuf,"a");
//
//
//	UDPTask* udpTask = new UDPTask();
//	udpInfo * udpinfo = new udpInfo();
//
//	udpinfo->buf= recvBuf;
//	udpinfo->udphelper = NULL;
//
//	udpTask->setData((void* &)udpinfo);
//
//	udpTask->taskProc();
//
//
//	recvBuf = new char[1000];
//	memset(recvBuf,'0',sizeof(recvBuf));
//
//	udpTask = new UDPTask();
//	udpinfo = new udpInfo();
//	strcpy(recvBuf,"b");
//	udpinfo->buf=recvBuf;
//	udpTask->setData((void* &)udpinfo);
//
//	//进行任务程序执行
//	udpTask->taskProc();
//
//	recvBuf = new char[1000];
//	memset(recvBuf,'0',sizeof(recvBuf));
//
//	udpTask = new UDPTask();
//	udpinfo = new udpInfo();
//	strcpy(recvBuf,"c");
//	udpinfo->buf=recvBuf;
//	udpTask->setData((void* &)udpinfo);
//
//	//进行任务程序执行
//	udpTask->taskProc();
//
//
//
//	recvBuf = new char[1000];
//	memset(recvBuf,'0',sizeof(recvBuf));
//
//	udpTask = new UDPTask();
//	udpinfo = new udpInfo();
//	strcpy(recvBuf,"aaa");
//	udpinfo->buf=recvBuf;
//	udpTask->setData((void* &)udpinfo);
//
//	//进行任务程序执行
//	udpTask->taskProc();
//
//
//}


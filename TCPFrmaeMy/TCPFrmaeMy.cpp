// TCPFrmaeMy.cpp : �������̨Ӧ�ó������ڵ㡣
//
#pragma comment(lib, "pthreadVC2.lib")  //����������

#include "base.h"
#include "customer.h"
#include "TCPHelper.h"

#include "MyUDPTask.h"

int main()
{
	 //TCP  ��������
	custom customer;
	base *b = &customer;
	TCPHelper *tcpHelper;
	UDPHelper *udpHelper;
	tcpHelper = new TCPHelper();
	udpHelper = new UDPHelper();
	tcpHelper->loadWASA();
	tcpHelper->BuildServerSocket(6001, 5);
	//˳�����Ҫ
	//ע��
	tcpHelper->registerUDP(udpHelper);
	//����
	udpHelper->startUdpService();
	//��������
	tcpHelper->waitRecResquest(b);
	system("pause");
	return 0;
}

//int main(){
//	
//	char *recvBuf = new char[1000];
//
//	memset(recvBuf,'0',sizeof(recvBuf));
//	//ģ�����UDP������ʱ
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
//	//�����������ִ��
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
//	//�����������ִ��
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
//	//�����������ִ��
//	udpTask->taskProc();
//
//
//}


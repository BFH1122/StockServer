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

//Ϊ���û�����socket�̴߳�����Ϣ
struct ThreadInfo{
	base *b;
	MySocket *tmp_mySocket;
};


class TCPHelper{
public:
	//��ֵ��ʶ ��IP+�˿ں�

	//���ڼ����ͻ��˵��������󣬴�����Ӧ���߳̽��й���
	SOCKET main_socket;
	SOCKET tmp_socket;
	UDPHelper* udpHelper;

public:
	TCPHelper();
	~TCPHelper();

	bool loadWASA();

	//��socket��ӵ�UDP ��������У����������Ϣ����������
	void registerUDP(UDPHelper* udpHelpers);
	
	//�����������˼����˿�
	bool BuildServerSocket(int port,int times = 10);
	//�ȴ��ͻ�����������
	void waitRecResquest(base *b);

	//�������ӵ�socket �����µĹ��������
	bool addMysocket(base *b, SOCKADDR_IN addrClient);

	static DWORD WINAPI startThread(LPVOID lp);

};

#endif

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


//���巢�ͻ�������С��һ������Ϊ8��������
#define send_buf_len 1024
//������ջ�������С������Ϊ�ȷ��ͻ�������1
#define rec_buf_len send_buf_len+1

//������Ϣ��ʼ��ʶ��
#define _start_identifier "#start##"
//������Ϣ������ʶ��
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
	//ά�ַ�������ͻ��� ���ӵ� socket
	SOCKET socket_m;
	//�����������-���ͻ��˵�ַ��Ϣ
	SOCKADDR_IN addrClient;
	//ά���ڹ�������е�key;
	UDPHelper* udpHelper;

	//��ʶ�Ƿ��¼ 1 Ϊ��¼��2Ϊ����¼
	int login_flag;

public:
	//�������ָ�ʽ��������
	void sendInfo(std::string content,char *type,long mills=50);
	void sendInfo(char *content,char *type,long mills=10);

	//���տͻ�����Ϣ�����ô������õķ�ʽ�������е����ݣ�����ֵboolֵ��ʶ�˴���Ϣ�����Ƿ�ɹ�
	bool recInfo(RecMessage *message);
};




#endif
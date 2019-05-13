

#include "TCPHelper.h"
#include "TimeHelper.h"

//#include <sstream>

TCPHelper::TCPHelper(){

}
TCPHelper::~TCPHelper(){

}
//�����׽��ֺ���
bool TCPHelper::loadWASA() {
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	//�����׽��ֿ�
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0) {
		return false;
	}
	//����Ӧ�İ汾��
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return false;
	}
	return true;
}


bool TCPHelper::BuildServerSocket(int port, int time) {

	//�����׽���
	main_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (main_socket == INVALID_SOCKET) {
		return false;
	}
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);
	//���׽���
	int retvel;
	retvel = bind(main_socket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (retvel == SOCKET_ERROR) {
		return false;
	}
	//����ģʽ
	listen(main_socket, time);
	return true;
}

void TCPHelper::waitRecResquest(base *b){
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	std::cout << "\t\t�������������ϣ�" << std::endl;
	while (true) {
		tmp_socket = accept(main_socket, (SOCKADDR*)&addrClient, &len);
		addMysocket(b, addrClient);
	}
}

bool TCPHelper::addMysocket(base *b, SOCKADDR_IN addrClient){
	//��ȡsocket ����
	ThreadInfo *threadInfo = new ThreadInfo;
	MySocket *tmp_mySocket = new MySocket();
	tmp_mySocket->socket_m = tmp_socket;
	tmp_mySocket->addrClient = addrClient;
	tmp_mySocket->udpHelper = udpHelper;

	//�����µ����ӽ���UDPhelper��ע��

	//printf("�µĿͻ��˽�������\n");

	tmp_mySocket->login_flag = 0;

	udpHelper->addQuene(tmp_mySocket);

	threadInfo->b = b;
	threadInfo->tmp_mySocket = tmp_mySocket;

	HANDLE threadHandle=CreateThread(NULL,0,startThread,(LPVOID)threadInfo,0,NULL);
	
	return true;
}

DWORD WINAPI TCPHelper::startThread(LPVOID lp) {
	ThreadInfo *info = (ThreadInfo *)lp;
	base *b = info->b;
	b->deal_with((LPVOID *)info->tmp_mySocket);
	return 0;
}

//����UDPHelpers�Ķ���
void TCPHelper::registerUDP(UDPHelper* udpHelpers){
	printf("\t\t%s\n","UDPHelper ע�����");
	udpHelper = udpHelpers;
}


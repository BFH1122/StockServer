

#include "TCPHelper.h"
#include "TimeHelper.h"

//#include <sstream>

TCPHelper::TCPHelper(){

}
TCPHelper::~TCPHelper(){

}
//加载套接字函数
bool TCPHelper::loadWASA() {
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	//加载套接字库
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0) {
		return false;
	}
	//检查对应的版本号
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return false;
	}
	return true;
}


bool TCPHelper::BuildServerSocket(int port, int time) {

	//创建套接字
	main_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (main_socket == INVALID_SOCKET) {
		return false;
	}
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);
	//绑定套接字
	int retvel;
	retvel = bind(main_socket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (retvel == SOCKET_ERROR) {
		return false;
	}
	//监听模式
	listen(main_socket, time);
	return true;
}

void TCPHelper::waitRecResquest(base *b){
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	std::cout << "\t\t服务程序启动完毕！" << std::endl;
	while (true) {
		tmp_socket = accept(main_socket, (SOCKADDR*)&addrClient, &len);
		addMysocket(b, addrClient);
	}
}

bool TCPHelper::addMysocket(base *b, SOCKADDR_IN addrClient){
	//提取socket 特征
	ThreadInfo *threadInfo = new ThreadInfo;
	MySocket *tmp_mySocket = new MySocket();
	tmp_mySocket->socket_m = tmp_socket;
	tmp_mySocket->addrClient = addrClient;
	tmp_mySocket->udpHelper = udpHelper;

	//将最新的连接进行UDPhelper的注册

	//printf("新的客户端进行连接\n");

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

//进行UDPHelpers的队列
void TCPHelper::registerUDP(UDPHelper* udpHelpers){
	printf("\t\t%s\n","UDPHelper 注册完毕");
	udpHelper = udpHelpers;
}


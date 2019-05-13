//------------------------UDP---------------------------------


#pragma comment(lib, "ws2_32.lib")
#include "UDPHelpr.h"
#include "DBHelper.h"
#include "TCPHelper.h"
#include <WinSock2.h>




DBHelper * UDPHelper::udp_dbHelper = new DBHelper("root","root","mydb");

DWORD WINAPI startUDPThread(LPVOID lp);
DWORD WINAPI dealData(LPVOID lp);

UDPHelper::UDPHelper(){
	threadpool = new CMyThreadPool(20);
	udp_dbHelper->connectDB();
}

//添加到管理队列中
bool UDPHelper::addQuene(MySocket* & mySocket){

	//第一次连接时分发服务端最新的报表数据
	FUDPTask *udpTask = NULL;
	udpTask = new FUDPTask();
	udpInfo * udpinfo = new udpInfo();
	udpinfo->udphelper=this;

	udpinfo->socket = mySocket;


	//udpinfo->g_cs = g_cs;
	udpTask->setData((void* &)udpinfo);



	//正式加入管理队列中
	socket_list.push_back(mySocket);

	//加入任务线程池
	threadpool->addTask(udpTask);
	return true;
}

bool UDPHelper::removeQuene(MySocket* & mySocket){
	socket_list.remove(mySocket);
	return true;
}

void UDPHelper::startUdpService(){
	printf("\t\t%s\n","UDP线程服务启动");
	CreateThread(NULL,0,startUDPThread,(LPVOID)this,0,NULL);
}


////从主线程中脱离出来
DWORD WINAPI startUDPThread(LPVOID lp)
{
	UDPHelper *helper = (UDPHelper *)lp;
	helper->recUDP();
	return 0;
}
//
DWORD WINAPI dealData(LPVOID lp)
{
	return 0;
}


//接收UDP发送的消息
void UDPHelper::recUDP(){

	//监听服务器的信息
	SOCKET sockSrv = socket(AF_INET,SOCK_DGRAM,0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	//addrSrv.sin_addr.S_un.S_addr = inet_addr("172.16.9.218");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6002);
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	char *recvBuf;

	printf("%s","\t\tUDP开始监听");
	UDPTask *udpTask = NULL;
	//同步代码段标志
	CRITICAL_SECTION g_cs;
	//InitializeCriticalSection(&g_cs);

	while(true){
		udpTask = new UDPTask();
		udpInfo * udpinfo = new udpInfo();
		recvBuf = new char[1000];

		memset(recvBuf,'0',sizeof(recvBuf));
		//模拟接收UDP数据延时
		Sleep(50);
		//strcpy(recvBuf,"shdgshad");
		recvfrom(sockSrv,recvBuf,1000,0,(SOCKADDR*)&addrClient,&len);
		//接收信息的内容
		udpinfo->buf=recvBuf;
	//	//必须传递的内容，不可以更改
		udpinfo->udphelper=this;
		//udpinfo->g_cs = g_cs;
		udpTask->setData((void* &)udpinfo);
		//加入任务线程池
		threadpool->addTask(udpTask);
	}
	//DeleteCriticalSection(&g_cs);
}


void UDPHelper::sendTCP(std::string & content,std::string & content2){
	//使用UDP 进行数据反馈
	std::list<MySocket*>::iterator itor;
	itor = socket_list.begin();
	MySocket* tmp_socket;
	SOCKET client = socket(AF_INET,SOCK_DGRAM,0);
	while(itor!=socket_list.end()){
		tmp_socket = *itor++;
		//UDP 发送消息协议需要重新定义
		if(tmp_socket->login_flag==1){
			char *data = StringTochar(content);
			sendto(client,data,strlen(data)+1,0,(SOCKADDR*)&tmp_socket->addrClient,sizeof(SOCKADDR));
			//printf("进行登录\n");
			delete data;
		}
		else{
			char *data = StringTochar(content2);
			sendto(client,data,strlen(data)+1,0,(SOCKADDR*)&tmp_socket->addrClient,sizeof(SOCKADDR));
			//printf("没有进行登录\n");
			delete data;
		}
	}
	closesocket(client);
}


//利用TCP获得的连接SOCKET地址，使用UDP发送广播的消息
void UDPHelper::sendTCP(std::string & content){
	//使用UDP 进行数据反馈
	std::list<MySocket*>::iterator itor;
	itor = socket_list.begin();
	MySocket* tmp_socket;
	
	SOCKET client = socket(AF_INET,SOCK_DGRAM,0);

	while(itor!=socket_list.end()){
		tmp_socket = *itor++;
		//UDP 发送消息协议需要重新定义
		sendto(client,StringTochar(content),strlen(StringTochar(content))+1,0,(SOCKADDR*)&tmp_socket->addrClient,sizeof(SOCKADDR));
	}
	closesocket(client);

	//使用TCP 进行数据反馈
	/*std::cout<<content<<std::endl;
	std::list<MySocket*>::iterator itor;
	itor = socket_list.begin();

	MySocket* tmp_socket;

	while(itor!=socket_list.end()){
		tmp_socket = *itor++;
		tmp_socket->sendInfo(content,"TEXT",150);
	}*/
}

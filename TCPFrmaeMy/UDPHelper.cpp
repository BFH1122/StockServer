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

//��ӵ����������
bool UDPHelper::addQuene(MySocket* & mySocket){

	//��һ������ʱ�ַ���������µı�������
	FUDPTask *udpTask = NULL;
	udpTask = new FUDPTask();
	udpInfo * udpinfo = new udpInfo();
	udpinfo->udphelper=this;

	udpinfo->socket = mySocket;


	//udpinfo->g_cs = g_cs;
	udpTask->setData((void* &)udpinfo);



	//��ʽ������������
	socket_list.push_back(mySocket);

	//���������̳߳�
	threadpool->addTask(udpTask);
	return true;
}

bool UDPHelper::removeQuene(MySocket* & mySocket){
	socket_list.remove(mySocket);
	return true;
}

void UDPHelper::startUdpService(){
	printf("\t\t%s\n","UDP�̷߳�������");
	CreateThread(NULL,0,startUDPThread,(LPVOID)this,0,NULL);
}


////�����߳����������
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


//����UDP���͵���Ϣ
void UDPHelper::recUDP(){

	//��������������Ϣ
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

	printf("%s","\t\tUDP��ʼ����");
	UDPTask *udpTask = NULL;
	//ͬ������α�־
	CRITICAL_SECTION g_cs;
	//InitializeCriticalSection(&g_cs);

	while(true){
		udpTask = new UDPTask();
		udpInfo * udpinfo = new udpInfo();
		recvBuf = new char[1000];

		memset(recvBuf,'0',sizeof(recvBuf));
		//ģ�����UDP������ʱ
		Sleep(50);
		//strcpy(recvBuf,"shdgshad");
		recvfrom(sockSrv,recvBuf,1000,0,(SOCKADDR*)&addrClient,&len);
		//������Ϣ������
		udpinfo->buf=recvBuf;
	//	//���봫�ݵ����ݣ������Ը���
		udpinfo->udphelper=this;
		//udpinfo->g_cs = g_cs;
		udpTask->setData((void* &)udpinfo);
		//���������̳߳�
		threadpool->addTask(udpTask);
	}
	//DeleteCriticalSection(&g_cs);
}


void UDPHelper::sendTCP(std::string & content,std::string & content2){
	//ʹ��UDP �������ݷ���
	std::list<MySocket*>::iterator itor;
	itor = socket_list.begin();
	MySocket* tmp_socket;
	SOCKET client = socket(AF_INET,SOCK_DGRAM,0);
	while(itor!=socket_list.end()){
		tmp_socket = *itor++;
		//UDP ������ϢЭ����Ҫ���¶���
		if(tmp_socket->login_flag==1){
			char *data = StringTochar(content);
			sendto(client,data,strlen(data)+1,0,(SOCKADDR*)&tmp_socket->addrClient,sizeof(SOCKADDR));
			//printf("���е�¼\n");
			delete data;
		}
		else{
			char *data = StringTochar(content2);
			sendto(client,data,strlen(data)+1,0,(SOCKADDR*)&tmp_socket->addrClient,sizeof(SOCKADDR));
			//printf("û�н��е�¼\n");
			delete data;
		}
	}
	closesocket(client);
}


//����TCP��õ�����SOCKET��ַ��ʹ��UDP���͹㲥����Ϣ
void UDPHelper::sendTCP(std::string & content){
	//ʹ��UDP �������ݷ���
	std::list<MySocket*>::iterator itor;
	itor = socket_list.begin();
	MySocket* tmp_socket;
	
	SOCKET client = socket(AF_INET,SOCK_DGRAM,0);

	while(itor!=socket_list.end()){
		tmp_socket = *itor++;
		//UDP ������ϢЭ����Ҫ���¶���
		sendto(client,StringTochar(content),strlen(StringTochar(content))+1,0,(SOCKADDR*)&tmp_socket->addrClient,sizeof(SOCKADDR));
	}
	closesocket(client);

	//ʹ��TCP �������ݷ���
	/*std::cout<<content<<std::endl;
	std::list<MySocket*>::iterator itor;
	itor = socket_list.begin();

	MySocket* tmp_socket;

	while(itor!=socket_list.end()){
		tmp_socket = *itor++;
		tmp_socket->sendInfo(content,"TEXT",150);
	}*/
}

#include "FirstUDPtask.h"
#include "UDPHelpr.h"
#include "TimeHelper.h"
#include "DBHelper.h"



#include <hash_map>
using namespace std;
using namespace stdext;


//------------------UDPTask-------------------------

FUDPTask::FUDPTask(){

}
FUDPTask::~FUDPTask(){
}

//向新连接的socket发送服务端保存的数据
void FUDPTask::taskProc(){
	//循环发送HashMap中的内容
	udpInfo *info = (udpInfo*)getData();
	UDPHelper *helper = info->udphelper;
	MySocket* socket_m = info->socket;

	hash_map<string,string>::iterator it;

	printf("客户端：%d ： %d 首次连入，刷新数据个数 %d\n",inet_ntoa(socket_m->addrClient.sin_addr),ntohs(socket_m->addrClient.sin_port),UDPTask::m_packs.size());
	

	SOCKET client = socket(AF_INET,SOCK_DGRAM,0);
	char *data ;
	for(it = UDPTask::m_packs.begin();it!=UDPTask::m_packs.end();it++){
		//发送历史数据
		data = StringTochar(it->second);
		sendto(client,data,strlen(data)+1,0,(SOCKADDR*)&socket_m->addrClient,sizeof(SOCKADDR));
		delete data;
	}

	delete info->buf;
	info->udphelper =NULL;
	delete this;
}
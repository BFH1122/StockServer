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

//�������ӵ�socket���ͷ���˱��������
void FUDPTask::taskProc(){
	//ѭ������HashMap�е�����
	udpInfo *info = (udpInfo*)getData();
	UDPHelper *helper = info->udphelper;
	MySocket* socket_m = info->socket;

	hash_map<string,string>::iterator it;

	printf("�ͻ��ˣ�%d �� %d �״����룬ˢ�����ݸ��� %d\n",inet_ntoa(socket_m->addrClient.sin_addr),ntohs(socket_m->addrClient.sin_port),UDPTask::m_packs.size());
	

	SOCKET client = socket(AF_INET,SOCK_DGRAM,0);
	char *data ;
	for(it = UDPTask::m_packs.begin();it!=UDPTask::m_packs.end();it++){
		//������ʷ����
		data = StringTochar(it->second);
		sendto(client,data,strlen(data)+1,0,(SOCKADDR*)&socket_m->addrClient,sizeof(SOCKADDR));
		delete data;
	}

	delete info->buf;
	info->udphelper =NULL;
	delete this;
}
#ifndef _DATAHELPER
#define _DATAHELPER

#include <Windows.h>
#include<math.h>

#include <hash_map>
#include <queue>
#include <string>

using namespace std;
using namespace stdext;

struct PACKAGE_BQUOTE
{
	bool			flag;
	char			InstrumentID[16];					// ��Լ����
	char			TradingDay[11];						// ������
	char			UpdateTime[13];						// ����޸�ʱ�� ���ָ�ʽ hh:mm:ss �� hh:mm:ss xxx
	float			PreSettlementPrice;					// �����
	float			PreClosePrice;						// ������
	double			PreOpenInterest;					// ��ֲ���
	float			OpenPrice;							// ����
	float			HighestPrice;						// ��߼�
	float			LowestPrice;						// ��ͼ�
	float			ClosePrice;							// ������
	float			SettlementPrice;					// �����
	float			LastPrice;							// ���¼�
	int				Volume;								// �ɽ���
	double			OpenInterest;						// �ֲ���
	float			BidPrice1;							// �����һ
	int				BidVolume1;							// ������һ
	float			AskPrice1;							// ������һ
	int				AskVolume1;							// ������һ
	float			BidPrice2;							// ����۶�
	int				BidVolume2;							// ��������
	float			AskPrice2;							// �����۶�
	int				AskVolume2;							// ��������
	float			BidPrice3;							// �������
	int				BidVolume3;							// ��������
	float			AskPrice3;							// ��������
	int				AskVolume3;							// ��������
	float			BidPrice4;							// �������
	int				BidVolume4;							// ��������
	float			AskPrice4;							// ��������
	int				AskVolume4;							// ��������
	float			BidPrice5;							// �������
	int				BidVolume5;							// ��������
	float			AskPrice5;							// ��������
	int				AskVolume5;							// ��������
};

struct ReportData
{
	char			InstrumentID[16];					// ��Լ����
	float			LastPrice;							// ���¼�
	int				Volume;								// �ɽ���
	float			BidPrice1;							// �����һ
	int				BidVolume1;							// ������һ
	float			AskPrice1;							// ������һ
	int				AskVolume1;							// ������һ
	double			OpenInterest;						// �ֲ���
	float			OpenPrice;							// ����
	float			HighestPrice;						// ��߼�
	float			LowestPrice;						// ��ͼ�
	float			SettlementPrice;					// �����
	float			PreSettlementPrice;					// �����
	float			PreClosePrice;						// ������
};

struct KlineInfo{
	//��Լ����
	char InstrumentID[16];
	//��߼�
	float maxPrice;
	//��ͼ�
	float lowPrice;
	//����
	float openPrice;
	//�ɽ���
	int Volume;
	//����
	float closePrice;
	//�ֲ���
	double openInterst;
	//UNIX ʱ��
	int time;

	float LementPrice; 
};

//��¼ǰһ�ε�ֵ
struct pre_Value{
	
	//������һ�γɽ���
	int pre_Volume;

	//������һ���ӳɽ���
	int pre_min_Volume;

	//��һ�θ������ݿ��ʱ��
	int updateTime;

	//֮ǰ���ܵļ�Ȩ����ֵ
	int pre_total_Lem;

};


char* SubStr(char* str,int a,int b);
bool Zero(char* str);
PACKAGE_BQUOTE* getUDPData(char* buf);
int hexToData(char* buf,int* pf);
float hexToData(char* buf,float* pf);
WORD hexToData(char* buf,WORD* pf);
double hexToData(char* buf,double* pf);

//����K��ͼ����
void GetKline();


static hash_map<string,pre_Value*>preValue;
static hash_map<string,KlineInfo*>m_kline;

static hash_map<string,queue<PACKAGE_BQUOTE*>>m_pack;


#endif




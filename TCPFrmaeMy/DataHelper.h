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
	char			InstrumentID[16];					// 合约代码
	char			TradingDay[11];						// 交易日
	char			UpdateTime[13];						// 最后修改时间 两种格式 hh:mm:ss 和 hh:mm:ss xxx
	float			PreSettlementPrice;					// 昨结算
	float			PreClosePrice;						// 昨收盘
	double			PreOpenInterest;					// 昨持仓量
	float			OpenPrice;							// 今开盘
	float			HighestPrice;						// 最高价
	float			LowestPrice;						// 最低价
	float			ClosePrice;							// 今收盘
	float			SettlementPrice;					// 今结算
	float			LastPrice;							// 最新价
	int				Volume;								// 成交量
	double			OpenInterest;						// 持仓量
	float			BidPrice1;							// 申买价一
	int				BidVolume1;							// 申买量一
	float			AskPrice1;							// 申卖价一
	int				AskVolume1;							// 申卖量一
	float			BidPrice2;							// 申买价二
	int				BidVolume2;							// 申买量二
	float			AskPrice2;							// 申卖价二
	int				AskVolume2;							// 申卖量二
	float			BidPrice3;							// 申买价三
	int				BidVolume3;							// 申买量三
	float			AskPrice3;							// 申卖价三
	int				AskVolume3;							// 申卖量三
	float			BidPrice4;							// 申买价四
	int				BidVolume4;							// 申买量四
	float			AskPrice4;							// 申卖价四
	int				AskVolume4;							// 申卖量四
	float			BidPrice5;							// 申买价五
	int				BidVolume5;							// 申买量五
	float			AskPrice5;							// 申卖价五
	int				AskVolume5;							// 申卖量五
};

struct ReportData
{
	char			InstrumentID[16];					// 合约代码
	float			LastPrice;							// 最新价
	int				Volume;								// 成交量
	float			BidPrice1;							// 申买价一
	int				BidVolume1;							// 申买量一
	float			AskPrice1;							// 申卖价一
	int				AskVolume1;							// 申卖量一
	double			OpenInterest;						// 持仓量
	float			OpenPrice;							// 今开盘
	float			HighestPrice;						// 最高价
	float			LowestPrice;						// 最低价
	float			SettlementPrice;					// 今结算
	float			PreSettlementPrice;					// 昨结算
	float			PreClosePrice;						// 昨收盘
};

struct KlineInfo{
	//合约名称
	char InstrumentID[16];
	//最高价
	float maxPrice;
	//最低价
	float lowPrice;
	//开盘
	float openPrice;
	//成交量
	int Volume;
	//收盘
	float closePrice;
	//持仓量
	double openInterst;
	//UNIX 时间
	int time;

	float LementPrice; 
};

//记录前一次的值
struct pre_Value{
	
	//保存上一次成交量
	int pre_Volume;

	//保存上一分钟成交量
	int pre_min_Volume;

	//上一次更新数据库的时间
	int updateTime;

	//之前的总的加权结算值
	int pre_total_Lem;

};


char* SubStr(char* str,int a,int b);
bool Zero(char* str);
PACKAGE_BQUOTE* getUDPData(char* buf);
int hexToData(char* buf,int* pf);
float hexToData(char* buf,float* pf);
WORD hexToData(char* buf,WORD* pf);
double hexToData(char* buf,double* pf);

//解析K线图数据
void GetKline();


static hash_map<string,pre_Value*>preValue;
static hash_map<string,KlineInfo*>m_kline;

static hash_map<string,queue<PACKAGE_BQUOTE*>>m_pack;


#endif




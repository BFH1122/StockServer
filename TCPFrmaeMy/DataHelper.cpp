#include "DataHelper.h"
#include <iomanip>
#include <string>
using namespace std;
PACKAGE_BQUOTE* getUDPData(char* buf)
{
	const int flagF = 39084;
	const int typeF = 4097;
	char* temp = SubStr(buf,0,1);
	WORD date;
	hexToData(temp,&date);
	delete temp;

	temp = SubStr(buf,2,3);
	WORD type;
	hexToData(temp,&type);
	delete temp;

	temp = SubStr(buf,4,29);
	bool zeroFlag = Zero(temp);
	delete temp;


	if (date != flagF || type != typeF ||!zeroFlag)
	{
		PACKAGE_BQUOTE* package = (PACKAGE_BQUOTE*)malloc(sizeof(PACKAGE_BQUOTE));
		package->flag = false;
		return package;
	}

	PACKAGE_BQUOTE* package = (PACKAGE_BQUOTE*)malloc(sizeof(PACKAGE_BQUOTE));
	package->flag = true;

	temp = SubStr(buf,30,31);
	WORD length;
	hexToData(temp,&length);
	delete temp;

	//合约代码InstrumentID[16]
	temp = SubStr(buf,32,47);
	strcpy(package->InstrumentID,temp);
	delete temp;

	// 交易日TradingDay[9]
	//temp = SubStr(buf,48,56);
	//strcpy(package->TradingDay,temp);

	char TradingDayBuf[11];
	memset(TradingDayBuf, '\0', 11);
	for (int i=0,j=0;i<11;i++,j++)
	{
		if (4==i||7==i)
		{
			TradingDayBuf[i]='-';
			i++;
		}
		TradingDayBuf[i]=buf[48+j];
	}
	strcpy(package->TradingDay,TradingDayBuf);




	//delete temp;
	// 修改时间UpdateTime[13]
	temp = SubStr(buf,57,65);
	strcpy(package->UpdateTime,temp);
	delete temp;

	// 昨结算float
	temp = SubStr(buf,70,73);
	float PreSettlementPrice;
	hexToData(temp,&PreSettlementPrice);
	delete temp;
	package->PreSettlementPrice = PreSettlementPrice;
	// 昨收盘
	temp = SubStr(buf,74,77);
	float PreClosePrice;
	hexToData(temp,&PreClosePrice);	
	delete temp;
	package->PreClosePrice = PreClosePrice;
	// 昨持仓量
	temp = SubStr(buf,78,85);
	double PreOpenInterest;
	hexToData(temp,&PreOpenInterest);
	delete temp;
	package->PreOpenInterest = PreOpenInterest;					
	// 今开盘
	temp = SubStr(buf,86,89);
	float OpenPrice;
	hexToData(temp,&OpenPrice);
	delete temp;
	package->OpenPrice = OpenPrice;							
	// 最高价
	temp = SubStr(buf,90,93);
	float HighestPrice;
	hexToData(temp,&HighestPrice);
	delete temp;
	package->HighestPrice = HighestPrice;
	// 最低价
	temp = SubStr(buf,94,97);
	float LowestPrice;
	hexToData(temp,&LowestPrice);
	delete temp;
	package->LowestPrice=LowestPrice;
	// 今收盘
	temp = SubStr(buf,98,101);
	float ClosePrice;
	hexToData(temp,&ClosePrice);	
	delete temp;
	package->ClosePrice = ClosePrice;
	// 今结算
	temp = SubStr(buf,102,105);
	float SettlementPrice;
	hexToData(temp,&SettlementPrice);	
	delete temp;
	package->SettlementPrice = SettlementPrice;
	// 最新价
	temp = SubStr(buf,106,109);
	float LastPrice;
	hexToData(temp,&LastPrice); 
	delete temp;
	package->LastPrice = LastPrice;
	// 成交量
	temp = SubStr(buf,110,113);
	int	Volume;
	hexToData(temp,&Volume);
	delete temp;
	package->Volume = Volume;

	// 持仓量
	temp = SubStr(buf,114,121);
	double OpenInterest;
	hexToData(temp,&OpenInterest);
	delete temp;
	package->OpenInterest = OpenInterest;

	// 申买价一
	temp = SubStr(buf,122,125);
	float BidPrice1;
	hexToData(temp,&BidPrice1);	
	delete temp;
	package->BidPrice1 = BidPrice1;
	// 申买量一
	temp = SubStr(buf,126,129);
	int	BidVolume1;
	hexToData(temp,&BidVolume1);
	delete temp;
	package->BidVolume1=BidVolume1;

	// 申卖价一
	temp = SubStr(buf,130,133);
	float AskPrice1;
	hexToData(temp,&AskPrice1);
	delete temp;
	package->AskPrice1 = AskPrice1;

	// 申卖量一
	temp = SubStr(buf,134,137);
	int AskVolume1;
	hexToData(temp,&AskVolume1);
	delete temp;
	package->AskVolume1 = AskVolume1;

	// 申买价二
	temp = SubStr(buf,138,141);
	float BidPrice2;
	hexToData(temp,&BidPrice2);
	delete temp;
	package->BidPrice2 = BidPrice2;

	// 申买量二
	temp = SubStr(buf,142,145);
	int BidVolume2;
	hexToData(temp,&BidVolume2);	
	delete temp;
	package->BidVolume2 = BidVolume2;

	// 申卖价二
	temp = SubStr(buf,146,149);
	float AskPrice2;
	hexToData(temp,&AskPrice2);
	delete temp;
	package->AskPrice2 = AskPrice2;

	// 申卖量二
	temp = SubStr(buf,150,153);
	int AskVolume2;
	hexToData(temp,&AskVolume2);
	delete temp;
	package->AskVolume2 = AskVolume2;

	// 申买价三
	temp = SubStr(buf,154,157);
	float BidPrice3;
	hexToData(temp,&BidPrice3);
	delete temp;
	package->BidPrice3 = BidPrice3;

	// 申买量三
	temp = SubStr(buf,158,161);
	int BidVolume3;
	hexToData(temp,&BidVolume3);	
	delete temp;
	package->BidVolume3 = BidVolume3;

	// 申卖价三
	temp = SubStr(buf,162,165);
	float AskPrice3;
	hexToData(temp,&AskPrice3);
	delete temp;
	package->AskPrice3 = AskPrice3;

	// 申卖量三
	temp = SubStr(buf,166,169);
	int AskVolume3;
	hexToData(temp,&AskVolume3);	
	delete temp;
	package->AskVolume3 = AskVolume3;

	// 申买价四
	temp = SubStr(buf,170,173);
	float BidPrice4;
	hexToData(temp,&BidPrice4);
	delete temp;
	package->BidPrice4 = BidPrice4;

	// 申买量四
	temp = SubStr(buf,174,177);
	int	 BidVolume4;
	hexToData(temp,&BidVolume4);
	delete temp;
	package->BidVolume4 = BidVolume4;

	// 申卖价四
	temp = SubStr(buf,178,181);
	float AskPrice4;
	hexToData(temp,&AskPrice4);
	delete temp;
	package->AskPrice4 = AskPrice4;

	// 申卖量四
	temp = SubStr(buf,182,185);
	int AskVolume4;
	hexToData(temp,&AskVolume4);
	delete temp;
	package->AskVolume4 = AskVolume4;

	// 申买价五
	temp = SubStr(buf,186,189);
	float BidPrice5;
	hexToData(temp,&BidPrice5);
	delete temp;
	package->BidPrice5 = BidPrice5;

	// 申买量五
	temp = SubStr(buf,190,193);
	int BidVolume5;
	hexToData(temp,&BidVolume5);
	delete temp;
	package->BidVolume5 = BidVolume5;

	// 申卖价五
	temp = SubStr(buf,194,197);
	float AskPrice5;
	hexToData(temp,&AskPrice5);
	delete temp;
	package->AskPrice5 = AskPrice5;

	// 申卖量五
	temp = SubStr(buf,198,201);
	int AskVolume5;
	hexToData(temp,&AskVolume5);	
	delete temp;
	package->AskVolume5 = AskVolume5;

	return package;
}
char* SubStr(char* str,int a,int b)
{
	//char* temp = NULL;
	char* temp = new char[b-a+2];
	for (int i = a; i<=b;i++)
	{
		temp[i-a]=str[i];

	}
	temp[b-a+1]='\0';
	return temp;
}
double hexToData(char* buf,double* pf)
{
	for (int i = 0; i<sizeof(double);i++)
	{
		*(( char*)pf+i)=*(buf+i); 
	}
	return *pf;
}
float hexToData(char* buf,float* pf)
{
	for (int i = 0; i<sizeof(float);i++)
	{
		*(( char*)pf+i)=*(buf+i); 
	}
	return *pf;
}
int hexToData(char* buf,int* pf)
{
	for (int i = 0; i<sizeof(buf);i++)
	{
		*(( char*)pf+i)=*(buf+i); 
	}
	return *pf;
}
WORD hexToData(char* buf,WORD* pf)
{
	for (int i = 0; i<sizeof(WORD);i++)
	{
		*(( char*)pf+i)=*(buf+i); 
	}
	return *pf;
}

bool Zero(char* str)
{
	int n = 26;
	for (int i = 0; i < n;i++)
	{
		if (str[i] != 0)
		{
			return false;
		}
	}
	return true;
}

//bool Zero(char* str)
//{
//	int n = strlen(str);
//	for (int i = 0; i < n;i++)
//	{
//		if (str[i] != '0')
//		{
//			return false;
//		}
//	}
//	return true;
//}


//解析K线图数据
void GetKline(PACKAGE_BQUOTE* pack){

}
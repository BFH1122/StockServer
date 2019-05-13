#include "MyUDPTask.h"
#include "UDPHelpr.h"
#include "TimeHelper.h"
#include <sstream>

ReportData* GetReport(PACKAGE_BQUOTE* pack);
PACKAGE_BQUOTE* testMethod();
PACKAGE_BQUOTE* testMethod1();
PACKAGE_BQUOTE* testMethod2();
PACKAGE_BQUOTE* testMethod3();
//------------------UDPTask-------------------------

hash_map<string,string> UDPTask::m_packs;

UDPTask::UDPTask(){

}
UDPTask::~UDPTask(){

}

void UDPTask::taskProc(){
	udpInfo *info = (udpInfo*)getData();
	
	UDPHelper *helper = info->udphelper;
	
	PACKAGE_BQUOTE* pack = getUDPData(info->buf);

	//PACKAGE_BQUOTE* pack;
	//if(strcmp(info->buf,"a")==0){
	//	pack = testMethod();
	//}else if(strcmp(info->buf,"b")==0){
	//	pack = testMethod1();
	//}else if(strcmp(info->buf,"c")==0){
	//	pack = testMethod3();
	//}
	//else{
	//	pack = testMethod2();
	//}

	//释放内存操作，不能乱动
	if(!pack->flag){
		delete info->buf;
		info->buf = NULL;
		info->udphelper =NULL;
		free(pack);
		delete this;
		return;
	}
	//计算K线值

	//当前包传递的时间值
	std::string timeStr= (std::string)pack->TradingDay+" "+(std::string)pack->UpdateTime;
	int updateTime = TimeHelper::strTimeUnix(timeStr);

	//当前包对应的历史数据
	pre_Value *tmp_pre_Value;

	//没有历史数据
	if( preValue.find(pack->InstrumentID)==preValue.end()){
		//初始化历史数据 --- 当前值
		pre_Value *info = new pre_Value ;
		info->pre_min_Volume = -1;
		info->pre_Volume = 0;
		info->updateTime = updateTime;
		preValue[pack->InstrumentID] = info;

	}

	//获得上一次成交量的信息
	tmp_pre_Value = preValue[pack->InstrumentID];

	//printf("Name: %s   %s \n",pack->InstrumentID,timeStr.c_str());

	//比较时间，判断是否一分钟之内的
	if(updateTime-tmp_pre_Value->updateTime > 59){
		//更新数据库
		if(m_kline.find(pack->InstrumentID)!=m_kline.end()){
			KlineInfo *mtmp_Kline = m_kline[pack->InstrumentID];
			if(tmp_pre_Value->pre_min_Volume >= 0){
				std::ostringstream oss;
				std::string str1 = "insert into kline(InstrumentID,Timestr,OpenPrice,HighestPrice,LowestPrice,ClosePrice,Volume,OpenInterest,LementPrice)VALUES((SELECT Instrument FROM instrument WHERE InstrumentName =";
				std::string str2="),";
				oss<<str1<<"'"<<pack->InstrumentID<<"'"<<str2;
				oss<<tmp_pre_Value->updateTime<<",'"<<mtmp_Kline->openPrice<<"','"<<mtmp_Kline->maxPrice<<"','"<<mtmp_Kline->lowPrice<<"','"<<mtmp_Kline->closePrice<<"','"<<mtmp_Kline->Volume<<"','"<<mtmp_Kline->openInterst<<"','"<<mtmp_Kline->LementPrice<<"')";
				std::string  sqlBuf = oss.str();
				//printf("%s进行一次数据库更新 %d\n",pack->InstrumentID,preValue.size());
				UDPHelper::udp_dbHelper->UpdateData(sqlBuf.c_str());
			}
			//记录这一分钟最新价
			mtmp_Kline->openPrice = pack->LastPrice;
			mtmp_Kline->maxPrice = pack->LastPrice;
			mtmp_Kline->lowPrice = pack->LastPrice;
		}

		//更新时间
		tmp_pre_Value->updateTime = updateTime;
		//pack->volume 是这一分钟起始成交量，，则tmp_pre_value->pre_volume是上一分钟最后的成交量
		tmp_pre_Value->pre_min_Volume = tmp_pre_Value->pre_Volume;

		tmp_pre_Value->pre_total_Lem = 0;
	}

	KlineInfo *tmp_Kline;
	//LeaveCriticalSection(&info->g_cs);
	if(m_kline.find(pack->InstrumentID)==m_kline.end()){
		tmp_Kline = new KlineInfo;
		//初始化最大，最小值，否则出错
		tmp_Kline->lowPrice = 1000000;
		tmp_Kline->maxPrice = -1;
		tmp_Kline->openPrice = pack->LastPrice;
		m_kline[pack->InstrumentID] = tmp_Kline;
	}

	tmp_Kline = m_kline[pack->InstrumentID];
	strcpy(tmp_Kline->InstrumentID ,pack->InstrumentID);
	tmp_Kline->time = updateTime;
	tmp_Kline->openInterst = pack->OpenInterest;

	//openPrice更新完成
	tmp_Kline->maxPrice = tmp_Kline->maxPrice > pack->LastPrice ? tmp_Kline->maxPrice : pack->LastPrice;
	tmp_Kline->lowPrice = tmp_Kline->lowPrice < pack->LastPrice ? tmp_Kline->lowPrice : pack->LastPrice;
	//更新收盘价，每一次更新，保证是最后一次接收的数据
	tmp_Kline->closePrice = pack->LastPrice; //


	//计算当前成交量 = 当前包成交量 - 上一分钟成交量
	tmp_Kline->Volume = tmp_pre_Value->pre_min_Volume == -1 ? 0:pack->Volume - tmp_pre_Value->pre_min_Volume;


	//计算结算值 加权总和
	tmp_pre_Value->pre_total_Lem += (pack->Volume - tmp_pre_Value->pre_Volume)* pack->LastPrice;

	//计算结算值   第一次肯定等于最新价
	tmp_Kline->LementPrice = tmp_Kline->Volume ==0 ? pack->LastPrice : tmp_pre_Value->pre_total_Lem / tmp_Kline->Volume;

	//更新上一次成交量
	tmp_pre_Value->pre_Volume = pack->Volume;

	//计算报表值
	ReportData *rPack = GetReport(pack);
	//转化为JSON字符形式
	std::string json;
	std::string json_2;
	DBHelper::toJSON(tmp_Kline,rPack,json);

	tmp_Kline = NULL;
	DBHelper::toJSON(tmp_Kline,rPack,json_2);

	//printf("添加\n");
	m_packs[pack->InstrumentID]=json_2;
	//发送UDP
	helper->sendTCP(json,json_2);


	//释放内存部分
	delete info->buf;
	info->udphelper =NULL;
	free(pack);
	free(rPack);
	delete this;
	return;
}

ReportData* GetReport(PACKAGE_BQUOTE* pack)
{
	ReportData* sendPack=(ReportData*)malloc(sizeof(ReportData));
	sendPack->AskPrice1=pack->AskPrice1;
	sendPack->AskVolume1=pack->AskVolume1;
	sendPack->BidPrice1=pack->BidPrice1;
	sendPack->BidVolume1=pack->BidVolume1;
	sendPack->HighestPrice=pack->HighestPrice;
	strcpy(sendPack->InstrumentID,pack->InstrumentID);
	sendPack->LastPrice=pack->LastPrice;
	sendPack->LowestPrice=pack->LowestPrice;

	sendPack->OpenInterest=pack->OpenInterest;
	sendPack->OpenPrice=pack->OpenPrice;

	sendPack->PreClosePrice=pack->PreClosePrice;

	sendPack->PreSettlementPrice=pack->PreSettlementPrice;
	sendPack->SettlementPrice=pack->SettlementPrice;
	sendPack->Volume=pack->Volume;
	return sendPack;
}

PACKAGE_BQUOTE* testMethod1(){
	//Sleep(500);
	//解析传递的数据
	PACKAGE_BQUOTE* pack = new PACKAGE_BQUOTE;
	pack->AskPrice1 = 1;
	pack->AskPrice2 = 2;
	pack->AskPrice3 = 3;
	pack->AskPrice4 = 4;
	pack->AskPrice5 = 5;
	pack->AskVolume1 = 11;
	pack->flag = true;
	strcpy(pack->TradingDay,"2019-05-05");
	strcpy(pack->UpdateTime,"13:39:40");
	strcpy(pack->InstrumentID,"IF1909");
	
	pack->OpenInterest = 14;
	
	pack->OpenPrice= 12;
	
	pack->HighestPrice = 26;
	
	pack->LowestPrice = 5;
	
	pack->ClosePrice = 10;

	pack->Volume = 70;
	
	pack->LastPrice = 25;
	
	return pack;
}


PACKAGE_BQUOTE* testMethod3(){
	//Sleep(500);
	//解析传递的数据
	PACKAGE_BQUOTE* pack = new PACKAGE_BQUOTE;

	pack->flag = true;
	strcpy(pack->TradingDay,"2019-05-05");
	strcpy(pack->UpdateTime,"13:39:20");
	strcpy(pack->InstrumentID,"IF1909");

	pack->OpenInterest = 11;
	pack->OpenPrice= 12;
	pack->HighestPrice = 25;
	pack->LowestPrice = 5;
	pack->ClosePrice = 10;

	pack->Volume = 75;
	pack->LastPrice = 16;

	return pack;
}

PACKAGE_BQUOTE* testMethod(){
	//Sleep(500);
	//解析传递的数据
	PACKAGE_BQUOTE* pack = new PACKAGE_BQUOTE;
	pack->AskPrice1 = 1;
	pack->AskPrice2 = 2;
	pack->AskPrice3 = 3;
	pack->AskPrice4 = 4;
	pack->AskPrice5 = 5;
	pack->AskVolume1 = 11;
	pack->flag = true;
	strcpy(pack->TradingDay,"2019-05-05");
	strcpy(pack->UpdateTime,"13:38:20");
	strcpy(pack->InstrumentID,"IF1909");
	pack->OpenInterest = 11;
	pack->OpenPrice= 12;
	pack->HighestPrice = 25;
	pack->LowestPrice = 5;
	pack->ClosePrice = 10;
	pack->Volume = 50;
	pack->LastPrice = 22;
	return pack;
}

PACKAGE_BQUOTE* testMethod2(){
	//Sleep(500);
	//解析传递的数据
	PACKAGE_BQUOTE* pack = new PACKAGE_BQUOTE;

	pack->flag = true;
	strcpy(pack->TradingDay,"2019-05-05");
	strcpy(pack->UpdateTime,"13:40:20");
	strcpy(pack->InstrumentID,"IF1909");

	pack->OpenInterest = 11;
	pack->OpenPrice= 12;
	pack->HighestPrice = 25;
	pack->LowestPrice = 5;
	pack->ClosePrice = 10;

	pack->Volume = 80;
	pack->LastPrice = 18;

	return pack;
}
#include "DBHelper.h"

#include <iostream>

DBHelper::DBHelper(char *usernames,char *passwords,char *dbs,char *hosts/*="127.0.0.1"*/,int ports/*=3306*/)
{
	username = usernames;
	password = passwords;
	db = dbs;
	host = hosts;
	port = ports;
	mysql_init(&m_sql);

	InitializeCriticalSection(&g_cs);

}

bool DBHelper::connectDB(){
	//连接mysql，数据库 返回false则连接失败，返回true则连接成功
	if (!(mysql_real_connect(&m_sql,host, username, password, db,port,NULL,0))) 
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool DBHelper::QueryData(const char *sql,MYSQL_RES* & res)
{
	EnterCriticalSection(&g_cs);
	if(mysql_query(&m_sql,sql))
	{
		LeaveCriticalSection(&g_cs);
		return false;
	}
	if (!(res=mysql_store_result(&m_sql)))    //获得sql语句结束后返回的结果集
	{
		LeaveCriticalSection(&g_cs);
		return false;
	}
	LeaveCriticalSection(&g_cs);
	return true;
}

bool DBHelper::UpdateData(const char *sql)
{

	EnterCriticalSection(&g_cs);
	if(mysql_query(&m_sql,sql))
	{
		LeaveCriticalSection(&g_cs);
		return false;
	}
	else
	{
		LeaveCriticalSection(&g_cs);
		return true;
	}

}

void DBHelper::closedb()
{
	mysql_close(&m_sql);
}

void DBHelper::closeRES(MYSQL_RES* & res)
{
	mysql_free_result(res);
}

void DBHelper::toJSON(MYSQL_RES* & res,std::string & json,int n,int num,char *response /* ="Kline" */)
{
	MYSQL_ROW row;


	MYSQL_FIELD *files;
	Json::Value root;

	Json::Value mroot;

	Json::FastWriter writer;
	Json::Value srow;
	Json::Value troot;

	Json::Value tmp;

	files=mysql_fetch_fields(res);//获得字段名 
	int len = mysql_num_fields(res);
	troot["Type"]=response;

	//循环遍历每一行
	while(row = mysql_fetch_row(res)){
		//循环遍历每一列
		for(int i = 0;i<len;i++){
			srow[files[i].name] = row[i];
		}
		root.append(srow);
	}
	int i = 0;
	int j = 0;
	double total = 0;

	for(;j<n;j++){
		total+=atof(root[j]["OpenPrice"].asString().c_str());
	}


	int lenn = root.size();

	for(i;i<num && i<root.size();i++,j++){
		//std::cout << i <<"   "<<j << std::endl;
		double tmp = total / n;
		root[i]["MA"] = tmp;
		if(j>root.size())
			break;
		total = total + atof(root[j]["OpenPrice"].asString().c_str()) - atof(root[i]["OpenPrice"].asString().c_str());

		mroot.append(root[i]);
	}

	troot["array"]=mroot;
	std::string str1 = troot.toStyledString();
	json = writer.write(troot);
	//std::cout<<json;
}


void DBHelper::toJSON(MYSQL_RES* & res,std::string & json,char *response){
    MYSQL_ROW row;
	MYSQL_FIELD *files;
	
	Json::Value root;
	Json::FastWriter writer;
	Json::Value srow;
	Json::Value troot;

	files=mysql_fetch_fields(res);//获得字段名 
	int len = mysql_num_fields(res);


	troot["Type"]=response;
	//循环遍历每一行
	while(row = mysql_fetch_row(res)){
		for(int i = 0;i<len;i++){
			srow[files[i].name] = row[i];
		}
		root.append(srow);
	}
	troot[response]=root;
	troot.toStyledString();
	json = writer.write(troot);
}



void DBHelper::toJSON(KlineInfo* & kPack,ReportData* rPack,std::string & json,char *response /* ="Realtime" */)
{
	Json::Value root;
	Json::FastWriter writer;
	Json::Value srow;
	Json::Value kJson;
	Json::Value rJson;

	root["Type"]=response;

	//K线图
	std::string InstrumentName(rPack->InstrumentID);			//报表的ID
	root["InstrumentName"]=InstrumentName;
	
	kJson["TimeStr"]=kPack==NULL?0:kPack->time;
	kJson["ClosePrice"]=kPack==NULL?0:kPack->closePrice;
	kJson["OpenPrice"]=kPack==NULL?0:kPack->openPrice;
	kJson["HighestPrice"]=kPack==NULL?0:kPack->maxPrice;
	kJson["LowestPrice"]=kPack==NULL?0:kPack->lowPrice;
	kJson["Volume"]=kPack==NULL?0:kPack->Volume;
	kJson["OpenInterest"]=kPack==NULL?0:kPack->openInterst;
	kJson["LementPrice"]=kPack==NULL?0:kPack->LementPrice;

	//report
	rJson["LastPrice"]=rPack->LastPrice;
	rJson["Volume"]=rPack->Volume;
	rJson["BidPrice1"]=rPack->BidPrice1;
	rJson["BidVolume1"]=rPack->BidVolume1;
	rJson["AskPrice1"]=rPack->AskPrice1;
	rJson["AskVolume1"]=rPack->AskVolume1;
	rJson["OpenPrice"]=rPack->OpenPrice;
	rJson["HighestPrice"]=rPack->HighestPrice;
	rJson["LowestPrice"]=rPack->LowestPrice;
	rJson["OpenInterest"]=rPack->OpenInterest;
	rJson["PreClosePrice"]=rPack->PreClosePrice;
	rJson["PreSettlementPrice"]=rPack->PreSettlementPrice;
	rJson["SettlementPrice"]=rPack->SettlementPrice;

	root["Kline"]=kJson;
	root["Report"]=rJson;

	root.toStyledString();
	json = writer.write(root);

}
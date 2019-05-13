#ifndef _HEADER_DB_H_
#define _HEADER_DB_H_
#include <stdio.h>
#include <mysql.h>

#include <string>
#include "DataHelper.h"

#include "json/json.h"

class DBHelper{
	//成员变量
private:
	MYSQL m_sql;

public:
	char *username;
	char *password;
	char *db;
	char *host;
	int port;

	CRITICAL_SECTION g_cs;

	//成员函数
public:
	//初始化函数
	DBHelper(char *usernames,char *passwords,char *dbs,char *hosts="127.0.0.1",int ports=3306);
	bool connectDB();

	bool QueryData(const char *sql,MYSQL_RES* & res);
	bool UpdateData(const char *sql);

	void closedb();

	void static closeRES(MYSQL_RES* & res);

	void static toJSON(MYSQL_RES* & res,std::string & json,char *response ="Kline");

	void static toJSON(MYSQL_RES* & res,std::string & json,int n,int num,char *response ="Kline");


	void static DBHelper::toJSON(KlineInfo* & kPack,ReportData* rPack,std::string & json,char *response  ="Kline" );

};


#endif

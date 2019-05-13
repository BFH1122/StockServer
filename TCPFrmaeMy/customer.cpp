
#include "stdafx.h"
#include "customer.h"
#include "TCPHelper.h"

//JSONHelper
#include "json/json.h"

#include "DBHelper.h"

#include "UDPHelpr.h"

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <cstring>
#include <sstream>


//只提供客户端请求K线图的服务
DWORD WINAPI custom::deal_with(LPVOID lpParameter){
	//客户端第一次进入服务器
	dbHelper = new DBHelper("root","root","mydb");
	//保存查询数据的结果
	std::string str = "";
	MySocket *my_socket = (MySocket *)lpParameter;
	UDPHelper *udpHelper = my_socket->udpHelper;
	RecMessage *netMessage = new RecMessage();
	std::cout << "客户端地址:" << inet_ntoa(my_socket->addrClient.sin_addr) << ":" << ntohs(my_socket->addrClient.sin_port) << std::endl;

	while(true){

		//意外断开的情况
		if(!my_socket->recInfo(netMessage)){
			std::cout << "客户端强行退出:" << inet_ntoa(my_socket->addrClient.sin_addr) << ":" << ntohs(my_socket->addrClient.sin_port) << std::endl;
			break;
		}
		if(strcmp(netMessage->content,"exit")==0){
			break;
		}
		//以上语句保证客户端与服务端正确的断开


		std::cout << "接收到的数据类型" << netMessage->_type <<std::endl;
		std::cout << "接收到的数据长度" << netMessage->_length <<std::endl;
		std::cout << "接收到的数据内容" << netMessage->content <<std::endl;
		
		//进行业务服务的逻辑
		parseJson(netMessage->content,my_socket);
		/*dbHelper->connectDB();
		dbHelper->QueryData("select Kline.InstrumentID,InstrumentName,Timestr,OpenPrice,HighestPrice,LowestPrice,ClosePrice,Volume,OpenInterest,LementPrice from Kline,Instrument where Kline.InstrumentID = Instrument.InstrumentID;",res);
		DBHelper::toJSON(res,str);
		DBHelper::closeRES(res);
		dbHelper->closedb();
		std::cout <<"查询的结果：" <<str <<std::endl;
		my_socket->sendInfo(str,"JSON");*/

	}

	std::cout << "客户端安全退出:" << inet_ntoa(my_socket->addrClient.sin_addr) << ":" << ntohs(my_socket->addrClient.sin_port) << std::endl;
	dbHelper->closedb();
	//**重要**发送给服务器断开指令 否则CPU 沾满
	udpHelper->removeQuene(my_socket);
	////关闭对应的socket
	closesocket(my_socket->socket_m);
	////退出，意味着线程的结束，socket 断开连接
	return 0;
}

void custom::parseJson(char* & str,MySocket* & my_socket){
	const std::string LOGIN ="Login";
	const std::string KLINE ="Kline";
	const std::string REPORT ="Report";
	Json::Value value;

	//登录命令返回的两种JSON数据
	std::string res[2] = {"{\"Type\":\"Login\",\"Response\":\"Fail\"}","{\"Type\":\"Login\",\"Response\":\"Success\"}"};

	// 开始解析json，解析后的json对象存储在value中，成功返回true
	Json::Reader reader;
	if (reader.parse(str, value))
	{
		// 字段key1
		if (value.isMember("Type") && value["Type"].isString())
		{
			std::string strTemp = value["Type"].asString();
			
			//解析登录命令逻辑函数
			if (strTemp.compare(LOGIN)==0){
				std::string name = value["User"].asString();
				std::string password = value["Password"].asString();
				//进行数据库认证				
				int flag = 0;
				if(strcmp("admin",password.c_str())==0 && strcmp(name.c_str(),"admin")==0)
					flag = 1;
				my_socket->sendInfo(res[flag==1?1:0],"JSON",80);
				//将登录标致设为1，可以发送UDP数据报
				my_socket->login_flag = flag==1?1:0;
				//
			}

			else if(my_socket->login_flag == 1)
			{
				//解析请求K线图数据逻辑函数
				if (strTemp.compare(KLINE)==0){
				//根据时间，点个数，完成数据库查询操作，然后进行JSON数据的返回
					std::cout << "客户端:" << inet_ntoa(my_socket->addrClient.sin_addr) << ":" << ntohs(my_socket->addrClient.sin_port) << "   请求K线数据" <<std::endl;
					MYSQL_RES* res;
					std::string str_result = "";
					dbHelper->connectDB();

					//解析传递的参数
					std::string InstrumentName = value["IDName"].asString();
					int endTime = value["EndTime"].asInt();
					int number = value["Num"].asInt();
					int n = value["MA"].asInt();

					//构造sql语句
					std::ostringstream oss;
					std::string str1 = "SELECT Ins.InstrumentName,k.Timestr,k.OpenPrice,k.HighestPrice,k.LowestPrice,k.ClosePrice,k.Volume,k.OpenInterest,k.LementPrice FROM kline AS k JOIN instrument AS Ins ON k.InstrumentID = Ins.Instrument WHERE  Ins.InstrumentName='";
					std::string str2="' AND k.Timestr <= ";
					std::string str3 =" ORDER BY k.Timestr DESC LIMIT ";
					oss<<str1<<InstrumentName<<str2<<endTime<<str3<<(number+n);
					std::string  sqlBuf = oss.str();

					//查询数据库，构造sql
					dbHelper->QueryData(sqlBuf.c_str(),res);

					DBHelper::toJSON(res,str_result,n,number);
				
					DBHelper::closeRES(res);
				
					//发送TCP消息
					my_socket->sendInfo(str_result,"JSON",250);	

					//printf("%s");

				}
				//客户端传递的命令在服务端没有对应的情况下
				else{
					std::cout << "客户端:" << inet_ntoa(my_socket->addrClient.sin_addr) << ":" << ntohs(my_socket->addrClient.sin_port) << " 命令解析不成功" <<std::endl;
				}
			}
			else
			{
				std::cout << "客户端:" << inet_ntoa(my_socket->addrClient.sin_addr) << ":" << ntohs(my_socket->addrClient.sin_port) << " 没有进行登录" <<std::endl;
			}
		}
	}
	
	else{
		std::cout << "客户端:" << inet_ntoa(my_socket->addrClient.sin_addr) << ":" << ntohs(my_socket->addrClient.sin_port) << " 命令解析不成功" <<std::endl;
	}
}
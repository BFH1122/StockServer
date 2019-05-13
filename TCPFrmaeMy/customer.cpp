
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


//ֻ�ṩ�ͻ�������K��ͼ�ķ���
DWORD WINAPI custom::deal_with(LPVOID lpParameter){
	//�ͻ��˵�һ�ν��������
	dbHelper = new DBHelper("root","root","mydb");
	//�����ѯ���ݵĽ��
	std::string str = "";
	MySocket *my_socket = (MySocket *)lpParameter;
	UDPHelper *udpHelper = my_socket->udpHelper;
	RecMessage *netMessage = new RecMessage();
	std::cout << "�ͻ��˵�ַ:" << inet_ntoa(my_socket->addrClient.sin_addr) << ":" << ntohs(my_socket->addrClient.sin_port) << std::endl;

	while(true){

		//����Ͽ������
		if(!my_socket->recInfo(netMessage)){
			std::cout << "�ͻ���ǿ���˳�:" << inet_ntoa(my_socket->addrClient.sin_addr) << ":" << ntohs(my_socket->addrClient.sin_port) << std::endl;
			break;
		}
		if(strcmp(netMessage->content,"exit")==0){
			break;
		}
		//������䱣֤�ͻ�����������ȷ�ĶϿ�


		std::cout << "���յ�����������" << netMessage->_type <<std::endl;
		std::cout << "���յ������ݳ���" << netMessage->_length <<std::endl;
		std::cout << "���յ�����������" << netMessage->content <<std::endl;
		
		//����ҵ�������߼�
		parseJson(netMessage->content,my_socket);
		/*dbHelper->connectDB();
		dbHelper->QueryData("select Kline.InstrumentID,InstrumentName,Timestr,OpenPrice,HighestPrice,LowestPrice,ClosePrice,Volume,OpenInterest,LementPrice from Kline,Instrument where Kline.InstrumentID = Instrument.InstrumentID;",res);
		DBHelper::toJSON(res,str);
		DBHelper::closeRES(res);
		dbHelper->closedb();
		std::cout <<"��ѯ�Ľ����" <<str <<std::endl;
		my_socket->sendInfo(str,"JSON");*/

	}

	std::cout << "�ͻ��˰�ȫ�˳�:" << inet_ntoa(my_socket->addrClient.sin_addr) << ":" << ntohs(my_socket->addrClient.sin_port) << std::endl;
	dbHelper->closedb();
	//**��Ҫ**���͸��������Ͽ�ָ�� ����CPU մ��
	udpHelper->removeQuene(my_socket);
	////�رն�Ӧ��socket
	closesocket(my_socket->socket_m);
	////�˳�����ζ���̵߳Ľ�����socket �Ͽ�����
	return 0;
}

void custom::parseJson(char* & str,MySocket* & my_socket){
	const std::string LOGIN ="Login";
	const std::string KLINE ="Kline";
	const std::string REPORT ="Report";
	Json::Value value;

	//��¼����ص�����JSON����
	std::string res[2] = {"{\"Type\":\"Login\",\"Response\":\"Fail\"}","{\"Type\":\"Login\",\"Response\":\"Success\"}"};

	// ��ʼ����json���������json����洢��value�У��ɹ�����true
	Json::Reader reader;
	if (reader.parse(str, value))
	{
		// �ֶ�key1
		if (value.isMember("Type") && value["Type"].isString())
		{
			std::string strTemp = value["Type"].asString();
			
			//������¼�����߼�����
			if (strTemp.compare(LOGIN)==0){
				std::string name = value["User"].asString();
				std::string password = value["Password"].asString();
				//�������ݿ���֤				
				int flag = 0;
				if(strcmp("admin",password.c_str())==0 && strcmp(name.c_str(),"admin")==0)
					flag = 1;
				my_socket->sendInfo(res[flag==1?1:0],"JSON",80);
				//����¼������Ϊ1�����Է���UDP���ݱ�
				my_socket->login_flag = flag==1?1:0;
				//
			}

			else if(my_socket->login_flag == 1)
			{
				//��������K��ͼ�����߼�����
				if (strTemp.compare(KLINE)==0){
				//����ʱ�䣬�������������ݿ��ѯ������Ȼ�����JSON���ݵķ���
					std::cout << "�ͻ���:" << inet_ntoa(my_socket->addrClient.sin_addr) << ":" << ntohs(my_socket->addrClient.sin_port) << "   ����K������" <<std::endl;
					MYSQL_RES* res;
					std::string str_result = "";
					dbHelper->connectDB();

					//�������ݵĲ���
					std::string InstrumentName = value["IDName"].asString();
					int endTime = value["EndTime"].asInt();
					int number = value["Num"].asInt();
					int n = value["MA"].asInt();

					//����sql���
					std::ostringstream oss;
					std::string str1 = "SELECT Ins.InstrumentName,k.Timestr,k.OpenPrice,k.HighestPrice,k.LowestPrice,k.ClosePrice,k.Volume,k.OpenInterest,k.LementPrice FROM kline AS k JOIN instrument AS Ins ON k.InstrumentID = Ins.Instrument WHERE  Ins.InstrumentName='";
					std::string str2="' AND k.Timestr <= ";
					std::string str3 =" ORDER BY k.Timestr DESC LIMIT ";
					oss<<str1<<InstrumentName<<str2<<endTime<<str3<<(number+n);
					std::string  sqlBuf = oss.str();

					//��ѯ���ݿ⣬����sql
					dbHelper->QueryData(sqlBuf.c_str(),res);

					DBHelper::toJSON(res,str_result,n,number);
				
					DBHelper::closeRES(res);
				
					//����TCP��Ϣ
					my_socket->sendInfo(str_result,"JSON",250);	

					//printf("%s");

				}
				//�ͻ��˴��ݵ������ڷ����û�ж�Ӧ�������
				else{
					std::cout << "�ͻ���:" << inet_ntoa(my_socket->addrClient.sin_addr) << ":" << ntohs(my_socket->addrClient.sin_port) << " ����������ɹ�" <<std::endl;
				}
			}
			else
			{
				std::cout << "�ͻ���:" << inet_ntoa(my_socket->addrClient.sin_addr) << ":" << ntohs(my_socket->addrClient.sin_port) << " û�н��е�¼" <<std::endl;
			}
		}
	}
	
	else{
		std::cout << "�ͻ���:" << inet_ntoa(my_socket->addrClient.sin_addr) << ":" << ntohs(my_socket->addrClient.sin_port) << " ����������ɹ�" <<std::endl;
	}
}
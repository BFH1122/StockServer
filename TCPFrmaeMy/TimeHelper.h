#ifndef _TIMEHELPER_H_
#define _TIMEHELPER_H_

#include <stdio.h> 
#include <memory.h>
#include <iostream>  
#include <ctime>
#include <string>


class TimeHelper{
public:
	//�ַ���ʱ�䣨����ʱ�䣩תUNIXʱ��
	static time_t strTimeUnix(std::string strTime);


	std::string TimeHelper::UnixTostr(time_t time,int flag=0);
	//UNIX ʱ��ת����ʱ��
};


#endif

#ifndef _TIMEHELPER_H_
#define _TIMEHELPER_H_

#include <stdio.h> 
#include <memory.h>
#include <iostream>  
#include <ctime>
#include <string>


class TimeHelper{
public:
	//字符串时间（正常时间）转UNIX时间
	static time_t strTimeUnix(std::string strTime);


	std::string TimeHelper::UnixTostr(time_t time,int flag=0);
	//UNIX 时间转正常时间
};


#endif

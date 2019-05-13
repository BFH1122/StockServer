#include "TimeHelper.h"

//
//std::string TimeHelper::UnixTostr(time_t time){
//	struct tm tm;
//	tm = *localtime(&time);
//	char s[64];
//	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
//	return s;
//}

time_t TimeHelper::strTimeUnix(std::string strTime){
	struct tm tms;
	memset(&tms, 0, sizeof(tm));  
	sscanf(strTime.c_str(), "%d-%d-%d %d:%d",   
		&tms.tm_year, &tms.tm_mon, &tms.tm_mday,  
		&tms.tm_hour, &tms.tm_min/*, &tms.tm_sec*/);  
	tms.tm_year -= 1900;  
	tms.tm_mon--;  
	return mktime(&tms);  
}

std::string TimeHelper::UnixTostr(time_t time,int flag){
	struct tm tm1;
	tm1 = *localtime(&time);
	char s[64];
	if(flag == 0)
		tm1.tm_sec=0;
	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm1);
	return s;
}
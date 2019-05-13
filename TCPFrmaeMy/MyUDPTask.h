#ifndef _UDPTASK_HEADER_H_
#define _UDPTASK_HEADER_H_

#include "CTask.h"
//启动UDP的任务信息
#include<string>
#include <hash_map>
//#include <Windows.h>

using namespace std;
using namespace stdext;

class UDPTask:public CTask{
public:
	static hash_map<string,string> m_packs;

public:
	UDPTask();
	~UDPTask();

public:
	virtual void taskProc();
};

#endif

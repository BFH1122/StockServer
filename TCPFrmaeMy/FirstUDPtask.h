#ifndef _FUDPTASK_HEADER_H_
#define _FUDPTASK_HEADER_H_


//客户端第一次连接时 发送所有的报表数据
#include "CTask.h"
//启动UDP的任务信息


class FUDPTask:public CTask{
public:
	FUDPTask();
	~FUDPTask();

public:
	virtual void taskProc();
};

#endif
#ifndef _FUDPTASK_HEADER_H_
#define _FUDPTASK_HEADER_H_


//�ͻ��˵�һ������ʱ �������еı�������
#include "CTask.h"
//����UDP��������Ϣ


class FUDPTask:public CTask{
public:
	FUDPTask();
	~FUDPTask();

public:
	virtual void taskProc();
};

#endif
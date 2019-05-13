#include "CTask.h"
#include <stdio.h>


CTask::CTask(){
}

CTask::~CTask(){
	//printf("调用析构函数\n");
	delete m_data;
}

void* CTask::getData(){
	return m_data;
}

void CTask::setData(void* & data){
	m_data = data;
}
#include "CTask.h"
#include <stdio.h>


CTask::CTask(){
}

CTask::~CTask(){
	//printf("������������\n");
	delete m_data;
}

void* CTask::getData(){
	return m_data;
}

void CTask::setData(void* & data){
	m_data = data;
}
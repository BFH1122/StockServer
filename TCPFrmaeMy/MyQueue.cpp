#include "MyQueue.h"
#include"task.h"

#include <stdio.h>

CMyQueue::CMyQueue(void)
{
}


CMyQueue::~CMyQueue(void)
{
}

CTask* CMyQueue::pop()
{
	CTask*p=NULL;
	m_mutex.Lock();
	//printf("任务队列取之前 %d %d %d\n",getTaskQueneSize(),sizeof(m_TaskQueue),sizeof(*p));
	if(!m_TaskQueue.empty())
	{
		p=m_TaskQueue.front();
		m_TaskQueue.pop_front();

	}
	m_mutex.Unlock();
	//printf("任务队列取之后 %d %d %d\n",getTaskQueneSize(),sizeof(m_TaskQueue),sizeof(*p));
	return p;
}

bool CMyQueue::push(CTask* & t)
{
	if(!t)
	{
		return false;
	}
	m_mutex.Lock();
	m_TaskQueue.push_back(t);
	m_mutex.Unlock();
	return true;
}

int CMyQueue::getTaskQueneSize(){
	int len;
	m_mutex.Lock();
	len=m_TaskQueue.size();
	m_mutex.Unlock();
	return len;
}

bool CMyQueue::isEmpty()
{
	bool ret=false;
	m_mutex.Lock();
	ret=m_TaskQueue.empty();
	m_mutex.Unlock();
	return ret;
}

bool CMyQueue::pushFront(CTask* & t )
{
	if(!t)
	{
		return false;
	}
	m_mutex.Lock();
	m_TaskQueue.push_front(t);
	m_mutex.Unlock();
	return true;
}

bool CMyQueue::clear()
{
	m_mutex.Lock();
// 	std::deque<CTask*>::iterator iter=m_TaskQueue.begin();
// 	for(;iter!=m_TaskQueue.end();iter++)
// 	{
// 		delete (*iter);
// 	}
	m_TaskQueue.clear();
	m_mutex.Unlock();
	return true;
}

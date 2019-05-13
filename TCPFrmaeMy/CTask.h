#pragma once

class CTask
{

public:
	CTask();
	~CTask(void);
public:
	virtual void taskProc()=0;
	void setData(void* & data);
	void* getData();
private:
	void* m_data;
};

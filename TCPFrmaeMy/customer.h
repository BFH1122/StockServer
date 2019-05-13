#ifndef _HEADER_H_CUSTOM_
#define _HEADER_H_CUSTOM_
#include "base.h"
#include "DBHelper.h"
#include "MySocket.h"

class custom:public base{
public:
	DBHelper *dbHelper;
	DWORD WINAPI deal_with(LPVOID lpParameter);
	void parseJson(char* & str,MySocket* & my_socket);
};

#endif

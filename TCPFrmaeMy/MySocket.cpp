#include "MySocket.h"

//填充字节函数--数据长度
bool Char8toNum(char* tmp,int & num);
bool NumToChar8(long long num,char* &tmp);
void UNPACKMessage(std::string str,RecMessage *NetMessage);

//str为保存接收的内容
bool MySocket::recInfo(RecMessage *message){
	std::string str = "";
	char buf[rec_buf_len];
	memset(buf,0,rec_buf_len);
	//接收网络传递的数据
	int len = recv(socket_m,buf,rec_buf_len,0);

	if(len == -1)
		return false;

	//保存接收到的数据
	str = "";
	//判断是否以开始标识符开头
	if(strncmp(buf, _start_identifier, strlen(_start_identifier)) != 0){
		message = NULL;
		return true;
	}

	str += buf;
	while(true){
		if(strncmp(_end_identifier,
			StringTochar(str.substr(str.length()-strlen(_end_identifier),str.length())),
			strlen(_end_identifier))
			==0)
		{
			//cout <<"发现结尾符" <<endl;
			//发现结尾符，跳出接收循环
			break;
		}
		len = recv(socket_m,buf,rec_buf_len,0);

		if(len == -1)
			return false;
		str += buf;
	}
	UNPACKMessage(str,message);
	return true;
}
void UNPACKMessage(std::string str,RecMessage *NetMessage){
	NetMessage->_start = StringTochar(str.substr(0,8));
	Char8toNum(StringTochar(str.substr(8,8)),NetMessage->_length);
	NetMessage->_type = StringTochar(str.substr(16,4));
	NetMessage->content = StringTochar(str.substr(20,NetMessage->_length));
	NetMessage->_end = StringTochar(str.substr(20+NetMessage->_length,4));
}

string PACKMessage(string content,char* type){
	std::string str = "";
	char *len = new char[9];
	NumToChar8(content.length(),len);
	str += _start_identifier;
	str += len;
	str += type;
	str += content;
	str += _end_identifier;
	return str;
}

void MySocket::sendInfo(std::string strs,char *type,long mills/* =10 */){
	std::string str=PACKMessage(strs,type);

	//std::cout << "发送消息 ： "<<str<<endl;

	char *tmp = new char[send_buf_len];
	int i = 0;
	while (i < str.length()) {
		memset(tmp, 0, send_buf_len);
		tmp = StringTochar(str.substr(i, send_buf_len > str.length() - i
			? str.length() - i : send_buf_len));
		send(socket_m, tmp, strlen(tmp) + 1, 0);
		i += send_buf_len;
		//printf("%s",tmp);
		Sleep(mills);
	}

}
void MySocket::sendInfo(char *content,char *type,long mills/* =10 */){
	std::string con = content;
	sendInfo(con,type,mills);
}
//----- 工具类函数
bool Char8toNum(char* tmp,int & num){
	int len = strlen(tmp);
	//数据格式出错
	if(len !=8){
		return false;
	}
	char *tt = new char[len+1];
	memset(tt,0,len+1);
	int index = 0;
	for(int i=0;i<len;i++){
		if(tmp[i]=='0' && index == 0)
			continue;
		else
			tt[index++] = tmp[i];
	}
	num = atoi(tt);
	return true;
}
bool NumToChar8(long long num , char* & tmp){ 
	char * tt = new char[9];
	tmp = new char[9];
	itoa(num,tt,10);
	int len = strlen(tt);
	if(len > 8)
		return false;
	for(int i = 0;i<8;i++){
		if(i<8-len)
			tmp[i] = '0';
		else
			tmp[i] = tt[i-8+len];
	}
	tmp[8] = '\0';
	return true;
}
char* StringTochar(std::string str) {
	char *data = new char[strlen(str.c_str()) + 1];
	strcpy(data, str.c_str());
	return data;
}

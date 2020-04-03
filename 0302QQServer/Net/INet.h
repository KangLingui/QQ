#ifndef _INET_H			//防止头文件重复导入  
#define _INET_H        //
#include <winsock2.h>
#include "Packef.h"
#pragma comment(lib, "ws2_32.lib")
class INet         //接口类
{
public:
	INet(){}
	virtual ~INet(){}
public:
		virtual bool InitNetWork()   =	0;   //加载网络
		virtual void UnInitNetWork()  =	0;  //卸载网络
		virtual bool SendData(unsigned int sock,char *szbuf,int nLen ,short nPort)  =	0;//接收数据 
								   //通过sock说      大小                  端口号
};






#endif
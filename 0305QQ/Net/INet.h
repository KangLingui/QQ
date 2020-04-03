#ifndef _INET_H
#define _INET_H
#include <winsock2.h>
#include <process.h>
#include "Packdef.h"
#pragma comment(lib, "ws2_32.lib")
class INet
{
public:
	INet()
	{}
	virtual ~INet()
	{}
public:
	virtual bool InitNetWork()  =0;
	virtual void UnInitNetWork() =0;           
	virtual bool SendData(unsigned int sock,char *szbuf,int nLen ,short nPort) =0;

};









#endif
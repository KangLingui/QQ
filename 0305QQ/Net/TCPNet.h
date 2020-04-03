#ifndef _TCPNET_H
#define _TCPNET_H
#include "INet.h"
#include "IKernel.h"
class TCPNet :public INet
{
public:
	TCPNet (IKernel *pkernel);     //接收到数据以后要交给核心类处理
	virtual ~TCPNet();
public:
	 bool InitNetWork();
	 void UnInitNetWork();     
	 bool SendData(unsigned int sock,char *szbuf,int nLen ,short nPort);
	 static   unsigned __stdcall ThreadProc( void * );  //线程函数

private:
	SOCKET m_sockclient;
	HANDLE m_hthread;  //接收线程句柄
	bool   m_bflagquit;//线程退出
	IKernel *m_ptcpkernel;
};









#endif
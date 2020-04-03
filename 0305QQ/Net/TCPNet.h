#ifndef _TCPNET_H
#define _TCPNET_H
#include "INet.h"
#include "IKernel.h"
class TCPNet :public INet
{
public:
	TCPNet (IKernel *pkernel);     //���յ������Ժ�Ҫ���������ദ��
	virtual ~TCPNet();
public:
	 bool InitNetWork();
	 void UnInitNetWork();     
	 bool SendData(unsigned int sock,char *szbuf,int nLen ,short nPort);
	 static   unsigned __stdcall ThreadProc( void * );  //�̺߳���

private:
	SOCKET m_sockclient;
	HANDLE m_hthread;  //�����߳̾��
	bool   m_bflagquit;//�߳��˳�
	IKernel *m_ptcpkernel;
};









#endif
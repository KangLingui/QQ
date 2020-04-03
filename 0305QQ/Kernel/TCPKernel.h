#ifndef _TCPKERNEL_H
#define _TCPKERNEL_H
#include "IKernel.h"
#include "TCPNet.h"

class TCPKernel :public IKernel
{
public:
	TCPKernel();
	virtual ~TCPKernel();
public:	   
	 bool  Open() ;
	 void  Close();
	 void  DealData(unsigned int sock,char*szbuf,short nport=0);
	 bool SendData(unsigned int sock,char *szbuf,int nLen ,short nPort);


private:
	INet *m_ptcpnet;
};








#endif
#ifndef _TCPNET_H
#define _TCPNET_H
#include "INet.h"
#include "IKernel.h"
#include <process.h>
#include <list>
#include <map>
using namespace std;
class TCPNet :public INet
{
public:
	
		TCPNet(IKernel *pkernel);
		virtual ~TCPNet();
public:
		virtual bool InitNetWork();
		virtual void UnInitNetWork();     

		virtual bool SendData(unsigned int sock,char *szbuf,int nLen ,short nPort=0);
		static unsigned  __stdcall ThreadAccept( void * ); //线程函数
		static unsigned  __stdcall ThreadRecv( void * );
private:
		SOCKET m_sockListen;
		HANDLE m_hThreadAccept;  //用来操作线程的
public:
		bool   m_bflagquit;
		list<HANDLE> m_lsthThreadRecv;
		std::map<unsigned,SOCKET> m_mapIdToSocket;  //建立映射关系
		IKernel *m_ptcpkernel;
};

#endif
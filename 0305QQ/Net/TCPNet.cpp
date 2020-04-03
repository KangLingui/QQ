#include "stdafx.h"
#include "TCPNet.h"

TCPNet::TCPNet(IKernel *pkernel)
{
	m_sockclient =NULL;
	m_bflagquit =true;
	m_hthread =NULL;
	m_ptcpkernel =pkernel;
}

TCPNet:: ~TCPNet()
{

}




bool TCPNet::InitNetWork() //
{
	WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) 
	{
      
        return false;
    }

/* Confirm that the WinSock DLL supports 2.2.*/
/* Note that if the DLL supports versions greater    */
/* than 2.2 in addition to 2.2, it will still return */
/* 2.2 in wVersion since that is the version we      */
/* requested.                                        */

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
    
       
        UnInitNetWork();
        return false;
    }
   
	//2.创建客户端
	m_sockclient =  socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(INVALID_SOCKET ==m_sockclient)
	{
		UnInitNetWork();
        return false;
	}
	//3.找地方
	
	sockaddr_in addrServer;

	addrServer.sin_family =AF_INET;
	addrServer.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
	addrServer.sin_port =htons(_DEF_PORT);

	//连接服务器
	if(SOCKET_ERROR==  connect(m_sockclient,(const sockaddr*)&addrServer,sizeof(sockaddr_in)))
	{
			 UnInitNetWork();
			 return false;
	}


	//创建线程 -- ------接收数据       
		m_hthread = (HANDLE)_beginthreadex(0,0,&ThreadProc,this,0,0);
	return true;
}


unsigned __stdcall TCPNet::ThreadProc( void * lpvoid )//线程函数
{
	//如果连接服务器成功 ---------------创建线程 -------------- 接收数据
	TCPNet *pthis = (TCPNet*)lpvoid;

	int nPackSize;
	 int nRelReadNum;
	 char *pszbuf =NULL;

	while(pthis->m_bflagquit)
	{
		//先接受包的大小
			nRelReadNum = recv(pthis->m_sockclient,(char*)&nPackSize,sizeof(int),0);
			if(nRelReadNum <=0)  //异常
			{
				continue;
			}
			
		//后接收包的内容
		pszbuf = new char[nPackSize];  //根据包的大小申请存储空间
		int noffset = 0;
		while(nPackSize)   //循环接收包
		{
		   nRelReadNum = recv(pthis->m_sockclient,pszbuf+noffset,nPackSize,0);  //每次接收到包的大小
		   nPackSize -= nRelReadNum;
		   noffset += nRelReadNum;//存储位置后移
		}

			//数据处理    
		pthis->m_ptcpkernel->DealData(pthis->m_sockclient,pszbuf);

		delete []pszbuf;
		pszbuf = NULL;
	}
	return 0;
}


void TCPNet::UnInitNetWork()   //
{
	 
	m_bflagquit = false;
		if(m_hthread)
		{
			//如果线程自己不能退    等待线程一段时间  在无信号状态下你强制杀死线程
			if(WAIT_TIMEOUT   == WaitForSingleObject(m_hthread,200))
				  TerminateThread(m_hthread,-1);
					CloseHandle(m_hthread);
					m_hthread = NULL;
		}

		if(m_sockclient)
		{
			closesocket(m_sockclient);
			m_sockclient = NULL;
		}
	 WSACleanup();
}



bool TCPNet::SendData(unsigned int sock,char *szbuf,int nLen ,short nPort)
{
	//客户端通过网络连接服务器
	//客户端向服务器发送数据
	
	//先检验数据
	if(!szbuf || nLen <=0)
		return false;
	//为了防止粘包

	//先发送包大小
	if(send(m_sockclient,(const char*)&nLen,sizeof(int),0) <= 0 )
		return false;

	//后发送包的内容
	if(send(m_sockclient,szbuf,nLen,0) <= 0 )
		return false;

	return true;
}
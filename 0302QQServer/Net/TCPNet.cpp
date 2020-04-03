#include "TCPNet.h"

TCPNet::TCPNet(IKernel *pkernel)
{

		m_sockListen = NULL;
		m_hThreadAccept = NULL;
		m_bflagquit = true;
		m_ptcpkernel = pkernel;
}

TCPNet::~TCPNet()
{
}


bool TCPNet::InitNetWork()
{
	//1.选择种类 中餐 火锅店 烤肉店
	 WORD wVersionRequested;
    WSADATA wsaData;
    int err;

/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested=MAKEWORD(2, 2);

    err =WSAStartup(wVersionRequested, &wsaData);
    if (err!= 0) 
	{   
        return false;
    }

/* Confirm that the WinSock DLL supports 2.2.*/
/* Note that if the DLL supports versions greater    */
/* than 2.2 in addition to 2.2, it will still return */
/* 2.2 in wVersion since that is the version we      */
/* requested.                                        */

    if (LOBYTE(wsaData.wVersion)!=2 || HIBYTE(wsaData.wVersion)!=2) 
	{  
        UnInitNetWork();
        return false;
    }
   
	//2.雇人--店长 
	m_sockListen =  socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(INVALID_SOCKET == m_sockListen)
	{
		UnInitNetWork();
        return false;
	}
	//3.选址 
	sockaddr_in addrServer;
	addrServer.sin_family =AF_INET;
	addrServer.sin_addr.S_un.S_addr = INADDR_ANY;  //本机任意网卡INADDR_ANY
	addrServer.sin_port = htons(_DEF_PORT);
	if(SOCKET_ERROR ==bind(m_sockListen,(const sockaddr*)&addrServer,sizeof(sockaddr_in)))
	{
		UnInitNetWork();
        return false;
	}
	
	//4.宣传
	if( SOCKET_ERROR  ==listen(m_sockListen,_DEF_LISTENNUM))
	{
		 UnInitNetWork();
        return false;
	}
	//等待客人   创建线程函数1CreateThread  2._beginthreadex
	
	m_hThreadAccept = (HANDLE) _beginthreadex(0,//安全属性
		0,//栈的大小  1Mb
		&ThreadAccept,//地址
		this,//函数列表
		0,
		0//立即运行
		);
	return true;
}

unsigned  __stdcall TCPNet::ThreadRecv( void * lpvoid )  //接收数据
{
	TCPNet *pthis = (TCPNet*)lpvoid;
	SOCKET sockwaiter =pthis->m_mapIdToSocket[GetCurrentThreadId()];//获得当前线程的ID交给服务员
	int nPackSize;
	int nRelReadNum;
	char *pszbuf = NULL;


	while(pthis->m_bflagquit)
	{  //接收数据
		//先接收包的大小  nRelReadNum 
		                                   //接受的空间
		nRelReadNum = recv(sockwaiter,(char*)&nPackSize,sizeof(int),0);
		//判断是否下线
		if(nRelReadNum <=0)        //接收包有问题
		{
			   //判断对方是否下线10054状态
				if(WSAGetLastError() == 10054 || nRelReadNum ==0)
				{
					auto ite = pthis->m_mapIdToSocket.begin();
						while(ite != pthis->m_mapIdToSocket.end())
						{
								if(ite->second == sockwaiter)
								{
									pthis->m_mapIdToSocket.erase(ite);
									  closesocket(sockwaiter);
									  sockwaiter = NULL;
									return 0;
								}
								 ite++;
						}
				}
		}
		
		//再接收包的内容
		pszbuf = new char[nPackSize];     //存储包的空间，   空间的大小nPackSize就是接受到的包的大小
		 int noffset = 0;
			while(nPackSize)
			{
			   nRelReadNum = recv(sockwaiter,pszbuf+noffset,nPackSize,0);   //每次真正接到的大小
			   nPackSize -= nRelReadNum;      //用总的大小减去每次接到的大小  就是剩余要接的大小
			   noffset += nRelReadNum;   //每次接到数据存储的位置 
			}

		                             //谁拿的数据  数据是什么
		pthis->m_ptcpkernel->DealData(sockwaiter,pszbuf,0);//处理数据

		delete []pszbuf;
		pszbuf = NULL;
	
	}

	return 0;
}

unsigned  __stdcall TCPNet::ThreadAccept( void * lpvoid)  //线程函数
{
		TCPNet *pthis = (TCPNet*)lpvoid;
		HANDLE hThread = NULL;

		unsigned uthreadid;
		SOCKET sockWaiter;
		while(pthis->m_bflagquit)
		{
			sockWaiter =  accept(pthis->m_sockListen,NULL,NULL);  //监听到客人交给服务员
			//为了解决堵塞问题    每来一个客人  就为该客人创建一个线程
			hThread = (HANDLE) _beginthreadex(0,0,&ThreadRecv,pthis,0,&uthreadid);

			if(hThread)
				{   
					pthis->m_mapIdToSocket[uthreadid] = sockWaiter;  //一个客户对应一个线程的ID
					pthis->m_lsthThreadRecv.push_back(hThread);
				}

	}
	return 0;
}




void TCPNet::UnInitNetWork()   //卸载库
{
	if(m_sockListen)   //如果店长存在 
	{
		closesocket(m_sockListen);
		m_sockListen = NULL;
	}
	m_bflagquit = false;

	//遍历接收数据的链表
	auto ite = m_lsthThreadRecv.begin();
	while(ite != m_lsthThreadRecv.end())
	{
		//如果线程不能退出    强制杀死
		//如何判断线程能否自己退    -------   等线程的信号一段时间 
			if(WAIT_TIMEOUT == WaitForSingleObject(*ite,200))
			{
				TerminateThread(*ite,-1);
			}

			CloseHandle(*ite);
			*ite = NULL;
			ite++;	
	}
	m_lsthThreadRecv.clear();


	if(m_hThreadAccept)    //线程句柄
	{
			if(WAIT_TIMEOUT == WaitForSingleObject(m_hThreadAccept,200))
				TerminateThread(m_hThreadAccept,-1);
			CloseHandle(m_hThreadAccept);
			m_hThreadAccept = NULL;
	}
	 WSACleanup();
}

	      //服务员tcp--socket  udp --ip,nPort
bool TCPNet::SendData(unsigned int sock,char *szbuf,int nLen ,short nPort)
{
                  //无效          空        不合格
	if(sock == INVALID_SOCKET || !szbuf || nLen <=0)
		return false;

	//TCP面向数据流的通信方式--------缺点-粘包
	//如何解决粘包问题 ----------1.固定包长度  2.特殊字符结尾  3，短链接  4.在包头加包长度

	//1.先发包大小
	if(send(sock,(const char*)&nLen,sizeof(int),0)<=0)
		return false;

	//2.再发数据
	if(send(sock,szbuf,nLen,0)<=0)
		return false;

	return true;
}


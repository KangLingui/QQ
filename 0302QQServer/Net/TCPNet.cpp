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
	//1.ѡ������ �в� ����� �����
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
   
	//2.����--�곤 
	m_sockListen =  socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(INVALID_SOCKET == m_sockListen)
	{
		UnInitNetWork();
        return false;
	}
	//3.ѡַ 
	sockaddr_in addrServer;
	addrServer.sin_family =AF_INET;
	addrServer.sin_addr.S_un.S_addr = INADDR_ANY;  //������������INADDR_ANY
	addrServer.sin_port = htons(_DEF_PORT);
	if(SOCKET_ERROR ==bind(m_sockListen,(const sockaddr*)&addrServer,sizeof(sockaddr_in)))
	{
		UnInitNetWork();
        return false;
	}
	
	//4.����
	if( SOCKET_ERROR  ==listen(m_sockListen,_DEF_LISTENNUM))
	{
		 UnInitNetWork();
        return false;
	}
	//�ȴ�����   �����̺߳���1CreateThread  2._beginthreadex
	
	m_hThreadAccept = (HANDLE) _beginthreadex(0,//��ȫ����
		0,//ջ�Ĵ�С  1Mb
		&ThreadAccept,//��ַ
		this,//�����б�
		0,
		0//��������
		);
	return true;
}

unsigned  __stdcall TCPNet::ThreadRecv( void * lpvoid )  //��������
{
	TCPNet *pthis = (TCPNet*)lpvoid;
	SOCKET sockwaiter =pthis->m_mapIdToSocket[GetCurrentThreadId()];//��õ�ǰ�̵߳�ID��������Ա
	int nPackSize;
	int nRelReadNum;
	char *pszbuf = NULL;


	while(pthis->m_bflagquit)
	{  //��������
		//�Ƚ��հ��Ĵ�С  nRelReadNum 
		                                   //���ܵĿռ�
		nRelReadNum = recv(sockwaiter,(char*)&nPackSize,sizeof(int),0);
		//�ж��Ƿ�����
		if(nRelReadNum <=0)        //���հ�������
		{
			   //�ж϶Է��Ƿ�����10054״̬
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
		
		//�ٽ��հ�������
		pszbuf = new char[nPackSize];     //�洢���Ŀռ䣬   �ռ�Ĵ�СnPackSize���ǽ��ܵ��İ��Ĵ�С
		 int noffset = 0;
			while(nPackSize)
			{
			   nRelReadNum = recv(sockwaiter,pszbuf+noffset,nPackSize,0);   //ÿ�������ӵ��Ĵ�С
			   nPackSize -= nRelReadNum;      //���ܵĴ�С��ȥÿ�νӵ��Ĵ�С  ����ʣ��Ҫ�ӵĴ�С
			   noffset += nRelReadNum;   //ÿ�νӵ����ݴ洢��λ�� 
			}

		                             //˭�õ�����  ������ʲô
		pthis->m_ptcpkernel->DealData(sockwaiter,pszbuf,0);//��������

		delete []pszbuf;
		pszbuf = NULL;
	
	}

	return 0;
}

unsigned  __stdcall TCPNet::ThreadAccept( void * lpvoid)  //�̺߳���
{
		TCPNet *pthis = (TCPNet*)lpvoid;
		HANDLE hThread = NULL;

		unsigned uthreadid;
		SOCKET sockWaiter;
		while(pthis->m_bflagquit)
		{
			sockWaiter =  accept(pthis->m_sockListen,NULL,NULL);  //���������˽�������Ա
			//Ϊ�˽����������    ÿ��һ������  ��Ϊ�ÿ��˴���һ���߳�
			hThread = (HANDLE) _beginthreadex(0,0,&ThreadRecv,pthis,0,&uthreadid);

			if(hThread)
				{   
					pthis->m_mapIdToSocket[uthreadid] = sockWaiter;  //һ���ͻ���Ӧһ���̵߳�ID
					pthis->m_lsthThreadRecv.push_back(hThread);
				}

	}
	return 0;
}




void TCPNet::UnInitNetWork()   //ж�ؿ�
{
	if(m_sockListen)   //����곤���� 
	{
		closesocket(m_sockListen);
		m_sockListen = NULL;
	}
	m_bflagquit = false;

	//�����������ݵ�����
	auto ite = m_lsthThreadRecv.begin();
	while(ite != m_lsthThreadRecv.end())
	{
		//����̲߳����˳�    ǿ��ɱ��
		//����ж��߳��ܷ��Լ���    -------   ���̵߳��ź�һ��ʱ�� 
			if(WAIT_TIMEOUT == WaitForSingleObject(*ite,200))
			{
				TerminateThread(*ite,-1);
			}

			CloseHandle(*ite);
			*ite = NULL;
			ite++;	
	}
	m_lsthThreadRecv.clear();


	if(m_hThreadAccept)    //�߳̾��
	{
			if(WAIT_TIMEOUT == WaitForSingleObject(m_hThreadAccept,200))
				TerminateThread(m_hThreadAccept,-1);
			CloseHandle(m_hThreadAccept);
			m_hThreadAccept = NULL;
	}
	 WSACleanup();
}

	      //����Աtcp--socket  udp --ip,nPort
bool TCPNet::SendData(unsigned int sock,char *szbuf,int nLen ,short nPort)
{
                  //��Ч          ��        ���ϸ�
	if(sock == INVALID_SOCKET || !szbuf || nLen <=0)
		return false;

	//TCP������������ͨ�ŷ�ʽ--------ȱ��-ճ��
	//��ν��ճ������ ----------1.�̶�������  2.�����ַ���β  3��������  4.�ڰ�ͷ�Ӱ�����

	//1.�ȷ�����С
	if(send(sock,(const char*)&nLen,sizeof(int),0)<=0)
		return false;

	//2.�ٷ�����
	if(send(sock,szbuf,nLen,0)<=0)
		return false;

	return true;
}


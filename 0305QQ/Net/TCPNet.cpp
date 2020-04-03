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
   
	//2.�����ͻ���
	m_sockclient =  socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(INVALID_SOCKET ==m_sockclient)
	{
		UnInitNetWork();
        return false;
	}
	//3.�ҵط�
	
	sockaddr_in addrServer;

	addrServer.sin_family =AF_INET;
	addrServer.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
	addrServer.sin_port =htons(_DEF_PORT);

	//���ӷ�����
	if(SOCKET_ERROR==  connect(m_sockclient,(const sockaddr*)&addrServer,sizeof(sockaddr_in)))
	{
			 UnInitNetWork();
			 return false;
	}


	//�����߳� -- ------��������       
		m_hthread = (HANDLE)_beginthreadex(0,0,&ThreadProc,this,0,0);
	return true;
}


unsigned __stdcall TCPNet::ThreadProc( void * lpvoid )//�̺߳���
{
	//������ӷ������ɹ� ---------------�����߳� -------------- ��������
	TCPNet *pthis = (TCPNet*)lpvoid;

	int nPackSize;
	 int nRelReadNum;
	 char *pszbuf =NULL;

	while(pthis->m_bflagquit)
	{
		//�Ƚ��ܰ��Ĵ�С
			nRelReadNum = recv(pthis->m_sockclient,(char*)&nPackSize,sizeof(int),0);
			if(nRelReadNum <=0)  //�쳣
			{
				continue;
			}
			
		//����հ�������
		pszbuf = new char[nPackSize];  //���ݰ��Ĵ�С����洢�ռ�
		int noffset = 0;
		while(nPackSize)   //ѭ�����հ�
		{
		   nRelReadNum = recv(pthis->m_sockclient,pszbuf+noffset,nPackSize,0);  //ÿ�ν��յ����Ĵ�С
		   nPackSize -= nRelReadNum;
		   noffset += nRelReadNum;//�洢λ�ú���
		}

			//���ݴ���    
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
			//����߳��Լ�������    �ȴ��߳�һ��ʱ��  �����ź�״̬����ǿ��ɱ���߳�
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
	//�ͻ���ͨ���������ӷ�����
	//�ͻ������������������
	
	//�ȼ�������
	if(!szbuf || nLen <=0)
		return false;
	//Ϊ�˷�ֹճ��

	//�ȷ��Ͱ���С
	if(send(m_sockclient,(const char*)&nLen,sizeof(int),0) <= 0 )
		return false;

	//���Ͱ�������
	if(send(m_sockclient,szbuf,nLen,0) <= 0 )
		return false;

	return true;
}
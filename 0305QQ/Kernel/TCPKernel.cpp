#include "stdafx.h"
#include "TCPKernel.h"
#include "0305QQ.h"
TCPKernel::TCPKernel()
{
	m_ptcpnet =	new TCPNet(this);
}



TCPKernel::~TCPKernel()
{
	delete m_ptcpnet;
	m_ptcpnet =NULL;
}


bool TCPKernel::Open()
{
	if(!m_ptcpnet->InitNetWork())
	{
		return false;
	}
		

	return true;
}


void TCPKernel::Close()
{

	m_ptcpnet->UnInitNetWork();  //ж������
}



void TCPKernel::DealData(unsigned int sock,char*szbuf,short nport)
{
	//��������----------�ж��յ�����ʲô��  Ȼ���ڴ���
		PackType *pType =(PackType*)szbuf;
	switch (*pType)
	{

		case _DEF_PROTOCOL_REGISTER_RS:   
			{ 
					//ע��ظ���
				STRU_REGISTER_RS *psrr =(STRU_REGISTER_RS*)szbuf;
					if(psrr->m_szResult ==  _register_success)  //���ע��ɹ��򴰿ڷ���Ϣ
					{
															//ע��ظ���Ϣ
							theApp.m_pMainWnd->PostMessage(UM_REGISTERMSG,psrr->m_userid);
					}
					else
					{
						AfxMessageBox(_T("ע��ʧ��"));
					}
						
			}
			break;

		case _DEF_PROTOCOL_LOGIN_RS:   //��¼�ظ���
			{
				STRU_LOGIN_RS *pslr = (STRU_LOGIN_RS*)szbuf;
					if(pslr->m_szResult == _login_success)
					{
						//�򴰿ڷ���Ϣ
						theApp.m_pMainWnd->PostMessage(UM_LOGINMSG);
					}
				else
				{
					AfxMessageBox(_T("��¼ʧ��"));
					}
					
			}
			break;

		case _DEF_PROTOCOL_GETFRIENDLIST_RS:  //��ȡ���ѻظ�
			{
				STRU_GETFRIENDLIST_RS *psgr=  (STRU_GETFRIENDLIST_RS*)szbuf;
				theApp.m_pMainWnd->SendMessage(UM_GETFRIENDLISTMSG,(WPARAM)psgr->m_aryFriend,psgr->m_nFriendNum);
			                   //�˴�������PostMessage���ͣ�����������
			}
			break;


		case   _DEF_PROTOCOL_SELECTFRIEND_RQ:   //ѡ��������
			{
				STRU_SELECTFRIEND_RQ *pssr = (STRU_SELECTFRIEND_RQ *)szbuf;//������Э��
				theApp.m_pMainWnd->SendMessage(UM_SELECTFRIENDMSG,pssr->m_userid,(LPARAM)pssr->m_szContent);
				//��Ϣ�Ƿ��͵������ڵ�                             ע�⣺������������ID
			}


	default:
		break;
	}

}
bool TCPKernel::SendData(unsigned int sock,char *szbuf,int nLen ,short nPort)
{
		if(!m_ptcpnet->SendData(sock,szbuf,nLen,nPort))  
		{
			return false;
		}
		

		return true;
}
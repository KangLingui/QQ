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

	m_ptcpnet->UnInitNetWork();  //卸载网络
}



void TCPKernel::DealData(unsigned int sock,char*szbuf,short nport)
{
	//处理数据----------判断收到的是什么包  然后在处理
		PackType *pType =(PackType*)szbuf;
	switch (*pType)
	{

		case _DEF_PROTOCOL_REGISTER_RS:   
			{ 
					//注册回复包
				STRU_REGISTER_RS *psrr =(STRU_REGISTER_RS*)szbuf;
					if(psrr->m_szResult ==  _register_success)  //如果注册成功向窗口发消息
					{
															//注册回复消息
							theApp.m_pMainWnd->PostMessage(UM_REGISTERMSG,psrr->m_userid);
					}
					else
					{
						AfxMessageBox(_T("注册失败"));
					}
						
			}
			break;

		case _DEF_PROTOCOL_LOGIN_RS:   //登录回复包
			{
				STRU_LOGIN_RS *pslr = (STRU_LOGIN_RS*)szbuf;
					if(pslr->m_szResult == _login_success)
					{
						//向窗口发消息
						theApp.m_pMainWnd->PostMessage(UM_LOGINMSG);
					}
				else
				{
					AfxMessageBox(_T("登录失败"));
					}
					
			}
			break;

		case _DEF_PROTOCOL_GETFRIENDLIST_RS:  //获取好友回复
			{
				STRU_GETFRIENDLIST_RS *psgr=  (STRU_GETFRIENDLIST_RS*)szbuf;
				theApp.m_pMainWnd->SendMessage(UM_GETFRIENDLISTMSG,(WPARAM)psgr->m_aryFriend,psgr->m_nFriendNum);
			                   //此处不能用PostMessage发送？？？？？？
			}
			break;


		case   _DEF_PROTOCOL_SELECTFRIEND_RQ:   //选择好友唠嗑
			{
				STRU_SELECTFRIEND_RQ *pssr = (STRU_SELECTFRIEND_RQ *)szbuf;//解析包协议
				theApp.m_pMainWnd->SendMessage(UM_SELECTFRIENDMSG,pssr->m_userid,(LPARAM)pssr->m_szContent);
				//消息是发送到主窗口的                             注意：：这里是朋友ID
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
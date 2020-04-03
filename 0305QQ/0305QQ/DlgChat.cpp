// DlgChat.cpp : 实现文件
//

#include "stdafx.h"
#include "0305QQ.h"
#include "DlgChat.h"
#include "afxdialogex.h"
#include"DlgMain.h"

// CDlgChat 对话框

IMPLEMENT_DYNAMIC(CDlgChat, CDialogEx)

CDlgChat::CDlgChat(CWnd* pParent /*=NULL*/): CDialogEx(CDlgChat::IDD, pParent), m_edtsend1(_T(""))
{

}

CDlgChat::~CDlgChat()
{
}

void CDlgChat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lstbox);
	DDX_Text(pDX, IDC_EDIT1, m_edtsend1);
}


BEGIN_MESSAGE_MAP(CDlgChat, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgChat::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgChat 消息处理程序


void CDlgChat::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//拿到聊天窗口内容   发送出去
	CString strqq;

	UpdateData(TRUE);

	GetWindowText(strqq);
	STRU_SELECTFRIEND_RQ  ssr;
	ssr.m_ntype = _DEF_PROTOCOL_SELECTFRIEND_RQ;
	ssr.m_userid = ((CDlgMain*)theApp.m_pMainWnd)->m_userID;
	ssr.m_Frienid = _wtoi64(strqq);

	WideCharToMultiByte(CP_ACP,0,m_edtsend1,-1,ssr.m_szContent,_DEF_CONTENTNUM,0,0);

	//将聊天信息发给服务器
	if(theApp.m_ptcpkernel->SendData(0,(char*)&ssr,sizeof(ssr),0)) //判断消息是否发送成功
	{
		//如果消息发送成功----把自己的消息放到聊天窗口区
		m_lstbox.AddString(_T("我说："));   
		m_lstbox.AddString(m_edtsend1);
		m_edtsend1 = _T("");
		UpdateData(FALSE);   //
	}


}

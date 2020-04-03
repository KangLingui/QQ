// DlgChat.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "0305QQ.h"
#include "DlgChat.h"
#include "afxdialogex.h"
#include"DlgMain.h"

// CDlgChat �Ի���

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


// CDlgChat ��Ϣ�������


void CDlgChat::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	//�õ����촰������   ���ͳ�ȥ
	CString strqq;

	UpdateData(TRUE);

	GetWindowText(strqq);
	STRU_SELECTFRIEND_RQ  ssr;
	ssr.m_ntype = _DEF_PROTOCOL_SELECTFRIEND_RQ;
	ssr.m_userid = ((CDlgMain*)theApp.m_pMainWnd)->m_userID;
	ssr.m_Frienid = _wtoi64(strqq);

	WideCharToMultiByte(CP_ACP,0,m_edtsend1,-1,ssr.m_szContent,_DEF_CONTENTNUM,0,0);

	//��������Ϣ����������
	if(theApp.m_ptcpkernel->SendData(0,(char*)&ssr,sizeof(ssr),0)) //�ж���Ϣ�Ƿ��ͳɹ�
	{
		//�����Ϣ���ͳɹ�----���Լ�����Ϣ�ŵ����촰����
		m_lstbox.AddString(_T("��˵��"));   
		m_lstbox.AddString(m_edtsend1);
		m_edtsend1 = _T("");
		UpdateData(FALSE);   //
	}


}

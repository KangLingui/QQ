#include "stdafx.h"
#include "0305QQ.h"
#include "DlgMain.h"
#include "afxdialogex.h"




IMPLEMENT_DYNAMIC(CDlgMain, CDialogEx)

CDlgMain::CDlgMain(CWnd* pParent )
	: CDialogEx(CDlgMain::IDD, pParent)
{

}

CDlgMain::~CDlgMain()
{
}

void CDlgMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lstctrl);
}

BEGIN_MESSAGE_MAP(CDlgMain, CDialogEx)

	ON_MESSAGE(UM_GETFRIENDLISTMSG,&CDlgMain::GetFriendListRq) //建立消息映射
	ON_MESSAGE(UM_SELECTFRIENDMSG,&CDlgMain::SelectFriendRq)   //建立消息映射

	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDlgMain::OnNMDblclkList1)
END_MESSAGE_MAP()
      


// CDlgMain 消息处理程序


BOOL CDlgMain::OnInitDialog()
{
		CDialogEx::OnInitDialog();

		//向窗口插入列表头
		m_lstctrl.InsertColumn(0,_T("QQ"),0,150);
		m_lstctrl.InsertColumn(1,_T("名字"),0,150);
		m_lstctrl.InsertColumn(2,_T("状态"),0,150);

		m_lstctrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);//设置整行选中的风格



	//向服务器发送获取好友列表请求
		STRU_GETFRIENDLIST_RQ sgr;
		sgr.m_ntype =_DEF_PROTOCOL_GETFRIENDLIST_RQ;
		sgr.m_userid =m_userID;

		theApp.m_ptcpkernel->SendData(0, (char*)&sgr,sizeof(sgr),0);//发送请求



	return TRUE; 



}
LRESULT CDlgMain::GetFriendListRq(WPARAM W,LPARAM L) //获得好友的窗口消息
{
	FriendInfo *pfriend =  (FriendInfo *)W;//好朋友的数组
	int nfriendnum = L;//好朋友的个数
	TCHAR szbuf[100] = {0};
	CString str;
	for(int i = 0;i<nfriendnum;i++)  //将好友循环插入窗口
	{
			//字符转换、？？？？、
		_itow_s(pfriend[i].m_userid,szbuf,100,10);
		m_lstctrl.InsertItem(i,szbuf) ;   //插入QQ号

		str = pfriend[i].m_szName;
		m_lstctrl.SetItemText(i,1,str);  //出入名字

		str = pfriend[i].m_state;
		m_lstctrl.SetItemText(i,2,str);  //插入状态
	
	
	}


	return 0;

}


LRESULT CDlgMain::SelectFriendRq(WPARAM W,LPARAM L)     //选择好友唠嗑的主窗口消息
{
	//W===好友ID   L ==聊天内容
	CString strqq;
	strqq.Format(_T("%d"),W);   //格式化  
	
	char *m_szContent  = (char *)L;
	TCHAR szbuf[100] = {0};
	MultiByteToWideChar(CP_ACP,0,m_szContent,-1,szbuf,100);   //多字节转为宽字节

	CDlgChat  *pdlg =m_maptoqqdlg[strqq];

	if(!pdlg)
	{
	pdlg = new CDlgChat;
	pdlg->Create(IDD_CHAT);  //创建非模态
	pdlg->SetWindowText(strqq);   //设置聊天窗口标题

	//----以上会出现一个好友多个聊天窗口----为了做到唯一窗口---建立map映射----------------
	m_maptoqqdlg[strqq] =pdlg;
	}
	pdlg->ShowWindow(SW_SHOWNORMAL);

	strqq = strqq+_T("说：");//拼接    朋友说。。。。。
	pdlg->m_lstbox.AddString(strqq);
	pdlg->m_lstbox.AddString(szbuf);


	return 0; 


}



void CDlgMain::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	// TODO: 在此添加控件通知处理程序代码
	//为了同时可以和多个好友聊天------需要创建非模态对话框
	CString strqq  =m_lstctrl.GetItemText(pNMItemActivate->iItem,0);

	//双击qq好友  弹出聊天窗口
	//先判断当前好友是否有聊天窗口
	CDlgChat  *pdlg = m_maptoqqdlg[strqq];

	if(!pdlg)
	{
	pdlg = new CDlgChat;
	pdlg->Create(IDD_CHAT);  //创建非模态
	pdlg->SetWindowText(strqq);   //设置聊天窗口标题

	//----以上会出现一个好友多个聊天窗口----为了做到唯一窗口---建立map映射----------------
	m_maptoqqdlg[strqq] =pdlg;
	}
	pdlg->ShowWindow(SW_SHOWNORMAL);   //显示到窗口

	

	*pResult = 0;
}

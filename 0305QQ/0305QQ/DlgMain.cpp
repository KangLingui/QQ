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

	ON_MESSAGE(UM_GETFRIENDLISTMSG,&CDlgMain::GetFriendListRq) //������Ϣӳ��
	ON_MESSAGE(UM_SELECTFRIENDMSG,&CDlgMain::SelectFriendRq)   //������Ϣӳ��

	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDlgMain::OnNMDblclkList1)
END_MESSAGE_MAP()
      


// CDlgMain ��Ϣ�������


BOOL CDlgMain::OnInitDialog()
{
		CDialogEx::OnInitDialog();

		//�򴰿ڲ����б�ͷ
		m_lstctrl.InsertColumn(0,_T("QQ"),0,150);
		m_lstctrl.InsertColumn(1,_T("����"),0,150);
		m_lstctrl.InsertColumn(2,_T("״̬"),0,150);

		m_lstctrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);//��������ѡ�еķ��



	//����������ͻ�ȡ�����б�����
		STRU_GETFRIENDLIST_RQ sgr;
		sgr.m_ntype =_DEF_PROTOCOL_GETFRIENDLIST_RQ;
		sgr.m_userid =m_userID;

		theApp.m_ptcpkernel->SendData(0, (char*)&sgr,sizeof(sgr),0);//��������



	return TRUE; 



}
LRESULT CDlgMain::GetFriendListRq(WPARAM W,LPARAM L) //��ú��ѵĴ�����Ϣ
{
	FriendInfo *pfriend =  (FriendInfo *)W;//�����ѵ�����
	int nfriendnum = L;//�����ѵĸ���
	TCHAR szbuf[100] = {0};
	CString str;
	for(int i = 0;i<nfriendnum;i++)  //������ѭ�����봰��
	{
			//�ַ�ת��������������
		_itow_s(pfriend[i].m_userid,szbuf,100,10);
		m_lstctrl.InsertItem(i,szbuf) ;   //����QQ��

		str = pfriend[i].m_szName;
		m_lstctrl.SetItemText(i,1,str);  //��������

		str = pfriend[i].m_state;
		m_lstctrl.SetItemText(i,2,str);  //����״̬
	
	
	}


	return 0;

}


LRESULT CDlgMain::SelectFriendRq(WPARAM W,LPARAM L)     //ѡ�������ྵ���������Ϣ
{
	//W===����ID   L ==��������
	CString strqq;
	strqq.Format(_T("%d"),W);   //��ʽ��  
	
	char *m_szContent  = (char *)L;
	TCHAR szbuf[100] = {0};
	MultiByteToWideChar(CP_ACP,0,m_szContent,-1,szbuf,100);   //���ֽ�תΪ���ֽ�

	CDlgChat  *pdlg =m_maptoqqdlg[strqq];

	if(!pdlg)
	{
	pdlg = new CDlgChat;
	pdlg->Create(IDD_CHAT);  //������ģ̬
	pdlg->SetWindowText(strqq);   //�������촰�ڱ���

	//----���ϻ����һ�����Ѷ�����촰��----Ϊ������Ψһ����---����mapӳ��----------------
	m_maptoqqdlg[strqq] =pdlg;
	}
	pdlg->ShowWindow(SW_SHOWNORMAL);

	strqq = strqq+_T("˵��");//ƴ��    ����˵����������
	pdlg->m_lstbox.AddString(strqq);
	pdlg->m_lstbox.AddString(szbuf);


	return 0; 


}



void CDlgMain::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//Ϊ��ͬʱ���ԺͶ����������------��Ҫ������ģ̬�Ի���
	CString strqq  =m_lstctrl.GetItemText(pNMItemActivate->iItem,0);

	//˫��qq����  �������촰��
	//���жϵ�ǰ�����Ƿ������촰��
	CDlgChat  *pdlg = m_maptoqqdlg[strqq];

	if(!pdlg)
	{
	pdlg = new CDlgChat;
	pdlg->Create(IDD_CHAT);  //������ģ̬
	pdlg->SetWindowText(strqq);   //�������촰�ڱ���

	//----���ϻ����һ�����Ѷ�����촰��----Ϊ������Ψһ����---����mapӳ��----------------
	m_maptoqqdlg[strqq] =pdlg;
	}
	pdlg->ShowWindow(SW_SHOWNORMAL);   //��ʾ������

	

	*pResult = 0;
}

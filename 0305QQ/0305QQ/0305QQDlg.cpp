
// 0305QQDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "0305QQ.h"
#include "0305QQDlg.h"
#include "afxdialogex.h"
#include "DlgMain.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMy0305QQDlg �Ի���



CMy0305QQDlg::CMy0305QQDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy0305QQDlg::IDD, pParent)
	, m_edtUserId(1)
	, m_edtUserName(_T("С��"))    //��ʼ��Ϣ
	, m_edtUserPassWord(_T("1111"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy0305QQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDT_USERID, m_edtUserId);     //ʵ�ֿռ����ӱ����İ�
	DDX_Text(pDX, IDC_EDT_USERNAME, m_edtUserName);		//ʵ�ֿռ����ӱ����İ�
	DDX_Text(pDX, IDC_EDIT_PASWORD, m_edtUserPassWord);		//ʵ�ֿռ����ӱ����İ�
}

BEGIN_MESSAGE_MAP(CMy0305QQDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_MESSAGE(UM_REGISTERMSG,&CMy0305QQDlg::RegisterMsg)
	ON_MESSAGE(UM_LOGINMSG,&CMy0305QQDlg::LoginMsg)
	ON_BN_CLICKED(IDC_BUTTON1, &CMy0305QQDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMy0305QQDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMy0305QQDlg ��Ϣ�������


LRESULT CMy0305QQDlg::RegisterMsg(WPARAM W,LPARAM L) //�ڴ�����  �������� ע��ظ� ����Ϣ
{
		//W ---���Ǵ������û� ----- id
		m_edtUserId = W;


		//��ʾ�Ի��� 
		UpdateData(FALSE);	 //j����Ϣ��ʼ�����Ի�����
		AfxMessageBox(_T("ע��ɹ�"));


	return 0;
}


LRESULT CMy0305QQDlg::LoginMsg(WPARAM W,LPARAM L)
{
	UpdateData(TRUE);//��ȡ�����ϵ��û�����

		//�����¼�ɹ����л�����
	CDlgMain dlg;

	theApp.m_pMainWnd = &dlg;			//�л���������
	dlg.m_userID = m_edtUserId;			//��¼��¼��qq��
    EndDialog(IDOK);						//�ȹر�ԭ���Ĵ���
	dlg.DoModal();						    //�󵯳�������	
	return 0;
}


BOOL CMy0305QQDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������


	if(!theApp.m_ptcpkernel->Open() )   //���ӷ�����
	{
			MessageBox(_T("���ӷ�����ʧ��"));
		return FALSE;
	}
	return TRUE;  
	// ���ǽ��������õ��ؼ������򷵻� TRUE
	
}

void CMy0305QQDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMy0305QQDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMy0305QQDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy0305QQDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	 
	CDialogEx::OnClose();
}


void CMy0305QQDlg::OnBnClickedButton1()    //ע�ᰴť
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	UpdateData(TRUE);		//���ܣ��ӶԻ����ϻ�ȡ����  

	STRU_REGISTER_RQ srr;    //ע������
	 srr.m_ntype = _DEF_PROTOCOL_REGISTER_RQ;

	//�����ֽ�ת���ֽ�  ��׼�ַ�                   
	WideCharToMultiByte(CP_ACP,0,m_edtUserName,-1,srr.m_szName,_DEF_SIZE,0,0); //����
	WideCharToMultiByte(CP_ACP,0,m_edtUserPassWord,-1,srr.m_szPassword,_DEF_SIZE,0,0);//���� 


	//��ע����Ϣ����������
	  theApp.m_ptcpkernel->SendData(0,(char*)&srr,sizeof(srr),0);


}


void CMy0305QQDlg::OnBnClickedButton2()   //��¼��ť
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);			//���ܣ��ӶԻ����ϻ�ȡ����  

	STRU_LOGIN_RQ slr;			 //��¼�����
	slr.m_ntype =_DEF_PROTOCOL_LOGIN_RQ;
		slr.m_userid =m_edtUserId;

	 WideCharToMultiByte(CP_ACP,0,m_edtUserPassWord,-1,slr.m_szPassword,_DEF_SIZE,0,0); 
	//����¼��Ϣ����������
		theApp.m_ptcpkernel->SendData(0,(char*)&slr,sizeof(slr),0);   

}

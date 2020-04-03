
// 0305QQDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "0305QQ.h"
#include "0305QQDlg.h"
#include "afxdialogex.h"
#include "DlgMain.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMy0305QQDlg 对话框



CMy0305QQDlg::CMy0305QQDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy0305QQDlg::IDD, pParent)
	, m_edtUserId(1)
	, m_edtUserName(_T("小康"))    //初始信息
	, m_edtUserPassWord(_T("1111"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy0305QQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDT_USERID, m_edtUserId);     //实现空间和添加变量的绑定
	DDX_Text(pDX, IDC_EDT_USERNAME, m_edtUserName);		//实现空间和添加变量的绑定
	DDX_Text(pDX, IDC_EDIT_PASWORD, m_edtUserPassWord);		//实现空间和添加变量的绑定
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


// CMy0305QQDlg 消息处理程序


LRESULT CMy0305QQDlg::RegisterMsg(WPARAM W,LPARAM L) //在窗口中  用来接收 注册回复 的消息
{
		//W ---就是代表了用户 ----- id
		m_edtUserId = W;


		//显示对话框 
		UpdateData(FALSE);	 //j将信息初始化到对话框上
		AfxMessageBox(_T("注册成功"));


	return 0;
}


LRESULT CMy0305QQDlg::LoginMsg(WPARAM W,LPARAM L)
{
	UpdateData(TRUE);//获取窗口上的用户数据

		//如果登录成功，切换窗口
	CDlgMain dlg;

	theApp.m_pMainWnd = &dlg;			//切换到主窗口
	dlg.m_userID = m_edtUserId;			//记录登录的qq号
    EndDialog(IDOK);						//先关闭原来的窗口
	dlg.DoModal();						    //后弹出主窗口	
	return 0;
}


BOOL CMy0305QQDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码


	if(!theApp.m_ptcpkernel->Open() )   //连接服务器
	{
			MessageBox(_T("连接服务器失败"));
		return FALSE;
	}
	return TRUE;  
	// 除非将焦点设置到控件，否则返回 TRUE
	
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMy0305QQDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMy0305QQDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy0305QQDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	 
	CDialogEx::OnClose();
}


void CMy0305QQDlg::OnBnClickedButton1()    //注册按钮
{
	// TODO: 在此添加控件通知处理程序代码
	
	UpdateData(TRUE);		//功能：从对话框上获取数据  

	STRU_REGISTER_RQ srr;    //注册请求
	 srr.m_ntype = _DEF_PROTOCOL_REGISTER_RQ;

	//将宽字节转多字节  标准字符                   
	WideCharToMultiByte(CP_ACP,0,m_edtUserName,-1,srr.m_szName,_DEF_SIZE,0,0); //名字
	WideCharToMultiByte(CP_ACP,0,m_edtUserPassWord,-1,srr.m_szPassword,_DEF_SIZE,0,0);//密码 


	//将注册信息发给服务器
	  theApp.m_ptcpkernel->SendData(0,(char*)&srr,sizeof(srr),0);


}


void CMy0305QQDlg::OnBnClickedButton2()   //登录按钮
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);			//功能：从对话框上获取数据  

	STRU_LOGIN_RQ slr;			 //登录请求包
	slr.m_ntype =_DEF_PROTOCOL_LOGIN_RQ;
		slr.m_userid =m_edtUserId;

	 WideCharToMultiByte(CP_ACP,0,m_edtUserPassWord,-1,slr.m_szPassword,_DEF_SIZE,0,0); 
	//将登录信息发给服务器
		theApp.m_ptcpkernel->SendData(0,(char*)&slr,sizeof(slr),0);   

}

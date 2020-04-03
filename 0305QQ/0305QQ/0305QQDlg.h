
// 0305QQDlg.h : 头文件
//

#pragma once


// CMy0305QQDlg 对话框
class CMy0305QQDlg : public CDialogEx
{
// 构造
public:
	CMy0305QQDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY0305QQ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual  BOOL OnInitDialog();
	afx_msg  void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg  void OnPaint();
	afx_msg  HCURSOR OnQueryDragIcon();
	
	LRESULT  RegisterMsg(WPARAM W,LPARAM L);   //在窗口  用来接收 注册回复 的消息
	LRESULT  LoginMsg(WPARAM W,LPARAM L);
	DECLARE_MESSAGE_MAP()
public:
	
	LONGLONG m_edtUserId;          //注册框的变量
	CString m_edtUserName;
	CString m_edtUserPassWord;
    afx_msg  void OnClose();
	afx_msg  void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};

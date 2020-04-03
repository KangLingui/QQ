#pragma once
#include "afxwin.h"


// CDlgChat 对话框

class CDlgChat : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgChat)

public:
	CDlgChat(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgChat();

// 对话框数据
	enum { IDD = IDD_CHAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_lstbox;
	afx_msg void OnBnClickedButton1();
	CString m_edtsend1;
};

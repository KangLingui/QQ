#pragma once
#include "afxcmn.h"
#include"DlgChat.h"
#include<map>

// CDlgMain 对话框

class CDlgMain : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMain)

public:
	CDlgMain(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMain();

// 对话框数据
	enum { IDD = IDD_DLG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
	LRESULT GetFriendListRq(WPARAM W,LPARAM L);     //获得好友的窗口消息
	LRESULT SelectFriendRq(WPARAM W,LPARAM L);     //选择好友唠嗑的主窗口消息
public:
	LONGLONG m_userID;
	virtual BOOL OnInitDialog();
	CListCtrl m_lstctrl;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	std::map<CString,CDlgChat*> m_maptoqqdlg;      //为了让一个好友只能弹出一个聊天对话框建立map关系



};

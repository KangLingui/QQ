#pragma once
#include "afxcmn.h"
#include"DlgChat.h"
#include<map>

// CDlgMain �Ի���

class CDlgMain : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMain)

public:
	CDlgMain(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMain();

// �Ի�������
	enum { IDD = IDD_DLG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
	LRESULT GetFriendListRq(WPARAM W,LPARAM L);     //��ú��ѵĴ�����Ϣ
	LRESULT SelectFriendRq(WPARAM W,LPARAM L);     //ѡ�������ྵ���������Ϣ
public:
	LONGLONG m_userID;
	virtual BOOL OnInitDialog();
	CListCtrl m_lstctrl;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	std::map<CString,CDlgChat*> m_maptoqqdlg;      //Ϊ����һ������ֻ�ܵ���һ������Ի�����map��ϵ



};

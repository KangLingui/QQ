
// 0305QQDlg.h : ͷ�ļ�
//

#pragma once


// CMy0305QQDlg �Ի���
class CMy0305QQDlg : public CDialogEx
{
// ����
public:
	CMy0305QQDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY0305QQ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual  BOOL OnInitDialog();
	afx_msg  void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg  void OnPaint();
	afx_msg  HCURSOR OnQueryDragIcon();
	
	LRESULT  RegisterMsg(WPARAM W,LPARAM L);   //�ڴ���  �������� ע��ظ� ����Ϣ
	LRESULT  LoginMsg(WPARAM W,LPARAM L);
	DECLARE_MESSAGE_MAP()
public:
	
	LONGLONG m_edtUserId;          //ע���ı���
	CString m_edtUserName;
	CString m_edtUserPassWord;
    afx_msg  void OnClose();
	afx_msg  void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};

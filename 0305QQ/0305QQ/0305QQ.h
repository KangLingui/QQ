
// 0305QQ.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//


#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include "TCPKernel.h"
// CMy0305QQApp:
// �йش����ʵ�֣������ 0305QQ.cpp
//

class CMy0305QQApp : public CWinApp
{
public:
	CMy0305QQApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
public:
	IKernel *m_ptcpkernel;
	virtual int ExitInstance();
};

extern CMy0305QQApp theApp;
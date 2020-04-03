
// 0305QQ.h : PROJECT_NAME 应用程序的主头文件
//


#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#include "TCPKernel.h"
// CMy0305QQApp:
// 有关此类的实现，请参阅 0305QQ.cpp
//

class CMy0305QQApp : public CWinApp
{
public:
	CMy0305QQApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
public:
	IKernel *m_ptcpkernel;
	virtual int ExitInstance();
};

extern CMy0305QQApp theApp;

// SerialDetector.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSerialDetectorApp: 
// �йش����ʵ�֣������ SerialDetector.cpp
//

class CSerialDetectorApp : public CWinApp
{
public:
	CSerialDetectorApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSerialDetectorApp theApp;
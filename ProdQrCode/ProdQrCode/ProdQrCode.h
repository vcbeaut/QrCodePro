
// ProdQrCode.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CProdQrCodeApp: 
// �йش����ʵ�֣������ ProdQrCode.cpp
//

class CProdQrCodeApp : public CWinApp
{
public:
	CProdQrCodeApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CProdQrCodeApp theApp;
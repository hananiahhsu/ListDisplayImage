
// GetPicFromUrl_MFC.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGetPicFromUrl_MFCApp:
// �йش����ʵ�֣������ GetPicFromUrl_MFC.cpp
//

class CGetPicFromUrl_MFCApp : public CWinAppEx
{
public:
	CGetPicFromUrl_MFCApp();

// ��д
	public:
	virtual BOOL InitInstance();



// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGetPicFromUrl_MFCApp theApp;
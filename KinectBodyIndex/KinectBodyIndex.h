
// KinectBodyIndex.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CKinectBodyIndexApp: 
// �йش����ʵ�֣������ KinectBodyIndex.cpp
//

class CKinectBodyIndexApp : public CWinApp
{
public:
	CKinectBodyIndexApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CKinectBodyIndexApp theApp;
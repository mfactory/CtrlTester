﻿
// CtrlTester.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CCtrlTesterApp:
// このクラスの実装については、CtrlTester.cpp を参照してください。
//

class CCtrlTesterApp : public CWinAppEx
{
public:
	CCtrlTesterApp();

// オーバーライド
	public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CCtrlTesterApp theApp;
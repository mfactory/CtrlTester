
// CtrlTesterDlg.h : ヘッダー ファイル
//

#pragma once
#include "xBrowser.h"
#include "xButton.h"

// CCtrlTesterDlg ダイアログ
class CCtrlTesterDlg : public CDialog
{
// コンストラクション
public:
	CCtrlTesterDlg(CWnd* pParent = NULL);	// 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_CTRLTESTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CxBrowser m_br;
	CxButton m_btn1;
};

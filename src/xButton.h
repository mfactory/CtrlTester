#pragma once


// CxButton

class CxButton : public CMFCButton
{
	DECLARE_DYNAMIC(CxButton)

public:
	CxButton();
	virtual ~CxButton();

protected:
	DECLARE_MESSAGE_MAP()

// オーバーライド
protected:
	virtual void OnFillBackground(CDC* pDC, const CRect& rectClient);
	virtual void OnDrawBorder(CDC* pDC, CRect& rectClient, UINT uiState);
	virtual void OnDrawFocusRect(CDC* pDC, const CRect& rectClient);
};



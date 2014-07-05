#pragma once

#include "mshtml.h"

class CxBrowser : public CWnd
{
public:
    CxBrowser();
    virtual ~CxBrowser();

	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pwndParent, UINT nID);
	BOOL CreateFromControl(CWnd *pwndParent, UINT nID, DWORD dwStyle = WS_CHILD | WS_VISIBLE);

	void Navigate(LPCTSTR lpszURL);
	void Write(LPCTSTR lpszHtml);				

public:
	virtual void PostNcDestroy();
	virtual BOOL PreTranslateMessage(MSG *pMsg);

// message
protected:
	void OnSize(UINT nType, int cx, int cy);

private:
    void _OnNavigateComplete2(LPDISPATCH lpDisp,VARIANT FAR* URL);

    DECLARE_MESSAGE_MAP()
    DECLARE_EVENTSINK_MAP()

	void WriteImpl();

protected:
	CWnd                    m_wnd;
	CComPtr< IWebBrowser2 > m_spBrowser;
	CComPtr< IDispatch >	m_spDisp;
	
	bool					m_bReady;
	CString					m_strHtml;
};

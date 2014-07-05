#include "stdafx.h"
#include "comdef.h"
#include "mshtml.h"
#include "mshtmcid.h"
#include "mshtmhst.h"
#include "exdispid.h"

#include "xBrowser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CxBrowser::CxBrowser()
: m_bReady(false)
{
}

CxBrowser::~CxBrowser()
{
	m_spBrowser.Release();
	m_spDisp.Release();
}

// Create Window
BOOL CxBrowser::Create( DWORD dwStyle, const RECT& rect, CWnd* pwndParent, UINT nID)
{
	m_bReady      = false;
	
	m_spBrowser.Release();

    // create this window
	BOOL bRet = __super::Create(AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW), NULL, dwStyle, rect, pwndParent, nID);
	if (!bRet) { return FALSE; }

	CRect rectBr(0,0,(rect.right - rect.left),(rect.bottom - rect.top));
	rectBr.left-=2;	rectBr.top-=2;
	rectBr.right++;	rectBr.bottom++;
    bRet = m_wnd.CreateControl(CLSID_WebBrowser, NULL, (WS_VISIBLE | WS_TABSTOP), rectBr, this, AFX_IDW_PANE_FIRST);
    if (!bRet) {
        DestroyWindow();
        return FALSE;
    }

    // get control interfaces
    LPUNKNOWN unknown = m_wnd.GetControlUnknown();
    HRESULT hr = unknown->QueryInterface(IID_IWebBrowser2,(void **)&m_spBrowser);
    if (SUCCEEDED(hr)) {
        hr = unknown->QueryInterface(IID_IDispatch,(void **)&m_spDisp);
    }

    if (!SUCCEEDED(hr)) {
        m_wnd.DestroyWindow();
        DestroyWindow();
        return FALSE;        
    }

	// navigate to initial blank page
	if (m_spBrowser) {
		Navigate(_T("about:blank"));
	}

    return TRUE;
}

// Create in place of dialog control
BOOL CxBrowser::CreateFromControl(CWnd *pwndParent,UINT nID,DWORD dwStyle)
{
	BOOL bRet = FALSE;
	ASSERT(pwndParent);

	CWnd *pwndTarget = pwndParent->GetDlgItem(nID);

	if (pwndTarget) {
		// get control location
		CRect		rect;
		pwndTarget->GetWindowRect(&rect);
		pwndParent->ScreenToClient(&rect);

		// destroy control, since the browser will take its place
		pwndTarget->DestroyWindow();

		// Create Window
		bRet = Create(dwStyle, rect, pwndParent, nID);
	}

	return bRet;
}

// Destruction
void CxBrowser::PostNcDestroy()
{
	m_spBrowser.Release();
	m_spDisp.Release();

	m_bReady = false;
	m_strHtml = _T("");
	
	CWnd::PostNcDestroy();
}

/////////////////////////////////////////////////////////////////////////////
// public function
/////////////////////////////////////////////////////////////////////////////

// Navigate to URL
void CxBrowser::Navigate(LPCTSTR lpszURL)
{
	m_bReady   = false;
	m_strHtml = _T("");

	if (m_spBrowser) {
		COleSafeArray vPostData;
		COleVariant vURL(lpszURL, VT_BSTR);
		COleVariant vHeaders(_T(""), VT_BSTR);
		COleVariant vTargetFrameName(_T(""), VT_BSTR);
		COleVariant vFlags((long)0, VT_I4);

		m_spBrowser->Navigate2(vURL,vFlags, vTargetFrameName, vPostData, vHeaders);
	}
}

// Write Html
void CxBrowser::Write(LPCTSTR lpszHtml)
{
	if (m_spBrowser) {
		m_strHtml = lpszHtml;
		
		WriteImpl();
	}
}

/////////////////////////////////////////////////////////////////////////////
// Message handlers
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CxBrowser,CWnd)
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CxBrowser::PreTranslateMessage(MSG *pMsg)
{
	BOOL bRet = FALSE;

	if (m_spBrowser) {
		CComQIPtr<IOleInPlaceActiveObject> spInPlace = m_spBrowser;
		if (spInPlace) {
			bRet = (spInPlace->TranslateAccelerator(pMsg) == S_OK) ? TRUE : FALSE;
		}
	}
	else {
		bRet = CWnd::PreTranslateMessage(pMsg);
	}

	return bRet;
}

void CxBrowser::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (m_spBrowser) {
        CRect rect(0,0,cx,cy);
		rect.left-=2;	rect.top-=2;
		rect.right++;	rect.bottom++;
		m_wnd.MoveWindow(&rect);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Event handlers
/////////////////////////////////////////////////////////////////////////////

BEGIN_EVENTSINK_MAP(CxBrowser,CWnd)
    ON_EVENT(CxBrowser,AFX_IDW_PANE_FIRST, DISPID_NAVIGATECOMPLETE2, _OnNavigateComplete2, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

void CxBrowser::_OnNavigateComplete2(LPDISPATCH lpDisp, VARIANT *URL)
{
    if (lpDisp == m_spDisp) {
		// signal document ready
		m_bReady = true;
		
		// write current content
		WriteImpl();
		
		// inform user navigation complete
		CString strURL;
		
		if (URL && (V_VT(URL)==VT_BSTR)) {
			strURL = V_BSTR(URL);
		}
    }
}

// Write deferred content
void CxBrowser::WriteImpl()
{
	if (m_bReady && (!m_strHtml.IsEmpty())) {

		CComPtr<IDispatch> spDispDocument;
		m_spBrowser->get_Document(&spDispDocument);

		CComQIPtr<IHTMLDocument2> spDoc = spDispDocument;	
		if (spDoc) {
			// construct text to be written to browser as SAFEARRAY
			SAFEARRAY *safe_array = SafeArrayCreateVector(VT_VARIANT,0,1);
			
			VARIANT	*variant;
			
			SafeArrayAccessData(safe_array,(LPVOID *)&variant);
			
			variant->vt      = VT_BSTR;
			variant->bstrVal = m_strHtml.AllocSysString();
			
			SafeArrayUnaccessData(safe_array);
			
			// write SAFEARRAY to browser document
			
			spDoc->write(safe_array);
			
			::SysFreeString(variant->bstrVal);

			variant->bstrVal = NULL;
			SafeArrayDestroy(safe_array);
		}

		m_strHtml.Empty();
	}
}

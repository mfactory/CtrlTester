// xButton.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "CtrlTester.h"
#include "xButton.h"


// CxButton

IMPLEMENT_DYNAMIC(CxButton, CMFCButton)

CxButton::CxButton()
{

}

CxButton::~CxButton()
{
}


BEGIN_MESSAGE_MAP(CxButton, CMFCButton)
END_MESSAGE_MAP()

void CxButton::OnFillBackground(CDC* pDC, const CRect& rectClient)
{
	//__super::OnFillBackground(pDC, rectClient);
}

void CxButton::OnDrawBorder(CDC* pDC, CRect& rectClient, UINT uiState)
{
	//__super::OnDrawBorder(pDC, rectClient, uiState);

	if (uiState & ODS_DISABLED){
		pDC->FillSolidRect( rectClient, RGB( 192,192,192 ) );
	}
	else if (uiState & ODS_SELECTED){
		pDC->FillSolidRect( rectClient, RGB( 192,192,255 ) );
	}
	else if (m_bChecked){
		pDC->FillSolidRect( rectClient, RGB( 255,192,192 ) );
	}
	else{
		pDC->FillSolidRect( rectClient, RGB( 255,255,255 ) );
	}

	// Draw Border
	pDC->MoveTo( rectClient.left, rectClient.top );
	pDC->LineTo( rectClient.right-1, rectClient.top );
	pDC->LineTo( rectClient.right-1, rectClient.bottom-1 );
	pDC->LineTo( rectClient.left, rectClient.bottom-1 );
	pDC->LineTo( rectClient.left, rectClient.top );

	// Adjust Next Draw Rect(Focus)
	rectClient.DeflateRect(1, 1);
	rectClient.DeflateRect(1, 1);
}

void CxButton::OnDrawFocusRect(CDC* pDC, const CRect& rectClient)
{
	//__super::OnDrawFocusRect(pDC, rectClient);
}
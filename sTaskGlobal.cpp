#include "stdafx.h"

// 新しい DDV の定義
// 文字数が a から b の間にあることを確認する
void AFXAPI DDV_MinMaxChars(CDataExchange* pDX, CString const& value, int minChars, int maxChars)
{
	ASSERT(minChars >= 1);        // allow them something
	ASSERT(maxChars >= 1);        // allow them something
	if (pDX->m_bSaveAndValidate && (value.GetLength() > maxChars || value.GetLength() < minChars))
	{	// エラーの表示
//		CString mes;
//		mes.Format("%sから%s", minChars, maxChars);
//		TCHAR szT[32];
//		wsprintf(szT, _T("%d"), minChars);
		CString prompt;
//		AfxFormatString1(prompt, AFX_IDP_PARSE_STRING_SIZE, szT);
		prompt.Format("%dから%d文字で入力してください", minChars, maxChars);
		AfxMessageBox(prompt, MB_ICONEXCLAMATION, AFX_IDP_PARSE_STRING_SIZE);
		prompt.Empty(); // exception prep
		pDX->Fail();
	}
	else if (pDX->m_hWndLastControl != NULL && pDX->m_bEditLastControl)
	{
		// limit the control max-chars automatically
		::SendMessage(pDX->m_hWndLastControl, EM_LIMITTEXT, maxChars, 0);
	}
}


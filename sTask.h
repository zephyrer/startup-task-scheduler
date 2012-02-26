// **********************************
//  sTask メイン関数 (sTaskApp) の定義
// **********************************
// sTask.h : STASK アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_STASK_H__E19A2865_0616_11D3_9DA0_0090CCA1066A__INCLUDED_)
#define AFX_STASK_H__E19A2865_0616_11D3_9DA0_0090CCA1066A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル
#include "TaskArray.h"	// ClassView によって追加されました。

/////////////////////////////////////////////////////////////////////////////
// CSTaskApp:
// このクラスの動作の定義に関しては sTask.cpp ファイルを参照してください。
//

class CSTaskApp : public CWinApp
{
public:
	unsigned int GenerateRandom(unsigned int min, unsigned int max);
	BOOL UninstallSeq(void);
	BOOL InstallSeq(void);
	BOOL RegReadAll(void);
	BOOL RegWriteAll(void);
	CTaskArray m_tasks;
	CSTaskApp();
	BOOL m_bMutexOwner;
	HANDLE m_hMutex;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSTaskApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
	//{{AFX_MSG(CSTaskApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void HtmlHelp(DWORD dwData,UINT nCmd);

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STASK_H__E19A2865_0616_11D3_9DA0_0090CCA1066A__INCLUDED_)

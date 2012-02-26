#if !defined(AFX_STASKPROGRESS_H__0C843A60_1793_11D3_9DA0_0090CCA1066A__INCLUDED_)
#define AFX_STASKPROGRESS_H__0C843A60_1793_11D3_9DA0_0090CCA1066A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// sTaskProgress.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress ダイアログ

class CDlgProgress : public CDialog
{
// コンストラクション
public:
	BOOL m_is_in_thread;
	int *m_progress_data;
	HWND * ptr_m_progress;
	HWND * ptr_m_dialog;
	HWND * ptr_m_lfn;
	HWND * ptr_m_mes;
	CDlgProgress(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgProgress)
	enum { IDD = IDD_DLG_NORMAL };
	CProgressCtrl	m_progress_ctrl;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgProgress)
	public:
	virtual BOOL Create(CWnd *wnd=NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgProgress)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STASKPROGRESS_H__0C843A60_1793_11D3_9DA0_0090CCA1066A__INCLUDED_)

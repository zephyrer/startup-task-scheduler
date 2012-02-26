#if !defined(AFX_STASKDLG_H__A8C9BBC1_15EA_11D3_9DA0_0090CCA1066A__INCLUDED_)
#define AFX_STASKDLG_H__A8C9BBC1_15EA_11D3_9DA0_0090CCA1066A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// sTaskDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDebug ダイアログ

class CDlgDebug : public CDialog
{
// コンストラクション
public:
	BOOL m_disable_cancel;
	CDlgDebug(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgDebug)
	enum { IDD = IDD_DEBUG };
	CButton	m_btn_cancel;
	CTime	m_date;
	CTime	m_tim;
	BOOL	m_dlg;
	BOOL	m_dlg0;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgDebug)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgDebug)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDlgPasswd ダイアログ

class CDlgPasswd : public CDialog
{
// コンストラクション
public:
	CDlgPasswd(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgPasswd)
	enum { IDD = IDD_DLG_PASSWD };
	CString	m_passwd;
	CString	m_message;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgPasswd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgPasswd)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CDlgInstall ダイアログ

class CDlgInstall : public CDialog
{
// コンストラクション
public:
	BOOL m_IsAdminInst;
	CDlgInstall(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgInstall)
	enum { IDD = IDD_DLG_INST };
	CButton	m_r_autoexec_ctrl1;		// 手動で追加
	CButton	m_r_autoexec_ctrl2;		// 手動で追加
	CButton	m_r_autoexec_ctrl3;		// 手動で追加
	CButton	m_r_autoexec_ctrl4;		// 手動で追加
	BOOL	m_chk_uninst_cpl;
	BOOL	m_chk_uninst_start;
	int		m_r_autoexec;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgInstall)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgInstall)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_chk_protect;
	CString m_edit_password;
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STASKDLG_H__A8C9BBC1_15EA_11D3_9DA0_0090CCA1066A__INCLUDED_)

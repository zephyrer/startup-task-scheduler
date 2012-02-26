#if !defined(AFX_STASKPROP_H__2B95E761_0649_11D3_9DA0_0090CCA1066A__INCLUDED_)
#define AFX_STASKPROP_H__2B95E761_0649_11D3_9DA0_0090CCA1066A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// sTaskProp.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPropAdd1 ダイアログ

class CPropAdd1 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropAdd1)

// コンストラクション
public:
	int *trn_sel;
	CComboBox *trn_winfile;
	CComboBox *trn_recom;
	BOOL is_new;
	CPropAdd1();
	~CPropAdd1();

// ダイアログ データ
	//{{AFX_DATA(CPropAdd1)
	enum { IDD = IDD_PROP_ADD1 };
	CButton	m_btn_execdir_ctrl;
	CComboBox	m_cmb_execdir_ctrl;
	CEdit	m_execdir_ctrl;
	CString	m_name;
	CString	m_fpass;
	CString	m_param;
	CString	m_mes;
	BOOL	m_exec;
	int		m_cmb_wndstyle;
	int		m_execdir_mode;
	CString	m_execdir;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CPropAdd1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPropAdd1)
	afx_msg void OnBtnBrowse();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSelchangeCmbExecdir();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnExecdir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CPropAdd2 ダイアログ

class CPropAdd2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropAdd2)

// コンストラクション
public:
	CString *trn_fpass;
	CString *trn_name;
	CPropAdd2();
	~CPropAdd2();

// ダイアログ データ
	//{{AFX_DATA(CPropAdd2)
	enum { IDD = IDD_PROP_ADD2 };
	CDateTimeCtrl	m_time1_w;
	CDateTimeCtrl	m_time0_w;
	CEdit	m_ctrl_interval;
	CStatic	m_ctrl_apart;
	CComboBox	m_cmb_apart;
	BOOL	m_dfri;
	BOOL	m_dmon;
	BOOL	m_dsun;
	BOOL	m_dthu;
	BOOL	m_dsat;
	BOOL	m_dtue;
	BOOL	m_dwed;
	BOOL	m_time;
	CTime	m_time0;
	CTime	m_time1;
	int		m_apart;
	UINT	m_interval;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CPropAdd2)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPropAdd2)
	afx_msg void OnSelchangeComboApart();
	afx_msg void OnCheckTime();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CPropAdd3 ダイアログ

class CPropAdd3 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropAdd3)

// コンストラクション
public:
	CPropAdd3();
	~CPropAdd3();

// ダイアログ データ
	//{{AFX_DATA(CPropAdd3)
	enum { IDD = IDD_PROP_ADD3 };
	CEdit	m_taskoffcount_w;
	BOOL	m_syncprev;
	BOOL	m_dialog;
	BOOL	m_taskoff;
	BOOL	m_waitexit;
	UINT	m_taskoffcount;
	UINT	m_waitsec;
	CTime	m_lastexec_date;
	CTime	m_lastexec_time;
	UINT	m_cnt_check;
	UINT	m_cnt_exec;
	int		m_cmb_syncexec;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CPropAdd3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPropAdd3)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CPropCnf1 ダイアログ

class CPropCnf1 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropCnf1)

// コンストラクション
public:
	CPropCnf1();
	~CPropCnf1();

// ダイアログ データ
	//{{AFX_DATA(CPropCnf1)
	enum { IDD = IDD_PROP_CNF1 };
	CButton	m_btn_curdir_ctrl;
	CComboBox	m_cmb_curdir_ctrl;
	CEdit	m_edit_curdir_ctrl;
	BOOL	m_dialog;
	BOOL	m_dialog0;
	BOOL	m_faildlg;
	UINT	m_delay;
	int		m_time;
	int		m_cmb_curdir;
	CString	m_curdir;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CPropCnf1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPropCnf1)
	afx_msg void OnBtnCurdir();
	afx_msg void OnSelchangeCmbCurdir();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CPropCnf2 ダイアログ

class CPropCnf2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropCnf2)

// コンストラクション
public:
	CPropCnf2();
	~CPropCnf2();

// ダイアログ データ
	//{{AFX_DATA(CPropCnf2)
	enum { IDD = IDD_PROP_CNF2 };
	BOOL	m_secure;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CPropCnf2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPropCnf2)
	afx_msg void OnBtnPass();
	afx_msg void OnGSecure();
	afx_msg void OnBtnMkunin();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CPropAdd4 ダイアログ

class CPropAdd4 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropAdd4)

// コンストラクション
public:
	CString *trn_name;
	CPropAdd4();
	~CPropAdd4();

// ダイアログ データ
	//{{AFX_DATA(CPropAdd4)
	enum { IDD = IDD_PROP_ADD4 };
	CEdit	m_taskoffcount_w;
	BOOL	m_syncprev;
	BOOL	m_dialog;
	BOOL	m_taskoff;
	BOOL	m_waitexit;
	UINT	m_waitsec;
	UINT	m_taskoffcount;
	int		m_cmb_syncexec;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CPropAdd4)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPropAdd4)
	afx_msg void OnCheckStop();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropAdd0 ダイアログ

class CPropAdd0 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropAdd0)

// コンストラクション
public:
	BOOL ScanFile(char *buf);
	CPropAdd0();
	~CPropAdd0();

// ダイアログ データ
	//{{AFX_DATA(CPropAdd0)
	enum { IDD = IDD_PROP_ADD0 };
	CButton	m_r_sel1_ctrl;
	CButton	m_r_sel2_ctrl;
	CComboBox	m_winfile;
	CComboBox	m_recom;
	int		m_select;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CPropAdd0)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPropAdd0)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CPropAdd5 ダイアログ

class CPropAdd5 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropAdd5)

// コンストラクション
public:
	CPropAdd5();
	~CPropAdd5();
	CString *trn_fpass;
	CString *trn_name;

// ダイアログ データ
	//{{AFX_DATA(CPropAdd5)
	enum { IDD = IDD_PROP_ADD5 };
		// メモ - ClassWizard はこの位置にデータ メンバを追加します。
		//    この位置に生成されるコードを編集しないでください。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CPropAdd5)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPropAdd5)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


int CALLBACK myBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
LPITEMIDLIST ConvertPIDL(char *lpszPath);


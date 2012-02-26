// **********************************
// ダイアログ CDlgDebug, CDlgPasswd, CDlgInstall
// **********************************

#include "stdafx.h"
#include "sTask.h"
#include "sTaskDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// **********************************
// CDlgDebug ダイアログ
// **********************************
/////////////////////////////////////////////////////////////////////////////
CDlgDebug::CDlgDebug(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDebug::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDebug)
	m_date = 0;
	m_tim = 0;
	m_dlg = FALSE;
	m_dlg0 = FALSE;
	//}}AFX_DATA_INIT
	m_disable_cancel = FALSE;
}


void CDlgDebug::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDebug)
	DDX_Control(pDX, IDCANCEL, m_btn_cancel);
	DDX_DateTimeCtrl(pDX, IDC_NOW_DATE, m_date);
	DDX_DateTimeCtrl(pDX, IDC_NOW_TIME, m_tim);
	DDX_Check(pDX, IDC_DLG, m_dlg);
	DDX_Check(pDX, IDC_DLG0, m_dlg0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDebug, CDialog)
	//{{AFX_MSG_MAP(CDlgDebug)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDlgDebug::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	// キャンセルボタンを押せないようにする
	if(m_disable_cancel) m_btn_cancel.EnableWindow(FALSE);
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}



// **********************************
// CDlgPasswd ダイアログ
// **********************************
/////////////////////////////////////////////////////////////////////////////
CDlgPasswd::CDlgPasswd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPasswd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPasswd)
	m_passwd = _T("");
	m_message = _T("");
	//}}AFX_DATA_INIT
}


void CDlgPasswd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPasswd)
	DDX_Text(pDX, IDC_EDIT, m_passwd);
	DDV_MaxChars(pDX, m_passwd, 20);
	DDX_Text(pDX, IDC_MESSAGE, m_message);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPasswd, CDialog)
	//{{AFX_MSG_MAP(CDlgPasswd)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// **********************************
// CDlgInstall ダイアログ
// **********************************
/////////////////////////////////////////////////////////////////////////////
CDlgInstall::CDlgInstall(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInstall::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInstall)
	m_chk_uninst_cpl = FALSE;
	m_chk_uninst_start = FALSE;
	m_r_autoexec = -1;
	m_chk_protect = FALSE;
	m_edit_password = _T("0000");
	//}}AFX_DATA_INIT
}

void CDlgInstall::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInstall)
	DDX_Control(pDX, IDC_R_AUTOEXEC, m_r_autoexec_ctrl1);	// 手動で追加
	DDX_Control(pDX, IDC_R_AUTOEXEC2, m_r_autoexec_ctrl2);	// 手動で追加
	DDX_Control(pDX, IDC_R_AUTOEXEC3, m_r_autoexec_ctrl3);	// 手動で追加
	DDX_Control(pDX, IDC_R_AUTOEXEC4, m_r_autoexec_ctrl4);	// 手動で追加
	DDX_Check(pDX, IDC_CHECK_UNINST_CPL, m_chk_uninst_cpl);
	DDX_Check(pDX, IDC_CHECK_UNINST_START, m_chk_uninst_start);
	DDX_Radio(pDX, IDC_R_AUTOEXEC, m_r_autoexec);
	DDX_Check(pDX, IDC_CHECK_PROTECT, m_chk_protect);
	DDV_MaxChars(pDX, m_edit_password, 20);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_edit_password);
}

BEGIN_MESSAGE_MAP(CDlgInstall, CDialog)
	//{{AFX_MSG_MAP(CDlgInstall)
	ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// **********************************
// ダイアログ初期化に実行
// インストールダイアログの不必要項目を無効化
// **********************************
BOOL CDlgInstall::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	// *******************
	// HKLM...run に書き込み権限の無いときは、アドミニストレータインストールを無効化
	// *******************
	if(!m_IsAdminInst) m_r_autoexec_ctrl4.EnableWindow(FALSE);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


// ************************************************************
// ヘルプ表示関数
// ************************************************************
void CDlgInstall::OnBtnHelp() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	this->HtmlHelp(0,HH_DISPLAY_TOPIC);

}

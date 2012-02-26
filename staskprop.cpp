// **********************************
// プロパティ・ダイアログ CPropAdd0, CPropAdd1, CPropAdd2, CPropAdd3, CPropAdd4,
//                        CPropCnf1, CPropCnf2
// **********************************

#include "stdafx.h"
#include "sTask.h"
#include "sTaskProp.h"
#include "sTaskDlg.h"
#include "sTaskGlobal.h"
#include "InstFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// **********************************
// CPropAdd0 プロパティ ページ
// **********************************
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CPropAdd0, CPropertyPage)

CPropAdd0::CPropAdd0() : CPropertyPage(CPropAdd0::IDD)
{
	//{{AFX_DATA_INIT(CPropAdd0)
	m_select = -1;
	//}}AFX_DATA_INIT
}

CPropAdd0::~CPropAdd0()
{
}

void CPropAdd0::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropAdd0)
	DDX_Control(pDX, IDC_R_SEL1, m_r_sel1_ctrl);
	DDX_Control(pDX, IDC_R_SEL2, m_r_sel2_ctrl);	// 手動で追加
	DDX_Control(pDX, IDC_WINFILE, m_winfile);
	DDX_Control(pDX, IDC_RECOMMEND, m_recom);
	DDX_Radio(pDX, IDC_R_SEL1, m_select);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropAdd0, CPropertyPage)
	//{{AFX_MSG_MAP(CPropAdd0)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPropAdd0::OnInitDialog() 
{
	CString strTemp;
	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	// **********************************
	// お勧めタスクを追加する。（ファイルがあるかどうか確認）
	// **********************************
	// Win98/2000 のおすすめファイル
	strTemp.LoadString(STR_NAME_DISKCLEAN);	// ディスクのクリーンアップ
	if(ScanFile("CLEANMGR.EXE")) m_recom.AddString(strTemp);

	// Win2000 のおすすめファイル
	strTemp.LoadString(STR_NAME_DEFRAG2000);	// デフラグ
	if(ScanFile("DFRG.MSC")) m_recom.AddString(strTemp);
	strTemp.LoadString(STR_NAME_EVENTVWR);	// イベントビュア
	if(ScanFile("EVENTVWR.MSC")) m_recom.AddString(strTemp);
	strTemp.LoadString(STR_NAME_SVC);	// サービスの起動と停止
	if(ScanFile("SERVICES.MSC")) m_recom.AddString(strTemp);

	// Win98 のおすすめファイル
	strTemp.LoadString(STR_NAME_SCANDISK);	// スキャンディスク
	if(ScanFile("SCANDSKW.EXE")) m_recom.AddString(strTemp);
	strTemp.LoadString(STR_NAME_DEFRAG);	// ディスクのデフラグ
	if(ScanFile("DEFRAG.EXE")) m_recom.AddString(strTemp);
	strTemp.LoadString(STR_NAME_SYSFILCHK);	// システムファイルチェッカー
	if(ScanFile("SYSTEM\\SFC.EXE")) m_recom.AddString(strTemp);
	strTemp.LoadString(STR_NAME_REGCHK);	// レジストリチェッカー
	if(ScanFile("SCANREGW.EXE")) m_recom.AddString(strTemp);
	m_recom.SetCurSel(0);

	if(m_recom.GetCount() <= 0)
	{
		m_r_sel1_ctrl.EnableWindow(FALSE);
		m_recom.EnableWindow(FALSE);
		m_r_sel1_ctrl.SetCheck(FALSE);
		m_r_sel2_ctrl.SetCheck(TRUE);
	}

	char path[MAX_PATH+1];
	::GetWindowsDirectory(path,MAX_PATH);
	strcat(path,"\\*.exe");
	m_winfile.Dir(0,path);
	m_winfile.SetCurSel(0);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// **********************************
// ファイルが存在すれば TRUE を返す関数  （お勧めタスク追加用）
// **********************************
BOOL CPropAdd0::ScanFile(char *buf)
{
	WIN32_FIND_DATA fdata;
	HANDLE hFind;
	char path[MAX_PATH+1];
	CString fname;

	// Windows フォルダのスキャン

	::GetWindowsDirectory(path,MAX_PATH);

	fname.Format("%s\\%s", path, buf);
	hFind = ::FindFirstFile(fname, &fdata);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		::FindClose(hFind);
		return TRUE;
	}

	// system フォルダのスキャン

	::GetSystemDirectory(path,MAX_PATH);

	fname.Format("%s\\%s", path, buf);
	hFind = ::FindFirstFile(fname, &fdata);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		::FindClose(hFind);
		return TRUE;
	}

	// 見つからなかった
	return FALSE;
}

// **********************************
// CPropAdd1 プロパティ ページ
// **********************************
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CPropAdd1, CPropertyPage)

CPropAdd1::CPropAdd1() : CPropertyPage(CPropAdd1::IDD)
{
	//{{AFX_DATA_INIT(CPropAdd1)
	m_name = _T("");
	m_fpass = _T("");
	m_param = _T("");
	m_mes = _T("");
	m_exec = FALSE;
	m_cmb_wndstyle = -1;
	m_execdir_mode = -1;
	m_execdir = _T("");
	m_cmb_powerstat = -1;
	//}}AFX_DATA_INIT
}

CPropAdd1::~CPropAdd1()
{
}

void CPropAdd1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropAdd1)
	DDX_Control(pDX, IDC_BTN_EXECDIR, m_btn_execdir_ctrl);
	DDX_Control(pDX, IDC_CMB_EXECDIR, m_cmb_execdir_ctrl);
	DDX_Control(pDX, IDC_EDIT_EXECDIR, m_execdir_ctrl);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDV_MinMaxChars(pDX, m_name, 1, 50);
	DDX_Text(pDX, IDC_EDIT_FPASS, m_fpass);
	DDV_MinMaxChars(pDX, m_fpass, 1, 250);
	DDX_Text(pDX, IDC_EDIT_PARAM, m_param);
	DDV_MaxChars(pDX, m_param, 100);
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_mes);
	DDV_MaxChars(pDX, m_mes, 100);
	DDX_Check(pDX, IDC_CHECK_ON, m_exec);
	DDX_CBIndex(pDX, IDC_CMB_WNDSTYLE, m_cmb_wndstyle);
	DDX_CBIndex(pDX, IDC_CMB_EXECDIR, m_execdir_mode);
	DDX_Text(pDX, IDC_EDIT_EXECDIR, m_execdir);
	DDX_CBIndex(pDX, IDC_CMB_POWERSTAT, m_cmb_powerstat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropAdd1, CPropertyPage)
	//{{AFX_MSG_MAP(CPropAdd1)
	ON_BN_CLICKED(IDC_BTN_BROWSE, OnBtnBrowse)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_CMB_EXECDIR, OnSelchangeCmbExecdir)
	ON_BN_CLICKED(IDC_BTN_EXECDIR, OnBtnExecdir)
	ON_BN_CLICKED(IDC_BTN_POWERTEST, OnBtnPowertest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// **********************************
// プログラムの参照ボタン
// **********************************
void CPropAdd1::OnBtnBrowse() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString strTmp1;
	strTmp1.LoadString(STR_FIL_SEL);
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_EXPLORER,strTmp1);
	if(dlg.DoModal()==TRUE)
		SetDlgItemText(IDC_EDIT_FPASS, dlg.GetPathName());
}

// **********************************
// 「新しいタスクの追加」の最初のウイザードページからの情報を受け取り、ファイル名などを設定する
// **********************************
void CPropAdd1::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	char path[MAX_PATH];
	CString buf1,buf2,strTmp1;

	if(m_fpass != "") return;
	if(*trn_sel == 2) return;	// 参照モード
	if(*trn_sel == 1)
	{
		trn_winfile->GetLBText(trn_winfile->GetCurSel(), path);
		buf1 = path;
		::GetWindowsDirectory(path,MAX_PATH);
		buf2.Format("%s\\%s", path, buf1);
		SetDlgItemText(IDC_EDIT_FPASS, buf2);
	}
	if(*trn_sel == 0)
	{
		trn_recom->GetLBText(trn_recom->GetCurSel(), path);
		buf1 = path;
		::GetWindowsDirectory(path,MAX_PATH);
		// Win98/2000 のおすすめタスク
		strTmp1.LoadString(STR_NAME_DISKCLEAN);
		if(buf1 == strTmp1) buf2 = "CLEANMGR.EXE";	// ディスクのクリーンアップ
		// Win2000 のおすすめタスク
		strTmp1.LoadString(STR_NAME_DEFRAG2000);
		if(buf1 == strTmp1) buf2 = "DFRG.MSC";	// デフラグ
		strTmp1.LoadString(STR_NAME_EVENTVWR);
		if(buf1 == strTmp1) buf2 = "EVENTVWR.MSC";	// イベントビュア
		strTmp1.LoadString(STR_NAME_SVC);
		if(buf1 == strTmp1) buf2 = "SERVICES.MSC";	// サービスの起動と停止
		// Win98 のおすすめタスク
		strTmp1.LoadString(STR_NAME_SCANDISK);	// スキャンディスク
		if(buf1 == strTmp1)
		{
			buf2 = "SCANDSKW.EXE";
			SetDlgItemText(IDC_EDIT_PARAM, "/a /n");
		}
		strTmp1.LoadString(STR_NAME_DEFRAG);	// ディスクのデフラグ
		if(buf1 == strTmp1) buf2 = "DEFRAG.EXE";
		strTmp1.LoadString(STR_NAME_SYSFILCHK);
		if(buf1 == strTmp1) buf2 = "SFC.EXE";	// システムファイルチェッカー
		strTmp1.LoadString(STR_NAME_REGCHK);
		if(buf1 == strTmp1) buf2 = "SCANREGW.EXE";	// レジストリチェッカー
		SetDlgItemText(IDC_EDIT_FPASS, buf2);
		SetDlgItemText(IDC_EDIT_NAME, buf1);
	}
}


// **********************************
// CPropAdd2 プロパティ ページ
// **********************************
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CPropAdd2, CPropertyPage)

CPropAdd2::CPropAdd2() : CPropertyPage(CPropAdd2::IDD)
{
	//{{AFX_DATA_INIT(CPropAdd2)
	m_dfri = FALSE;
	m_dmon = FALSE;
	m_dsun = FALSE;
	m_dthu = FALSE;
	m_dsat = FALSE;
	m_dtue = FALSE;
	m_dwed = FALSE;
	m_time = FALSE;
	m_time0 = 0;
	m_time1 = 0;
	m_apart = -1;
	m_interval = 0;
	//}}AFX_DATA_INIT
}

CPropAdd2::~CPropAdd2()
{
}

void CPropAdd2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropAdd2)
	DDX_Control(pDX, IDC_INPUT_TIME1, m_time1_w);
	DDX_Control(pDX, IDC_INOUT_TIME0, m_time0_w);
	DDX_Control(pDX, IDC_EDIT_INTERVAL, m_ctrl_interval);
	DDX_Control(pDX, IDC_TEXT_APART, m_ctrl_apart);
	DDX_Control(pDX, IDC_COMBO_APART, m_cmb_apart);
	DDX_Check(pDX, IDC_CHECK_FRI, m_dfri);
	DDX_Check(pDX, IDC_CHECK_MON, m_dmon);
	DDX_Check(pDX, IDC_CHECK_SUN, m_dsun);
	DDX_Check(pDX, IDC_CHECK_THU, m_dthu);
	DDX_Check(pDX, IDC_CHECK_SAT, m_dsat);
	DDX_Check(pDX, IDC_CHECK_TUE, m_dtue);
	DDX_Check(pDX, IDC_CHECK_WED, m_dwed);
	DDX_Check(pDX, IDC_CHECK_TIME, m_time);
	DDX_DateTimeCtrl(pDX, IDC_INOUT_TIME0, m_time0);
	DDX_DateTimeCtrl(pDX, IDC_INPUT_TIME1, m_time1);
	DDX_CBIndex(pDX, IDC_COMBO_APART, m_apart);
	DDX_Text(pDX, IDC_EDIT_INTERVAL, m_interval);
	DDV_MinMaxUInt(pDX, m_interval, 1, 31);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropAdd2, CPropertyPage)
	//{{AFX_MSG_MAP(CPropAdd2)
	ON_CBN_SELCHANGE(IDC_COMBO_APART, OnSelchangeComboApart)
	ON_BN_CLICKED(IDC_CHECK_TIME, OnCheckTime)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// **********************************
// スケジュール単位のコンボボックスが変わったら
// **********************************
void CPropAdd2::OnSelchangeComboApart() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CString strTmp1;
	// タスクのスケジュールの種類による選択項目の表示・非表示
	int i=m_cmb_apart.GetCurSel();
	switch(i)
	{
	case 0	:	// 起動ごと
		strTmp1.LoadString(STR_PER_LOGON);
		SetDlgItemText(IDC_TEXT_APART, strTmp1);
		break;
	case 1	:	// 日ごと
		strTmp1.LoadString(STR_PER_DAY);
		SetDlgItemText(IDC_TEXT_APART, strTmp1);
		break;
	case 2	:	// 週ごと
		strTmp1.LoadString(STR_PER_WEEK);
		SetDlgItemText(IDC_TEXT_APART, strTmp1);
		break;
	case 3	:	// 月ごと
		strTmp1.LoadString(STR_PER_MONTH);
		SetDlgItemText(IDC_TEXT_APART, strTmp1);
		break;
	case 4	:	// ランダム
		strTmp1.LoadString(STR_PER_RANDOM);
		SetDlgItemText(IDC_TEXT_APART, strTmp1);
		break;
	}
	if(i != 4)
	{
		m_ctrl_apart.EnableWindow(TRUE);
		m_ctrl_interval.EnableWindow(TRUE);
	}
	else
	{	// ランダム値のときは編集できないようにする
		m_ctrl_apart.EnableWindow(FALSE);
		m_ctrl_interval.EnableWindow(FALSE);
	}
}

// **********************************
// 時間除外エディットのグレイ状態
// **********************************
void CPropAdd2::OnCheckTime() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	// 時間除外指定による表示・非表示
	if(!IsDlgButtonChecked(IDC_CHECK_TIME))
	{
		m_time0_w.EnableWindow(FALSE);	
		m_time1_w.EnableWindow(FALSE);
	}
	else
	{
		m_time0_w.EnableWindow(TRUE);
		m_time1_w.EnableWindow(TRUE);
	}
	
}

void CPropAdd2::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPropertyPage::OnShowWindow(bShow, nStatus);

	CString strTmp1;
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	// タスクのスケジュールの種類による選択項目の表示・非表示
	int i=m_cmb_apart.GetCurSel();
	switch(i)
	{
	case 0	:	// 起動ごと
		strTmp1.LoadString(STR_PER_LOGON);
		SetDlgItemText(IDC_TEXT_APART, strTmp1);
		break;
	case 1	:	// 日ごと
		strTmp1.LoadString(STR_PER_DAY);
		SetDlgItemText(IDC_TEXT_APART, strTmp1);
		break;
	case 2	:	// 週ごと
		strTmp1.LoadString(STR_PER_WEEK);
		SetDlgItemText(IDC_TEXT_APART, strTmp1);
		break;
	case 3	:	// 月ごと
		strTmp1.LoadString(STR_PER_MONTH);
		SetDlgItemText(IDC_TEXT_APART, strTmp1);
		break;
	case 4	:	// ランダム
		strTmp1.LoadString(STR_PER_RANDOM);
		SetDlgItemText(IDC_TEXT_APART, strTmp1);
		break;
	}
	if(i != 4)
	{
		m_ctrl_apart.EnableWindow(TRUE);
		m_ctrl_interval.EnableWindow(TRUE);
	}
	else
	{	// ランダム値のときは編集できないようにする
		m_ctrl_apart.EnableWindow(FALSE);
		m_ctrl_interval.EnableWindow(FALSE);
	}
	
	// 時間除外指定による表示・非表示
	if(!IsDlgButtonChecked(IDC_CHECK_TIME))
	{
		m_time0_w.EnableWindow(FALSE);	
		m_time1_w.EnableWindow(FALSE);
	}

	// タイトル名の設定
	SetDlgItemText(IDC_PROP1_NAME,*trn_name);
}

// **********************************
// CPropAdd3 プロパティ ページ
// **********************************
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CPropAdd3, CPropertyPage)

CPropAdd3::CPropAdd3() : CPropertyPage(CPropAdd3::IDD)
{
	//{{AFX_DATA_INIT(CPropAdd3)
	m_syncprev = FALSE;
	m_dialog = FALSE;
	m_taskoff = FALSE;
	m_waitexit = FALSE;
	m_taskoffcount = 0;
	m_waitsec = 0;
	m_lastexec_date = 0;
	m_lastexec_time = 0;
	m_cnt_check = 0;
	m_cnt_exec = 0;
	m_cmb_syncexec = -1;
	//}}AFX_DATA_INIT
}

CPropAdd3::~CPropAdd3()
{
}

void CPropAdd3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropAdd3)
	DDX_Control(pDX, IDC_EDIT_STOP, m_taskoffcount_w);
	DDX_Check(pDX, IDC_CHECK_PREVTASK, m_syncprev);
	DDX_Check(pDX, IDC_CHECK_STDLG, m_dialog);
	DDX_Check(pDX, IDC_CHECK_STOP, m_taskoff);
	DDX_Check(pDX, IDC_CHECK_WAIT, m_waitexit);
	DDX_Text(pDX, IDC_EDIT_STOP, m_taskoffcount);
	DDV_MinMaxUInt(pDX, m_taskoffcount, 0, 100);
	DDX_Text(pDX, IDC_EDIT_WAIT, m_waitsec);
	DDV_MinMaxUInt(pDX, m_waitsec, 0, 120);
	DDX_DateTimeCtrl(pDX, IDC_EDIT_TIME1, m_lastexec_date);
	DDX_DateTimeCtrl(pDX, IDC_EDIT_TIME2, m_lastexec_time);
	DDX_Text(pDX, IDC_EDIT_CHECK, m_cnt_check);
	DDX_Text(pDX, IDC_EDIT_EXEC, m_cnt_exec);
	DDX_CBIndex(pDX, IDC_CMB_SYNCEXEC, m_cmb_syncexec);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropAdd3, CPropertyPage)
	//{{AFX_MSG_MAP(CPropAdd3)
	ON_BN_CLICKED(IDC_CHECK_STOP, OnCheckStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPropAdd3::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	// 自動停止指定による表示・非表示
	if(!IsDlgButtonChecked(IDC_CHECK_STOP))
		m_taskoffcount_w.EnableWindow(FALSE);	

	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CPropAdd3::OnCheckStop() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	// 自動停止指定による表示・非表示
	if(!IsDlgButtonChecked(IDC_CHECK_STOP))
		m_taskoffcount_w.EnableWindow(FALSE);	
	else m_taskoffcount_w.EnableWindow(TRUE);	
	
}

// **********************************
// CPropAdd4 プロパティ ページ
// **********************************
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CPropAdd4, CPropertyPage)

CPropAdd4::CPropAdd4() : CPropertyPage(CPropAdd4::IDD)
{
	//{{AFX_DATA_INIT(CPropAdd4)
	m_syncprev = FALSE;
	m_dialog = FALSE;
	m_taskoff = FALSE;
	m_waitexit = FALSE;
	m_waitsec = 0;
	m_taskoffcount = 0;
	m_cmb_syncexec = -1;
	//}}AFX_DATA_INIT
}

CPropAdd4::~CPropAdd4()
{
}

void CPropAdd4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropAdd4)
	DDX_Control(pDX, IDC_EDIT_STOP, m_taskoffcount_w);
	DDX_Check(pDX, IDC_CHECK_PREVTASK, m_syncprev);
	DDX_Check(pDX, IDC_CHECK_STDLG, m_dialog);
	DDX_Check(pDX, IDC_CHECK_STOP, m_taskoff);
	DDX_Check(pDX, IDC_CHECK_WAIT, m_waitexit);
	DDX_Text(pDX, IDC_EDIT_WAIT, m_waitsec);
	DDV_MinMaxUInt(pDX, m_waitsec, 0, 120);
	DDX_Text(pDX, IDC_EDIT_STOP, m_taskoffcount);
	DDV_MinMaxUInt(pDX, m_taskoffcount, 0, 100);
	DDX_CBIndex(pDX, IDC_CMB_SYNCEXEC, m_cmb_syncexec);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropAdd4, CPropertyPage)
	//{{AFX_MSG_MAP(CPropAdd4)
	ON_BN_CLICKED(IDC_CHECK_STOP, OnCheckStop)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPropAdd4::OnSetActive() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	// ウィザード形式のボタン「次」「完了」などの制御
	CPropertySheet* pSheet = (CPropertySheet*)GetParent();
	ASSERT_KINDOF(CPropertySheet, pSheet);
	pSheet->SetWizardButtons( PSWIZB_BACK | PSWIZB_NEXT);	
//	pSheet->SetWizardButtons( PSWIZB_BACK | PSWIZB_NEXT | PSWIZB_FINISH);
	return CPropertyPage::OnSetActive();
}


BOOL CPropAdd2::OnSetActive() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	CPropertySheet* pSheet = (CPropertySheet*)GetParent();
	ASSERT_KINDOF(CPropertySheet, pSheet);
	pSheet->SetWizardButtons( PSWIZB_BACK | PSWIZB_NEXT);	
	return CPropertyPage::OnSetActive();
}

void CPropAdd4::OnCheckStop() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	// 自動停止指定による表示・非表示
	if(!IsDlgButtonChecked(IDC_CHECK_STOP))
		m_taskoffcount_w.EnableWindow(FALSE);	
	else m_taskoffcount_w.EnableWindow(TRUE);	
}

BOOL CPropAdd4::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	if(!IsDlgButtonChecked(IDC_CHECK_STOP))
		m_taskoffcount_w.EnableWindow(FALSE);	
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CPropAdd4::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	// タイトル名の設定
	SetDlgItemText(IDC_PROP1_NAME,*trn_name);
	
}

// **********************************
// CPropCnf1 プロパティ ページ
// **********************************
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CPropCnf1, CPropertyPage)

CPropCnf1::CPropCnf1() : CPropertyPage(CPropCnf1::IDD)
{
	//{{AFX_DATA_INIT(CPropCnf1)
	m_dialog = FALSE;
	m_dialog0 = FALSE;
	m_faildlg = FALSE;
	m_delay = 0;
	m_time = -1;
	m_cmb_curdir = -1;
	m_curdir = _T("");
	//}}AFX_DATA_INIT
}

CPropCnf1::~CPropCnf1()
{
}

void CPropCnf1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropCnf1)
	DDX_Control(pDX, IDC_BTN_CURDIR, m_btn_curdir_ctrl);
	DDX_Control(pDX, IDC_CMB_CURDIR, m_cmb_curdir_ctrl);
	DDX_Control(pDX, IDC_EDIT_CURDIR, m_edit_curdir_ctrl);
	DDX_Check(pDX, IDC_DLG, m_dialog);
	DDX_Check(pDX, IDC_DLG0, m_dialog0);
	DDX_Check(pDX, IDC_G_FAILDLG, m_faildlg);
	DDX_Text(pDX, IDC_G_DELAY, m_delay);
	DDV_MinMaxUInt(pDX, m_delay, 0, 120);
	DDX_Radio(pDX, IDC_G_TIME1, m_time);
	DDX_CBIndex(pDX, IDC_CMB_CURDIR, m_cmb_curdir);
	DDX_Text(pDX, IDC_EDIT_CURDIR, m_curdir);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropCnf1, CPropertyPage)
	//{{AFX_MSG_MAP(CPropCnf1)
	ON_BN_CLICKED(IDC_BTN_CURDIR, OnBtnCurdir)
	ON_CBN_SELCHANGE(IDC_CMB_CURDIR, OnSelchangeCmbCurdir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// **********************************
// CPropCnf2 プロパティ ページ
// **********************************
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CPropCnf2, CPropertyPage)

CPropCnf2::CPropCnf2() : CPropertyPage(CPropCnf2::IDD)
{
	//{{AFX_DATA_INIT(CPropCnf2)
	m_secure = FALSE;
	//}}AFX_DATA_INIT
}

CPropCnf2::~CPropCnf2()
{
}

void CPropCnf2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropCnf2)
	DDX_Check(pDX, IDC_G_SECURE, m_secure);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropCnf2, CPropertyPage)
	//{{AFX_MSG_MAP(CPropCnf2)
	ON_BN_CLICKED(IDC_BTN_PASS, OnBtnPass)
	ON_BN_CLICKED(IDC_G_SECURE, OnGSecure)
	ON_BN_CLICKED(IDC_BTN_MKUNIN, OnBtnMkunin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// **********************************
// パスワードの変更
// **********************************
void CPropCnf2::OnBtnPass() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CSTaskApp *theApp;
	theApp = (CSTaskApp *)AfxGetApp();

	CString strTmp1, strTmp2;
	CDlgPasswd dlg;

	dlg.m_message.LoadString(STR_PAS_INPOLD);
	if((dlg.DoModal() == TRUE) && (dlg.m_passwd == theApp->m_tasks.g_passwd)) ;
	else
	{	// パスワードの変更を受け付けない場合
		strTmp1.LoadString(STR_ERRPAS);
		strTmp2.LoadString(STR_APPNAME);
		::MessageBox(NULL, strTmp1, strTmp2, MB_OK|MB_ICONSTOP|MB_APPLMODAL);
		return;
	}
	dlg.m_passwd = "";
	dlg.m_message.LoadString(STR_PAS_IMPNEW); // "新しいパスワードを入力してください"
	if(dlg.DoModal() == TRUE)
	{
		CString pass;
		pass = dlg.m_passwd;
		dlg.m_passwd = "";
		dlg.m_message.LoadString(STR_PAS_IMPCONFIRM); // "確認のためもう一度パスワードを入力してください";
		if(dlg.DoModal() == TRUE)
		{
			if(dlg.m_passwd == pass)
			{
				strTmp1.LoadString(STR_PAS_CHANGEOK);
				strTmp2.LoadString(STR_APPNAME);
				::MessageBox(NULL, strTmp1, strTmp2 ,MB_OK|MB_ICONINFORMATION|MB_APPLMODAL);
				theApp->m_tasks.g_passwd = pass;
				return;
			}
		}
	}
	strTmp1.LoadString(STR_PAS_CHANGENG);
	strTmp2.LoadString(STR_APPNAME);
	::MessageBox(NULL, strTmp1, strTmp2 ,MB_OK|MB_ICONEXCLAMATION|MB_APPLMODAL);
}

// **********************************
// パスワードによる保護
// **********************************
void CPropCnf2::OnGSecure() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString strTmp1, strTmp2;
	if(IsDlgButtonChecked(IDC_G_SECURE))
	{
		CSTaskApp *theApp;
		theApp = (CSTaskApp *)AfxGetApp();
		CDlgPasswd dlg;
		dlg.m_message.LoadString(STR_PAS_IMP); // "現在のパスワードを入力してください";
		if((dlg.DoModal() == TRUE) && (dlg.m_passwd == theApp->m_tasks.g_passwd)) ;
		else
		{	// パスワードの変更を受け付けない場合
			strTmp1.LoadString(STR_PAS_ERR_OFF);
			strTmp2.LoadString(STR_APPNAME);
			::MessageBox(NULL, strTmp1, strTmp2 ,MB_OK|MB_ICONSTOP|MB_APPLMODAL);
			CheckDlgButton(IDC_G_SECURE, 0);
			return;
		}
	}
}

// **********************************
//  スタートメニューにアンインストール・アイコン登録
// **********************************
void CPropCnf2::OnBtnMkunin() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	::MkUninstMnu();
}
/////////////////////////////////////////////////////////////////////////////
// CPropAdd5 プロパティ ページ

IMPLEMENT_DYNCREATE(CPropAdd5, CPropertyPage)

CPropAdd5::CPropAdd5() : CPropertyPage(CPropAdd5::IDD)
{
	//{{AFX_DATA_INIT(CPropAdd5)
		// メモ - ClassWizard はこの位置にメンバの初期化処理を追加します。
	//}}AFX_DATA_INIT
}

CPropAdd5::~CPropAdd5()
{
}

void CPropAdd5::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropAdd5)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropAdd5, CPropertyPage)
	//{{AFX_MSG_MAP(CPropAdd5)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropAdd5 メッセージ ハンドラ

BOOL CPropAdd5::OnSetActive() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	// ウィザード形式のボタン「次」「完了」などの制御
	CPropertySheet* pSheet = (CPropertySheet*)GetParent();
	ASSERT_KINDOF(CPropertySheet, pSheet);
	pSheet->SetWizardButtons( PSWIZB_BACK | PSWIZB_NEXT | PSWIZB_FINISH);
	
	return CPropertyPage::OnSetActive();
}


void CPropAdd5::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください

	// タイトル名の設定
	SetDlgItemText(IDC_PROP5_NAME,*trn_name);
	SetDlgItemText(IDC_EDIT_FPASS,*trn_fpass);
	
}

// **********************************
// （グローバル）カレント・ディレクトリの選択
// **********************************
void CPropCnf1::OnBtnCurdir() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char strPathname[MAX_PATH];
	LPITEMIDLIST pidl;
	BROWSEINFO bi;

	// テキストボックスに入力されているパスを得る
	GetDlgItemText(IDC_EDIT_CURDIR, strPathname, MAX_PATH-1);

	// Win95 対策 （最後に \ が付いたフォルダ名は受け付けない）
	if(strlen(strPathname)>3)		// ルートフォルダと空白は除く
	{
		if(strPathname[strlen(strPathname)-1] == '\\')
			strPathname[strlen(strPathname)-1] = (char)NULL;
	}

	// BROWSEINFO 構造体の初期化
	ZeroMemory(&bi,sizeof(bi));
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpszTitle = (LPCSTR)"Selection of current folder";
	bi.hwndOwner = m_hWnd;
	bi.lpfn = &myBrowseCallbackProc;	// 初期フォルダ設定のためコールバック関数設定
	bi.lParam = (LPARAM)strPathname;	// 初期フォルダをパラメータとして設定

	pidl = ::SHBrowseForFolder(&bi);
	if(pidl == NULL) return;
	SHGetPathFromIDList(pidl, strPathname);

	////////////////////////
	// pidl を明示的に解放する
	////////////////////////
	IMalloc *imalloc;
	// Shellのメモリ管理オブジェクトを取得
	if( ::SHGetMalloc( &imalloc) != NOERROR) imalloc = NULL;
	// オブジェクト取得に成功した場合に開放する
	if( imalloc != NULL) imalloc->Free((void *)pidl); // ITEMIDLISTを開放

	// ルートフォルダの扱い。最後に \ がついているかどうか
	if(strPathname[strlen(strPathname)-1] != '\\')
		strcat(strPathname, "\\");
	// ファイル入力ボックスをアップデートする
	SetDlgItemText(IDC_EDIT_CURDIR, strPathname);
	
}

// **********************************
// フォルダ選択 SDK ダイアログ ::SHBrowseForFolder 用の コールバック関数
// **********************************
int CALLBACK myBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	// ダイアログが初期化されるとき （BFFM_INITIALIZED）
    if(uMsg == BFFM_INITIALIZED){
		// フォルダを選択する （BFFM_SETSELECTION）メッセージ
		if(lpData != NULL)
	        SendMessage(hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);
    }
    return 0;
}

// **********************************
// フォルダ選択 SDK ダイアログ ::SHBrowseForFolder 用
// ルートフォルダの指定のためのフォルダ名コンバート
// **********************************
LPITEMIDLIST ConvertPIDL(char *lpszPath)
{
	ULONG			nDummy;
	ULONG			dwAttributes;
	OLECHAR			olePath[MAX_PATH];
	LPSHELLFOLDER	pDesktopFolder;
	LPITEMIDLIST	pidl;

	if(SUCCEEDED(::SHGetDesktopFolder(&pDesktopFolder))) {
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpszPath, -1, olePath, MAX_PATH);
		if(FAILED(pDesktopFolder->ParseDisplayName(NULL, NULL, olePath, &nDummy, &pidl, &dwAttributes)))
		{
			pidl = NULL;
		}
	}
	return(pidl);
}

// **********************************
// （グローバル）カレントディレクトリのコンボボックスにより項目無効化
// **********************************
void CPropCnf1::OnSelchangeCmbCurdir() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int i=m_cmb_curdir_ctrl.GetCurSel();
	switch(i)
	{
	case 0	:	// 何もしない
		m_edit_curdir_ctrl.EnableWindow(FALSE);
		m_btn_curdir_ctrl.EnableWindow(FALSE);
		break;
	case 1	:	// sTaskフォルダ
		m_edit_curdir_ctrl.EnableWindow(FALSE);
		m_btn_curdir_ctrl.EnableWindow(FALSE);
		break;
	case 2	:	// ユーザ設定
		m_edit_curdir_ctrl.EnableWindow(TRUE);
		m_btn_curdir_ctrl.EnableWindow(TRUE);
		break;
	}
	
}

BOOL CPropCnf1::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	switch(m_cmb_curdir)
	{
	case 0	:	// 何もしない
		m_edit_curdir_ctrl.EnableWindow(FALSE);
		m_btn_curdir_ctrl.EnableWindow(FALSE);
		break;
	case 1	:	// sTaskフォルダ
		m_edit_curdir_ctrl.EnableWindow(FALSE);
		m_btn_curdir_ctrl.EnableWindow(FALSE);
		break;
	case 2	:	// ユーザ設定
		m_edit_curdir_ctrl.EnableWindow(TRUE);
		m_btn_curdir_ctrl.EnableWindow(TRUE);
		break;
	}
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CPropAdd1::OnSelchangeCmbExecdir() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int i=m_cmb_execdir_ctrl.GetCurSel();
	switch(i)
	{
	case 0	:	// この機能を使わない
		m_execdir_ctrl.EnableWindow(FALSE);
		m_btn_execdir_ctrl.EnableWindow(FALSE);
		break;
	case 1	:	// 実行するプログラムのフォルダ
		m_execdir_ctrl.EnableWindow(FALSE);
		m_btn_execdir_ctrl.EnableWindow(FALSE);
		break;
	case 2	:	// テンポラリフォルダ
		m_execdir_ctrl.EnableWindow(FALSE);
		m_btn_execdir_ctrl.EnableWindow(FALSE);
		break;
	case 3	:	// ユーザ指定
		m_execdir_ctrl.EnableWindow(TRUE);
		m_btn_execdir_ctrl.EnableWindow(TRUE);
		break;
	}
	
}

BOOL CPropAdd1::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	switch(m_execdir_mode)
	{
	case 0	:	// この機能を使わない
		m_execdir_ctrl.EnableWindow(FALSE);
		m_btn_execdir_ctrl.EnableWindow(FALSE);
		break;
	case 1	:	// 実行するプログラムのフォルダ
		m_execdir_ctrl.EnableWindow(FALSE);
		m_btn_execdir_ctrl.EnableWindow(FALSE);
		break;
	case 2	:	// テンポラリフォルダ
		m_execdir_ctrl.EnableWindow(FALSE);
		m_btn_execdir_ctrl.EnableWindow(FALSE);
		break;
	case 3	:	// ユーザ指定
		m_execdir_ctrl.EnableWindow(TRUE);
		m_btn_execdir_ctrl.EnableWindow(TRUE);
		break;
	}
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CPropAdd1::OnBtnExecdir() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char strPathname[MAX_PATH];
	LPITEMIDLIST pidl;
	BROWSEINFO bi;

	// テキストボックスに入力されているパスを得る
	GetDlgItemText(IDC_EDIT_EXECDIR, strPathname, MAX_PATH-1);

	// Win95 対策 （最後に \ が付いたフォルダ名は受け付けない）
	if(strlen(strPathname)>3)		// ルートフォルダと空白は除く
	{
		if(strPathname[strlen(strPathname)-1] == '\\')
			strPathname[strlen(strPathname)-1] = (char)NULL;
	}

	// BROWSEINFO 構造体の初期化
	ZeroMemory(&bi,sizeof(bi));
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpszTitle = (LPCSTR)"Selection of working folder";
	bi.hwndOwner = m_hWnd;
	bi.lpfn = &myBrowseCallbackProc;	// 初期フォルダ設定のためコールバック関数設定
	bi.lParam = (LPARAM)strPathname;	// 初期フォルダをパラメータとして設定

	pidl = ::SHBrowseForFolder(&bi);
	if(pidl == NULL) return;
	SHGetPathFromIDList(pidl, strPathname);

	////////////////////////
	// pidl を明示的に解放する
	////////////////////////
	IMalloc *imalloc;
	// Shellのメモリ管理オブジェクトを取得
	if( ::SHGetMalloc( &imalloc) != NOERROR) imalloc = NULL;
	// オブジェクト取得に成功した場合に開放する
	if( imalloc != NULL) imalloc->Free((void *)pidl); // ITEMIDLISTを開放

	// ルートフォルダの扱い。最後に \ がついているかどうか
	if(strPathname[strlen(strPathname)-1] != '\\')
		strcat(strPathname, "\\");
	// ファイル入力ボックスをアップデートする
	SetDlgItemText(IDC_EDIT_EXECDIR, strPathname);
	
	
}

void CPropAdd1::OnBtnPowertest() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	SYSTEM_POWER_STATUS SystemPowerStatus;
	CString strTmp, strBatteryFlag;
	GetSystemPowerStatus(&SystemPowerStatus);
	if(SystemPowerStatus.BatteryFlag == 255) strBatteryFlag = "Unknown";
	else{
		if(SystemPowerStatus.BatteryFlag == 128) strBatteryFlag = "No Battery";
		else{
			strBatteryFlag = "Battery ";
			if(SystemPowerStatus.BatteryFlag & 1) strBatteryFlag += " High";
			if(SystemPowerStatus.BatteryFlag & 2) strBatteryFlag += " Low";
			if(SystemPowerStatus.BatteryFlag & 4) strBatteryFlag += " Critical";
			if(SystemPowerStatus.BatteryFlag & 8) strBatteryFlag += " , Charging now";
		}
	}
	strTmp.Format("Power Status Information\nWindows Base Services : GetSystemPowerStatus\n"
				"\n"
				"AC Line Status : %s\n"
				"Battery Flag : %s\n"
				"Battery Life Percent : %d %%\n"
				"Battery Life Time : %d seconds\n"
				"Battery Full Life Time : %d seconds",
				SystemPowerStatus.ACLineStatus == 0 ? "AC Offline" : (SystemPowerStatus.ACLineStatus == 1 ? "AC Online" : "Unknown"),
				strBatteryFlag,
				SystemPowerStatus.BatteryLifePercent == 255 ? -1 : SystemPowerStatus.BatteryLifePercent,
				SystemPowerStatus.BatteryLifeTime,
				SystemPowerStatus.BatteryFullLifeTime);

	MessageBox(strTmp);

}

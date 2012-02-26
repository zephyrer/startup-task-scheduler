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
	//}}AFX_DATA_INIT
}

CPropAdd1::~CPropAdd1()
{
}

void CPropAdd1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropAdd1)
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDV_MinMaxChars(pDX, m_name, 1, 50);
	DDX_Text(pDX, IDC_EDIT_FPASS, m_fpass);
	DDV_MinMaxChars(pDX, m_fpass, 1, 250);
	DDX_Text(pDX, IDC_EDIT_PARAM, m_param);
	DDV_MaxChars(pDX, m_param, 100);
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_mes);
	DDV_MaxChars(pDX, m_mes, 100);
	DDX_Check(pDX, IDC_CHECK_ON, m_exec);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropAdd1, CPropertyPage)
	//{{AFX_MSG_MAP(CPropAdd1)
	ON_BN_CLICKED(IDC_BTN_BROWSE, OnBtnBrowse)
	ON_WM_SHOWWINDOW()
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
	pSheet->SetWizardButtons( PSWIZB_BACK | PSWIZB_NEXT | PSWIZB_FINISH);
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
	//}}AFX_DATA_INIT
}

CPropCnf1::~CPropCnf1()
{
}

void CPropCnf1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropCnf1)
	DDX_Check(pDX, IDC_DLG, m_dialog);
	DDX_Check(pDX, IDC_DLG0, m_dialog0);
	DDX_Check(pDX, IDC_G_FAILDLG, m_faildlg);
	DDX_Text(pDX, IDC_G_DELAY, m_delay);
	DDV_MinMaxUInt(pDX, m_delay, 0, 120);
	DDX_Radio(pDX, IDC_G_TIME1, m_time);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropCnf1, CPropertyPage)
	//{{AFX_MSG_MAP(CPropCnf1)
		// メモ: ClassWizard はこの位置に DDX および DDV の呼び出しコードを追加します。
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

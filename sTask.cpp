// **********************************
//  sTask メイン関数 (sTaskApp)、および sTaskAbout の定義
//
// (2002/05/11) 別スレッドでプログレスダイアログ表示化
// **********************************

#include "stdafx.h"
#include "sTask.h"

#include "MainFrm.h"
#include "sTaskDoc.h"
#include "sTaskView.h"
#include "sTaskDlg.h"
#include "InstFunc.h"
#include "CheckWinVer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTaskApp

BEGIN_MESSAGE_MAP(CSTaskApp, CWinApp)
	//{{AFX_MSG_MAP(CSTaskApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// 標準のファイル基本ドキュメント コマンド
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTaskApp クラスの構築

CSTaskApp::CSTaskApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CSTaskApp オブジェクト

CSTaskApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSTaskApp クラスの初期化

BOOL CSTaskApp::InitInstance()
{
	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さく
	// したければ以下の特定の初期化ルーチンの中から不必要なもの
	// を削除してください。

	CString tmpstr,tmpstr2;

#ifdef _AFXDLL
	Enable3dControls();		// 共有 DLL の中で MFC を使用する場合にはここを呼び出してください。
#else
	Enable3dControlsStatic();	// MFC と静的にリンクしている場合にはここを呼び出してください。
#endif

	// 設定が保存される下のレジストリ キーを変更します。
	// TODO: この文字列を、会社名または所属など適切なものに
	// 変更してください。
	// レジストリの指定
	// リソースでAFX_IDS_APP_TITLEにアプリケーション名を設定すること 
	// m_pszAppName = "compdel" としてはいけない！ ASSERTION エラーとなる
	SetRegistryKey((LPCTSTR)"hi soft");

	LoadStdProfileSettings(0);	// MRUリスト管理(0:しない) *** ver 1.2 Win95 エラーの回避

	// **********************************
	//  レジストリから全設定値を読み込む
	// **********************************
	RegReadAll();

	// **********************************
	//  コマンドライン引数による分岐
	// **********************************
	if(!strcmpi(m_lpCmdLine, ""))
	{	// オプションなし
		// **********************************
		//  インストーラの起動。
		//  すでに、インストール済みと時は、単に TRUE を返すだけ
		// **********************************
		if(!InstallSeq())
			return FALSE;	// インストーラをユーザがキャンセルしたとき （プログラム終了）

	}
	else if(!strcmpi(m_lpCmdLine, "/start"))
	{	// 自動実行
		if(m_tasks.g_delay > 0) ::Sleep(m_tasks.g_delay*1000);
		m_tasks.ExecAllTask(NULL);
		RegWriteAll();
		return FALSE;		// （ウインドウを表示せずに、プログラム終了）
	}
	else if(!strcmpi(m_lpCmdLine, "/delete"))
	{	// アンインストール
		UninstallSeq();
		return FALSE;		// （ウインドウを表示せずに、プログラム終了）
	}
	else
	{
		// その他の引数のときは、エラーとする。
		// （引数付きで、ドキュメント・ビューのクラスを起動できないため）
		::MessageBox(NULL, "Parameter Error\r\n\r\n(\"/start\" and \"/delete\" is acceptable)", "sTask (Information)", MB_OK|MB_ICONINFORMATION|MB_APPLMODAL);
		return FALSE;		// （ウインドウを表示せずに、プログラム終了）
	}


	// **********************************
	//  パスワードによる保護がある場合
	// **********************************
	if(m_tasks.g_secure)
	{
		CDlgPasswd dlg;
		dlg.m_message.LoadString(STR_PAS_IMP);
		dlg.DoModal();
		if(dlg.m_passwd != m_tasks.g_passwd)
		{
			tmpstr.LoadString(STR_ERRPAS);
			::MessageBox(NULL, (LPCSTR)tmpstr, "sTask (Error)", MB_OK|MB_ICONSTOP|MB_APPLMODAL);
			return FALSE;		// パスワードが一致しなかったため終了
		}
	}

	// **********************************
	//  設定ダイアログの表示
	// **********************************

	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。

//		AfxInitRichEdit();	// 1999.11.24 コメントアウト　不要？
//		AfxEnableControlContainer() ; // 99.11.24 コメントアウト　不要？
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSTaskDoc),
		RUNTIME_CLASS(CMainFrame),       // メイン SDI フレーム ウィンドウ
		RUNTIME_CLASS(CSTaskView));
	AddDocTemplate(pDocTemplate);

	// このへんの MFC 標準のコマンドライン解析は消さないこと。内部でドキュメントの初期化をしている
	// DDE、file open など標準のシェル コマンドのコマンドラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// コマンドラインでディスパッチ コマンドを指定します。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;	// ウインドウを表示する (TRUEの場合)

	// プログラム終了時のレジストリへの保存は
	// CSTaskView::OnDestroy() 関数の中で行っている
}

/////////////////////////////////////////////////////////////////////////////
// CSTaskApp メッセージ ハンドラ

// **********************************
// レジストリに書き込む（全情報）
// **********************************
BOOL CSTaskApp::RegWriteAll()
{
	CString str_n;

	if(m_tasks.g_debug) return FALSE;	// デバッグモードのときは保存しない

	if(m_tasks.g_delay != GetProfileInt("Settings","delay",3))
		WriteProfileInt("Settings","delay",m_tasks.g_delay);
	if(m_tasks.g_time != (BOOL)GetProfileInt("Settings","time",1))
		WriteProfileInt("Settings","time",m_tasks.g_time);
	if(m_tasks.g_dialog != (BOOL)GetProfileInt("Settings","dlg",1))
		WriteProfileInt("Settings","dlg",m_tasks.g_dialog);
	if(m_tasks.g_dialog0 != (BOOL)GetProfileInt("Settings","dlg0",0))
		WriteProfileInt("Settings","dlg0",m_tasks.g_dialog0);
	if(m_tasks.g_faildlg != (BOOL)GetProfileInt("Settings","dlgfl",1))
		WriteProfileInt("Settings","dlgfl",m_tasks.g_faildlg);
	if(m_tasks.g_secure != (BOOL)GetProfileInt("Settings","secure",0))
		WriteProfileInt("Settings","secure",m_tasks.g_secure);
	if(m_tasks.g_passwd != GetProfileString("Settings","pass",""))
		WriteProfileString("Settings","pass",m_tasks.g_passwd);
	if(m_tasks.maxtask != GetProfileInt("Settings","maxtask",0))
		WriteProfileInt("Settings","maxtask",m_tasks.maxtask);

	for(UINT i=0; i<m_tasks.maxtask; i++)
	{
		str_n.Format("task%d",i);
		if(m_tasks.tasks[i].time_lastexec != CTime(GetProfileInt(str_n,"tlexec",0)))
			WriteProfileInt(str_n,"tlexec",m_tasks.tasks[i].time_lastexec.GetTime());
		if(m_tasks.tasks[i].name != GetProfileString(str_n,"name"))
			WriteProfileString(str_n,"name",m_tasks.tasks[i].name);
		if(m_tasks.tasks[i].fpass != GetProfileString(str_n,"fpass"))
			WriteProfileString(str_n,"fpass",m_tasks.tasks[i].fpass);
		if(m_tasks.tasks[i].param != GetProfileString(str_n,"param"))
			WriteProfileString(str_n,"param",m_tasks.tasks[i].param);
		if(m_tasks.tasks[i].mes != GetProfileString(str_n,"mes"))
			WriteProfileString(str_n,"mes",m_tasks.tasks[i].mes);
		if(m_tasks.tasks[i].exec != (BOOL)GetProfileInt(str_n,"exec",0))
			WriteProfileInt(str_n,"exec",m_tasks.tasks[i].exec);
		if(m_tasks.tasks[i].apart != GetProfileInt(str_n,"apart",0))
			WriteProfileInt(str_n,"apart",m_tasks.tasks[i].apart);
		if(m_tasks.tasks[i].interval != GetProfileInt(str_n,"intvl",1))
			WriteProfileInt(str_n,"intvl",m_tasks.tasks[i].interval);
		if(m_tasks.tasks[i].date != GetProfileInt(str_n,"date",0))
			WriteProfileInt(str_n,"date",m_tasks.tasks[i].date);
		if(m_tasks.tasks[i].excludetime != (BOOL)GetProfileInt(str_n,"extime",0))
			WriteProfileInt(str_n,"extime",m_tasks.tasks[i].excludetime);
		if(m_tasks.tasks[i].time0 != CTime(GetProfileInt(str_n,"t0",0)))
			WriteProfileInt(str_n,"t0",m_tasks.tasks[i].time0.GetTime());
		if(m_tasks.tasks[i].time1 != CTime(GetProfileInt(str_n,"t1",0)))
			WriteProfileInt(str_n,"t1",m_tasks.tasks[i].time1.GetTime());
		if(m_tasks.tasks[i].waitexit != (BOOL)GetProfileInt(str_n,"wexit",0))
			WriteProfileInt(str_n,"wexit",m_tasks.tasks[i].waitexit);
		if(m_tasks.tasks[i].waitsec != GetProfileInt(str_n,"wsec",0))
			WriteProfileInt(str_n,"wsec",m_tasks.tasks[i].waitsec);
		if(m_tasks.tasks[i].taskoff != (BOOL)GetProfileInt(str_n,"toff",0))
			WriteProfileInt(str_n,"toff",m_tasks.tasks[i].taskoff);
		if(m_tasks.tasks[i].taskoffcount != GetProfileInt(str_n,"toffcnt",0))
			WriteProfileInt(str_n,"toffcnt",m_tasks.tasks[i].taskoffcount);
		if(m_tasks.tasks[i].dialog != (BOOL)GetProfileInt(str_n,"dlg",1))
			WriteProfileInt(str_n,"dlg",m_tasks.tasks[i].dialog);
		if(m_tasks.tasks[i].syncprev != (BOOL)GetProfileInt(str_n,"sync",0))
			WriteProfileInt(str_n,"sync",m_tasks.tasks[i].syncprev);
		if(m_tasks.tasks[i].cnt_check != GetProfileInt(str_n,"c_c",0))
			WriteProfileInt(str_n,"c_c",m_tasks.tasks[i].cnt_check);
		if(m_tasks.tasks[i].cnt_exec != GetProfileInt(str_n,"c_e",0))
			WriteProfileInt(str_n,"c_e",m_tasks.tasks[i].cnt_exec);
		
	}
	return TRUE;
}

// **********************************
// レジストリから読み込む（全情報）
// **********************************
BOOL CSTaskApp::RegReadAll()
{
	CString str_n;

	m_tasks.g_delay = GetProfileInt("Settings","delay",3);
	m_tasks.g_time = GetProfileInt("Settings","time",1);
	m_tasks.g_dialog = GetProfileInt("Settings","dlg",1);
	m_tasks.g_dialog0 = GetProfileInt("Settings","dlg0",0);
	m_tasks.g_faildlg = GetProfileInt("Settings","dlgfl",1);
	m_tasks.g_secure = GetProfileInt("Settings","secure",0);
	m_tasks.g_passwd = GetProfileString("Settings","pass","");
	m_tasks.maxtask = GetProfileInt("Settings","maxtask",0);

	for(UINT i=0; i<m_tasks.maxtask; i++)
	{
		str_n.Format("task%d",i);
		m_tasks.tasks[i].time_lastexec = CTime(GetProfileInt(str_n,"tlexec",0));
		m_tasks.tasks[i].name = GetProfileString(str_n,"name");
		m_tasks.tasks[i].fpass = GetProfileString(str_n,"fpass");
		m_tasks.tasks[i].param = GetProfileString(str_n,"param");
		m_tasks.tasks[i].mes = GetProfileString(str_n,"mes");
		m_tasks.tasks[i].exec = GetProfileInt(str_n,"exec",0);
		m_tasks.tasks[i].apart = GetProfileInt(str_n,"apart",0);
		m_tasks.tasks[i].interval = GetProfileInt(str_n,"intvl",1);
		m_tasks.tasks[i].date = GetProfileInt(str_n,"date",0);
		m_tasks.tasks[i].excludetime = GetProfileInt(str_n,"extime",0);
		m_tasks.tasks[i].time0 = CTime(GetProfileInt(str_n,"t0",0));
		m_tasks.tasks[i].time1 = CTime(GetProfileInt(str_n,"t1",0));
		m_tasks.tasks[i].waitexit = GetProfileInt(str_n,"wexit",0);
		m_tasks.tasks[i].waitsec = GetProfileInt(str_n,"wsec",0);
		m_tasks.tasks[i].taskoff = GetProfileInt(str_n,"toff",0);
		m_tasks.tasks[i].taskoffcount = GetProfileInt(str_n,"toffcnt",0);
		m_tasks.tasks[i].dialog = GetProfileInt(str_n,"dlg",1);
		m_tasks.tasks[i].syncprev = GetProfileInt(str_n,"sync",0);
		m_tasks.tasks[i].cnt_check = GetProfileInt(str_n,"c_c",0);
		m_tasks.tasks[i].cnt_exec = GetProfileInt(str_n,"c_e",0);
		// オーバーフロー対策
		if(m_tasks.tasks[i].cnt_check > 1000) m_tasks.tasks[i].cnt_check=0;
		if(m_tasks.tasks[i].cnt_exec > 1000) m_tasks.tasks[i].cnt_exec = 0;
	}
	return TRUE;

}


// **********************************
//  インストール作業 一式
//
//  戻り値  TRUE:インストール完了または不要,  FALSE:ユーザキャンセル
// **********************************
BOOL CSTaskApp::InstallSeq()
{
	CDlgInstall dlg;
	dlg.m_r_autoexec = 1;	// スタートメニューに登録
	dlg.m_chk_uninst_start = TRUE;	// スタートメニューにアンインストールアイコン
	dlg.m_IsAdminInst = TRUE;


	// **********************************
	//  インストールフラグのチェック (HKLM/HKCU)
	// **********************************
	if(ChkHkcuInstReg() || ChkHklmInstReg()) return TRUE;

// ***********************
// sTask 固有  前バージョンまでの設定値を消去
// ***********************
	::RmUserRegEx();

	// **********************************
	//  Windows NT 系列とそれ以外で区別
	// **********************************
	if(::IsWinNT())
	{	// ********************  WinNT のとき ********************
		if(::IsPossibleWriteHklmRun())
		{	// HKLM ... Run 書き込み可のとき (Admin モード)
			if(dlg.DoModal() != IDOK) return FALSE;	// ユーザがインストールをキャンセル
			if(dlg.m_r_autoexec == 3) ::MkHklmInstReg();	// Admin インストール
			else ::MkHkcuInstReg();	// Current User インストール
		}
		else
		{	// HKLM ... Run 書き込み不可のとき (ユーザ モード)
			dlg.m_IsAdminInst = FALSE;	
			if(dlg.DoModal() != IDOK) return FALSE;	// ユーザがインストールをキャンセル
			::MkHkcuInstReg();
		}
	}
	else
	{	// ********************  WinNT以外 のとき ********************
		if(dlg.DoModal() != IDOK) return FALSE;	// ユーザがインストールをキャンセル
		if(dlg.m_r_autoexec == 3) ::MkHklmInstReg();	// Admin インストール
		else ::MkHkcuInstReg();	// Current User インストール
	}

	// **********************************
	//  インストールダイアログでの選択項目により、インストール作業
	// **********************************
	//  自動実行オプション
	//  選択されたオプション以外の「自動実行」を削除することも同時に行う
	// **********************************
	switch(dlg.m_r_autoexec)
	{
	case 0 :	// 自動実行しない
		::RmHklmLnk();
		::RmHkcuLnk();
		::RmStartMnu();
		break;
	case 1 :	// スタートメニュー
		::RmHklmLnk();
		::RmHkcuLnk();
		::MkStartMnu();
		break;
	case 2 :	// HKCU に自動実行
		::RmHklmLnk();
		::MkHkcuLnk();
		::RmStartMnu();
		break;
	case 3 :	// HKLM に自動実行
		::MkHklmLnk();
		::RmHkcuLnk();
		::RmStartMnu();
		break;
	}

	// **********************************
	//  スタートメニューに設定アイコン登録
	// **********************************
	::MkProgramsMnu();

	// **********************************
	//  コンパネからアンインストールする設定
	// **********************************
	if(dlg.m_chk_uninst_cpl)
	{
		if(::IsWinNT()) ::MkHkcuUninstMnu();
		else ::MkHklmUninstMnu();
	}

	// **********************************
	//  スタートメニューからアンインストールする設定
	// **********************************
	if(dlg.m_chk_uninst_start)
		::MkUninstMnu();

// sTask 固有 !
	// **********************************
	// パスワードの作成
	// **********************************
	int i;
	char strTmp3[2];	// パスワード作成用
	CString tmpstr, tmpstr2;
	tmpstr = "";
	for(i=0; i<8; i++)
	{
		strTmp3[0] = GenerateRandom(0x41, 0x5a);
		strTmp3[1] = (char)NULL;
		tmpstr += strTmp3;
	}
	m_tasks.g_passwd = tmpstr;
	tmpstr.LoadString(STR_MES_NEW_PASSWORD);
	tmpstr2.Format(tmpstr, m_tasks.g_passwd);
	::MessageBox(NULL, (LPCSTR)tmpstr2, "sTask (information) NEW PASSWORD", MB_OK|MB_ICONINFORMATION|MB_APPLMODAL);

	return TRUE;
}

// **********************************
//  アンインストール作業 一式
//
//  戻り値  TRUE:完了または不要,  FALSE:失敗またはユーザキャンセル
// **********************************
BOOL CSTaskApp::UninstallSeq()
{
	CString sTmpMsg;

	// **********************************
	//  Windows NT 系列とそれ以外で区別
	// **********************************
	if(::IsWinNT())
	{	// ********************  WinNT のとき ********************
		if(::IsPossibleWriteHklmRun())
		{	// HKLM ... Run 書き込み可のとき (Admin モードかも？)
			if(::ChkHklmInstReg())
			{	// Admin モードでインストールされていたとき
				sTmpMsg.LoadString(STR_MES_UNIN_ADM);
				if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "sTask Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
					return FALSE;	// ユーザがキャンセル
				::RmHklmInstReg();
			}
			else
			{	// カレント・ユーザ・モードのとき
				sTmpMsg.LoadString(STR_MES_UNIN_CUR);
				if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "sTask Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
					return FALSE;	// ユーザがキャンセル
				::RmHkcuInstReg();
			}
		}
		else
		{	// HKLM ... Run 書き込み不可のとき (ユーザ モード)
			if(::ChkHklmInstReg())
			{	// Admin モードでインストールされていたとき
				sTmpMsg.LoadString(STR_MES_UNIN_NOT);
				if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "sTask Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
					return FALSE;	// ユーザがキャンセル
				::RmUserRegEx();	// ユーザデータのみ消去する
				::RmHkcuInstReg();	// どちらにしても、これは消しておく
				//::RmHklmLnk();	// HKLM のインストールフラグは消去しない
				::RmHkcuLnk();
				::RmStartMnu();
				::RmHkcuUninstMnu();
				::RmUninstMnu();
				::RmProgramsMnu();
				return FALSE;
			}
			else
			{	// カレント・ユーザ・モードのとき
				sTmpMsg.LoadString(STR_MES_UNIN_CUR);
				if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "sTask Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
					return FALSE;	// ユーザがキャンセル
				::RmHkcuInstReg();
			}
		}
	}
	else
	{	// ********************  WinNT以外 のとき ********************
		if(::ChkHklmInstReg())
		{	// Admin モードでインストールされていたとき
			sTmpMsg.LoadString(STR_MES_UNIN_ADM);
			if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "sTask Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
				return FALSE;	// ユーザがキャンセル
			::RmHklmInstReg();
		}
		else
		{	// カレント・ユーザ・モードのとき
			sTmpMsg.LoadString(STR_MES_UNIN_CUR);
			if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "sTask Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
				return FALSE;	// ユーザがキャンセル
			::RmHkcuInstReg();
		}
	}

	::RmHkcuInstReg();		// どちらにしても、これは消しておく
	::RmHklmLnk();
	::RmHkcuLnk();
	::RmStartMnu();
// ***********************
// sTask 固有
// ***********************
	::RmUserRegEx();
//	::RmUserReg();

	::RmHkcuUninstMnu();
	::RmHklmUninstMnu();
	::RmUninstMnu();

	::RmProgramsMnu();

	return TRUE;

}

unsigned int CSTaskApp::GenerateRandom(unsigned int min, unsigned int max)
{
	unsigned int i=0;
	do
	{
		i = ((rand() & 0xff) * (max-min+1))/0xff + min;
	}while(i<min || i>max);

	return i;
}


// **********************************
// sTaskAbout の定義
// **********************************
/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard 仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAboutDlg)
		// メッセージ ハンドラはありません。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// メッセージ ハンドラはありません。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ダイアログを実行するためのアプリケーション コマンド
void CSTaskApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

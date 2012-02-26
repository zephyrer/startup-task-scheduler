// sTaskView.cpp : CSTaskView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "sTask.h"

#include "sTaskDoc.h"
#include "sTaskView.h"
#include "sTaskProp.h"
#include "sTaskDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTaskView

IMPLEMENT_DYNCREATE(CSTaskView, CListView)

BEGIN_MESSAGE_MAP(CSTaskView, CListView)
	//{{AFX_MSG_MAP(CSTaskView)
	ON_COMMAND(ID_VIEW_ICON, OnViewIcon)
	ON_COMMAND(ID_VIEW_REPORT, OnViewReport)
	ON_COMMAND(ID_TASK_NEW, OnTaskNew)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ICON, OnUpdateViewIcon)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REPORT, OnUpdateViewReport)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_COMMAND(ID_TASK_CUT, OnTaskCut)
	ON_COMMAND(ID_TASK_UP, OnTaskUp)
	ON_COMMAND(ID_TASK_DOWN, OnTaskDown)
	ON_COMMAND(ID_TASK_OFF, OnTaskOff)
	ON_COMMAND(ID_TASK_CONFIG, OnTaskConfig)
	ON_COMMAND(ID_WRITEREG, OnWriteReg)
	ON_COMMAND(ID_TASK_EXEC, OnTaskExec)
	ON_COMMAND(ID_CONFIG, OnConfig)
	ON_COMMAND(ID_DEBUG, OnDebug)
	ON_UPDATE_COMMAND_UI(ID_DEBUG, OnUpdateDebug)
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_TASK_CUT, OnUpdateTaskCut)
	ON_UPDATE_COMMAND_UI(ID_TASK_EXEC, OnUpdateTaskExec)
	ON_UPDATE_COMMAND_UI(ID_TASK_OFF, OnUpdateTaskOff)
	ON_UPDATE_COMMAND_UI(ID_TASK_CONFIG, OnUpdateTaskConfig)
	ON_UPDATE_COMMAND_UI(ID_TASK_UP, OnUpdateTaskUp)
	ON_UPDATE_COMMAND_UI(ID_TASK_DOWN, OnUpdateTaskDown)
	ON_UPDATE_COMMAND_UI(ID_TASK_NEW, OnUpdateTaskNew)
	ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTaskView クラスの構築/消滅

CSTaskView::CSTaskView()
{
	// TODO: この場所に構築用のコードを追加してください。
	theApp = (CSTaskApp *)AfxGetApp();
}

CSTaskView::~CSTaskView()
{
}

BOOL CSTaskView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。
	cs.style |= LVS_REPORT;

	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSTaskView クラスの描画


/////////////////////////////////////////////////////////////////////////////
// CSTaskView クラスの診断

#ifdef _DEBUG
void CSTaskView::AssertValid() const
{
	CListView::AssertValid();
}

void CSTaskView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CSTaskDoc* CSTaskView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSTaskDoc)));
	return (CSTaskDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSTaskView クラスのメッセージ ハンドラ

void CSTaskView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	CListCtrl& ListCtrl = GetListCtrl();

	// アイコン領域の確保
	m_sIconList.Create(16,16,ILC_COLORDDB,0,21);
	m_lIconList.Create(32,32,ILC_COLORDDB,0,21);
	m_sIconList.SetBkColor(::GetSysColor(COLOR_WINDOW));
	m_lIconList.SetBkColor(::GetSysColor(COLOR_WINDOW));

	DisplayTasks();

	m_ViewStyle = LVS_REPORT;
	ChgViewStyle();	
}

// 終了時には、レジストリをアップデートする
void CSTaskView::OnDestroy() 
{
	CListView::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	CString strTmp, strAppName;
	strAppName.LoadString(STR_APPNAME);	// あらかじめプログラム名をロードしておく（多用するため）

	if(!theApp->m_tasks.g_debug)
	{	// 変更内容をレジストリに保存する
		theApp->RegWriteAll();
	}
	else
	{
		strTmp.LoadString(STR_MES_DEBUG_NOSAVE);	//デバッグモードのため、変更内容は保存されません
		::MessageBox(NULL, strTmp, strAppName, MB_OK|MB_ICONINFORMATION|MB_APPLMODAL);
	}

	// 99.11.27 追加 イメージリストの開放
	BOOL i,j;
	// アイコンの削除（すでに表示されているときのための処理）
	j = m_lIconList.GetImageCount();
	for(i=0; i<j; i++)
		m_lIconList.Remove(0);
	j=m_sIconList.GetImageCount();
	for(i=0; i<j; i++)
		m_sIconList.Remove(0);
//	i = m_sIconList.DeleteImageList();
//	i = m_lIconList.DeleteImageList();
//	m_sIconList.DestroyWindow();
//	m_lIconList.DestroyWindow();

}


BOOL CSTaskView::ChgViewStyle()
{
	return ModifyStyle(LVS_TYPEMASK,m_ViewStyle|LVS_SINGLESEL & LVS_TYPEMASK);
}

void CSTaskView::OnViewIcon() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	m_ViewStyle = LVS_ICON;
	ChgViewStyle();	
}
void CSTaskView::OnUpdateViewIcon(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(m_ViewStyle==LVS_ICON ? 0:1);
	
}

void CSTaskView::OnViewReport() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	m_ViewStyle = LVS_REPORT;
	ChgViewStyle();	
}
void CSTaskView::OnUpdateViewReport(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(m_ViewStyle==LVS_REPORT ? 0:1);	
}

// リストコントロール（メインウインドウ）にタスク一覧を表示する
BOOL CSTaskView::DisplayTasks()
{
	static int insert_col=0;
	int n = theApp->m_tasks.maxtask;	// 0 のときは存在しない
	int i,j;
	CListCtrl& ListCtrl = GetListCtrl();
	CString strTmp;

	// リストの削除（すでに表示されているときのための処理）
	ListCtrl.DeleteAllItems();
	// アイコンの削除（すでに表示されているときのための処理）
	j = m_lIconList.GetImageCount();
	for(i=0; i<j; i++)
		m_lIconList.Remove(0);
	j=m_sIconList.GetImageCount();
	for(i=0; i<j; i++)
		m_sIconList.Remove(0);

	// アイコンの設定
	m_lIconList.Add(theApp->LoadIcon(IDR_XDOCICON));	// タスクの追加のアイコン
	m_sIconList.Add(theApp->LoadIcon(IDR_XDOCICON));
	HICON hicon;
	for(i=0; i<n; i++)
	{
		hicon = ::ExtractIcon(AfxGetApp()->m_hInstance, theApp->m_tasks.tasks[i].fpass ,0);
		if((UINT)hicon == NULL || (UINT)hicon == 1)
		{	// アイコンを持たないファイルのとき
			m_lIconList.Add(theApp->LoadIcon(IDR_SCRIPT));	// ドキュメントのアイコン
			m_sIconList.Add(theApp->LoadIcon(IDR_SCRIPT));
		}
		else
		{
			m_lIconList.Add(hicon);
			m_sIconList.Add(hicon);
		}
		// 99.11.27 GDIエラーの主原因発見　!!! 使い終わったハンドルは、毎回開放すること
		::DestroyIcon(hicon);
	}
	//::DestroyIcon(hicon);
	// リストにアイコンを結びつける
	ListCtrl.SetImageList(&m_sIconList, LVSIL_SMALL);
	ListCtrl.SetImageList(&m_lIconList, LVSIL_NORMAL);

	// 列の挿入
	if(!insert_col)
	{
		strTmp.LoadString(STR_VIEW_TASK);	// タスク名
		ListCtrl.InsertColumn(0,strTmp,LVCFMT_LEFT,180);
		strTmp.LoadString(STR_VIEW_SCHEDULE);	// スケジュール
		ListCtrl.InsertColumn(1,strTmp,LVCFMT_LEFT,300);
		strTmp.LoadString(STR_VIEW_PREVEXEC);	// 前回の実行日
		ListCtrl.InsertColumn(2,strTmp,LVCFMT_LEFT,100);
		strTmp.LoadString(STR_VIEW_NEXTEXEC);	// 次回の実行予定
		ListCtrl.InsertColumn(3,strTmp,LVCFMT_LEFT,100);
		strTmp.LoadString(STR_VIEW_STATUS);	// 状態
		ListCtrl.InsertColumn(4,strTmp,LVCFMT_LEFT,50);
		insert_col = 1;
	}

	// アイテムの挿入
	strTmp.LoadString(STR_VIEW_ADD_NEW_ITEM);	// タスクの追加
	ListCtrl.InsertItem(1, strTmp, 0);
	if(n<1) return TRUE;	// アイテムが一つもないとき
	for(i=0; i<n; i++)
	{
		// 項目自体の追加（１行目は１から始まるが、「タスク追加」を飛ばして２から）
		ListCtrl.InsertItem(i+2,theApp->m_tasks.tasks[i].name,i+1);
		// 項目の付加事項の追加（１行目は０から始まる）
		CString str;
		char dummy[20];

		// １桁目の表示
		if(theApp->m_tasks.tasks[i].apart == 4)
		{
			strTmp.LoadString(STR_PER_RANDOM);	// ランダム
			str = strTmp;
		}
		else
		{
			str = _itoa(theApp->m_tasks.tasks[i].interval, dummy, 10);
			if(theApp->m_tasks.tasks[i].apart == 0)
			{
				strTmp.LoadString(STR_VIEW_AT_LOGON);	// ログオン時
				str = strTmp;
				str += _itoa(theApp->m_tasks.tasks[i].interval, dummy, 10);
				strTmp.LoadString(STR_PER_LOGON);	// 回ごと
				str += strTmp;
			}
			else if(theApp->m_tasks.tasks[i].apart == 1)
			{
				strTmp.LoadString(STR_PER_DAY);	// 日ごと
				str += strTmp;
			}
			else if(theApp->m_tasks.tasks[i].apart == 2)
			{
				strTmp.LoadString(STR_PER_WEEK);	// 週ごと
				str += strTmp;
			}
			else if(theApp->m_tasks.tasks[i].apart == 3)
			{
				strTmp.LoadString(STR_PER_MONTH);	// 月ごと
				str += strTmp;
			}
		}
		if(theApp->m_tasks.tasks[i].date)
		{
			str += " ";
			if(theApp->m_tasks.tasks[i].date & 0x40)
			{
				strTmp.LoadString(STR_SUN);	// 日
				str += strTmp;
			}
			if(theApp->m_tasks.tasks[i].date & 0x20)
			{
				strTmp.LoadString(STR_MON);	// 月
				str += strTmp;
			}
			if(theApp->m_tasks.tasks[i].date & 0x10)
			{
				strTmp.LoadString(STR_TUE);	// 火
				str += strTmp;
			}
			if(theApp->m_tasks.tasks[i].date & 0x08)
			{
				strTmp.LoadString(STR_WED);	// 水
				str += strTmp;
			}
			if(theApp->m_tasks.tasks[i].date & 0x04)
			{
				strTmp.LoadString(STR_THU);	// 木
				str += strTmp;
			}
			if(theApp->m_tasks.tasks[i].date & 0x02)
			{
				strTmp.LoadString(STR_FRI);	// 金
				str += strTmp;
			}
			if(theApp->m_tasks.tasks[i].date & 0x01)
			{
				strTmp.LoadString(STR_SAT);	// 土
				str += strTmp;
			}
			strTmp.LoadString(STR_EXCEPTION);	// を除く
			str += strTmp;
		}
		if(theApp->m_tasks.tasks[i].excludetime)
		{
			str += " ";
			str += theApp->m_tasks.tasks[i].time0.Format("%#H:%M");
			str += " - ";
			str += theApp->m_tasks.tasks[i].time1.Format("%#H:%M");
			strTmp.LoadString(STR_EXCEPTION);	// を除く
			str += strTmp;
		}
		if(theApp->m_tasks.tasks[i].waitexit)
		{
			strTmp.LoadString(STR_VIEW_WAITPREV);	// 待機
			str = str + " " + strTmp;
		}
		if(theApp->m_tasks.tasks[i].syncprev)
		{
			strTmp.LoadString(STR_VIEW_LOOKPREV);	// 直前と連動
			str = str + " " + strTmp;
		}
		ListCtrl.SetItemText(i+1,1,str);


		// ２桁目「前回の実行日」の表示
		strTmp.LoadString(STR_VIEW_LIST_DATETIME);	// %m月%d日 %H:%M
		ListCtrl.SetItemText(i+1,2,
			theApp->m_tasks.tasks[i].time_lastexec.Format(strTmp));

		// ３桁目「次回の実行予定」の表示
		if(!theApp->m_tasks.tasks[i].exec)
		{
			str = "";
		}
		else if(theApp->m_tasks.tasks[i].apart == 0)
		{
			strTmp.LoadString(STR_VIEW_AT_LOGON);	// ログオン時
			str = strTmp;
		}
		else
		{
			strTmp.LoadString(STR_VIEW_LIST_DATETIME);	// %m月%d日 %H:%M
			str = theApp->m_tasks.CalcNextTime(i).Format(strTmp);
		}
		ListCtrl.SetItemText(i+1,3,str);

		// ４桁目「状態」のアイテムの表示
		if(!theApp->m_tasks.tasks[i].exec)
		{
			strTmp.LoadString(STR_VIEW_STOP_STATE);	// 停止中
			ListCtrl.SetItemText(i+1,4,strTmp);
		}
	}
	return TRUE;
}

// 新しいタスクの追加
void CSTaskView::OnTaskNew() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CString strTmp;
	CPropAdd0 prop0;
	CPropAdd1 prop1;
	CPropAdd2 prop2;
	CPropAdd4 prop4;
	CPropAdd5 prop5;	// 最終ページ（prop4 の変更内容を反映するためのダミーページ)

	strTmp.LoadString(STR_TASK_NEW_SHORT);	// タスクの機能設定
	CPropertySheet DlgAdd(strTmp, this);
	DlgAdd.AddPage(&prop0);
	DlgAdd.AddPage(&prop1);
	DlgAdd.AddPage(&prop2);
	DlgAdd.AddPage(&prop4);
	DlgAdd.AddPage(&prop5);		// 最後のページでは SetWizardButtons で「完了」表示する
	// ウイザード形式にする
	DlgAdd.m_psh.dwFlags |= PSH_WIZARD;
	// 「適用」ボタンを表示しない
	DlgAdd.m_psh.dwFlags=(DlgAdd.m_psh.dwFlags|PSH_NOAPPLYNOW);

	// 標準的な内容の設定
	prop0.m_select = 0;
	prop1.is_new = TRUE;
	prop1.m_exec = TRUE;
	prop1.trn_recom = &prop0.m_recom;
	prop1.trn_winfile = &prop0.m_winfile;
	prop1.trn_sel = &prop0.m_select;
	prop2.m_apart = 2;
	prop2.m_interval = 1;
	prop2.m_time0 = CTime(1999,1,1,0,0,0);
	prop2.m_time1 = CTime(1999,1,1,0,0,0);
	prop2.trn_fpass = &prop1.m_fpass;		// 文字列初期設定用
	prop2.trn_name = &prop1.m_name;			// 文字列初期設定用
	prop4.m_waitexit = TRUE;
	prop4.m_waitsec = 0;
	prop4.trn_name = &prop1.m_name;			// 文字列初期設定用
	prop4.m_cmb_syncexec = 0;
	prop5.trn_fpass = &prop1.m_fpass;		// 文字列初期設定用
	prop5.trn_name = &prop1.m_name;			// 文字列初期設定用
	if(DlgAdd.DoModal() == ID_WIZFINISH)
	{
		if(prop1.m_name != "" && prop1.m_fpass !="")
		{	// タスク名とファイル名が両方存在したら新規タスクとして認定する
			UINT date = (prop2.m_dsun ? 0x40:0) | (prop2.m_dmon ? 0x20:0) | (prop2.m_dtue ? 0x10:0) |
					(prop2.m_dwed ? 0x08:0) | (prop2.m_dthu ? 0x04:0) | (prop2.m_dfri ? 0x02:0) | (prop2.m_dsat ? 0x01:0);
			theApp->m_tasks.NewTask(prop1.m_name, prop1.m_fpass, prop1.m_param ,prop1.m_mes,
					prop1.m_exec, prop2.m_apart, prop2.m_interval , date,
					prop2.m_time, prop2.m_time0, prop2.m_time1,
					prop4.m_waitexit, prop4.m_waitsec, prop4.m_taskoff, prop4.m_taskoffcount,
					prop4.m_dialog, prop4.m_syncprev, prop4.m_cmb_syncexec);
		}
		else
		{
			CString strTmp, strAppName;
			strAppName.LoadString(STR_APPNAME);	// プログラム名
			strTmp.LoadString(STR_ERR_NOADD_NOTASKNAME);	// タスク名とタスクファイルが指定されていないので、新規登録されませんでした
			::MessageBox(this->m_hWnd, strTmp, strAppName, MB_OK|MB_ICONEXCLAMATION|MB_APPLMODAL );
		}

	}
	ChgViewStyle();	
	DisplayTasks();
}

// 選択中のタスクを消去する
void CSTaskView::OnTaskCut() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CListCtrl& ListCtrl = GetListCtrl();
	UINT i = (UINT)ListCtrl.GetFirstSelectedItemPosition();

	if(!i)
	{	// どれも選択されていなかったら
		return;
	}
	// i-2 : tasks[0] ... tasks[maxtask-1]
	theApp->m_tasks.DeleteTask(i-2);
	ChgViewStyle();	
	DisplayTasks();
}

// タスクをダブルクリックすると、タスクの機能設定を行う
void CSTaskView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	*pResult = 0;	// リターン　？？

	CListCtrl& ListCtrl = GetListCtrl();
	UINT i = (UINT)ListCtrl.GetFirstSelectedItemPosition();

	if(!i) return;	// 選択なし
	// タスク配列は 0 から始まる。
	if(i==1)
	{
		if(theApp->m_tasks.maxtask > 20) return;
		OnTaskNew();	// 先頭は「タスク追加」
		return;
	}
	i -= 2;	// タスク配列を０から始まるようにする

	EditTask(i);	// タスクの機能設定

}

// 選択中のタスクの機能設定
void CSTaskView::OnTaskConfig() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CListCtrl& ListCtrl = GetListCtrl();
	UINT i = (UINT)ListCtrl.GetFirstSelectedItemPosition();

	if(!i) return;	// 選択なし
	// タスク配列は 0 から始まる。
	if(i==1)
	{
		OnTaskNew();	// 先頭は「タスク追加」
		return;
	}
	i -= 2;	// タスク配列を０から始まるようにする
	EditTask(i);	
}


// 選択中のタスクの機能設定
BOOL CSTaskView::EditTask(UINT i)
{
	// ダイアログの準備
	CString strTmp, strAppName;
	strAppName.LoadString(STR_APPNAME);	// プログラム名

	CPropAdd1 prop1;
	CPropAdd2 prop2;
	CPropAdd3 prop3;
	strTmp.LoadString(STR_TASK_CONFIG_SHORT);	// タスクの機能設定
	CPropertySheet DlgAdd(strTmp, this);
	DlgAdd.AddPage(&prop1);
	DlgAdd.AddPage(&prop2);
	DlgAdd.AddPage(&prop3);

	// 「適用」ボタンを表示しない
	DlgAdd.m_psh.dwFlags=(DlgAdd.m_psh.dwFlags|PSH_NOAPPLYNOW);

	// 内容の設定
	prop1.m_name = theApp->m_tasks.tasks[i].name;
	prop1.m_fpass = theApp->m_tasks.tasks[i].fpass;
	prop1.m_param = theApp->m_tasks.tasks[i].param;
	prop1.m_mes = theApp->m_tasks.tasks[i].mes;
	prop1.m_exec = theApp->m_tasks.tasks[i].exec;
	prop2.m_apart = theApp->m_tasks.tasks[i].apart;
	prop2.m_interval = theApp->m_tasks.tasks[i].interval;
	if(theApp->m_tasks.tasks[i].date & 0x40) prop2.m_dsun = TRUE;
	if(theApp->m_tasks.tasks[i].date & 0x20) prop2.m_dmon = TRUE;
	if(theApp->m_tasks.tasks[i].date & 0x10) prop2.m_dtue = TRUE;
	if(theApp->m_tasks.tasks[i].date & 0x08) prop2.m_dwed = TRUE;
	if(theApp->m_tasks.tasks[i].date & 0x04) prop2.m_dthu = TRUE;
	if(theApp->m_tasks.tasks[i].date & 0x02) prop2.m_dfri = TRUE;
	if(theApp->m_tasks.tasks[i].date & 0x01) prop2.m_dsat = TRUE;
	prop2.m_time = theApp->m_tasks.tasks[i].excludetime;
	prop2.m_time0 = theApp->m_tasks.tasks[i].time0;
	prop2.m_time1 = theApp->m_tasks.tasks[i].time1;
	prop2.trn_fpass = &prop1.m_fpass;
	prop2.trn_name = &prop1.m_name;
	prop3.m_waitexit = theApp->m_tasks.tasks[i].waitexit;
	prop3.m_waitsec = theApp->m_tasks.tasks[i].waitsec;
	prop3.m_taskoff = theApp->m_tasks.tasks[i].taskoff;
	prop3.m_taskoffcount = theApp->m_tasks.tasks[i].taskoffcount;
	prop3.m_dialog = theApp->m_tasks.tasks[i].dialog;
	prop3.m_syncprev = theApp->m_tasks.tasks[i].syncprev;
	prop3.m_cmb_syncexec = theApp->m_tasks.tasks[i].syncexec;
	prop3.m_cnt_check = theApp->m_tasks.tasks[i].cnt_check;
	prop3.m_cnt_exec = theApp->m_tasks.tasks[i].cnt_exec;
	prop3.m_lastexec_date = theApp->m_tasks.tasks[i].time_lastexec;
	prop3.m_lastexec_time = theApp->m_tasks.tasks[i].time_lastexec;
	if(DlgAdd.DoModal() == TRUE)
	{
		if(prop1.m_name == "" || prop1.m_fpass =="")
		{
			strTmp.LoadString(STR_ERR_NOEDIT_NOTASKNAME);	// タスク名とタスクファイルが指定されていないので、変更されませんでした

			::MessageBox(this->m_hWnd, strTmp, strAppName, MB_OK|MB_ICONEXCLAMATION|MB_APPLMODAL );
			return FALSE;
		}
		theApp->m_tasks.tasks[i].name = prop1.m_name;
		theApp->m_tasks.tasks[i].fpass = prop1.m_fpass;
		theApp->m_tasks.tasks[i].param = prop1.m_param;
		theApp->m_tasks.tasks[i].mes = prop1.m_mes;
		theApp->m_tasks.tasks[i].exec = prop1.m_exec;
		theApp->m_tasks.tasks[i].apart = prop2.m_apart;
		theApp->m_tasks.tasks[i].interval = prop2.m_interval;
		theApp->m_tasks.tasks[i].date = (prop2.m_dsun ? 0x40:0) | (prop2.m_dmon ? 0x20:0) |
					(prop2.m_dtue ? 0x10:0) | (prop2.m_dwed ? 0x08:0) | (prop2.m_dthu ? 0x04:0) |
					(prop2.m_dfri ? 0x02:0) | (prop2.m_dsat ? 0x01:0);
		theApp->m_tasks.tasks[i].excludetime = prop2.m_time;
		theApp->m_tasks.tasks[i].time0 = prop2.m_time0;
		theApp->m_tasks.tasks[i].time1 = prop2.m_time1;
		theApp->m_tasks.tasks[i].waitexit = prop3.m_waitexit;
		theApp->m_tasks.tasks[i].waitsec = prop3.m_waitsec;
		theApp->m_tasks.tasks[i].taskoff = prop3.m_taskoff;
		theApp->m_tasks.tasks[i].taskoffcount = prop3.m_taskoffcount;
		theApp->m_tasks.tasks[i].dialog = prop3.m_dialog;
		theApp->m_tasks.tasks[i].syncprev = prop3.m_syncprev;
		theApp->m_tasks.tasks[i].syncexec = prop3.m_cmb_syncexec;
		theApp->m_tasks.tasks[i].cnt_check = prop3.m_cnt_check;
		theApp->m_tasks.tasks[i].cnt_exec = prop3.m_cnt_exec;
		theApp->m_tasks.tasks[i].time_lastexec = CTime(prop3.m_lastexec_date.GetYear(),
				prop3.m_lastexec_date.GetMonth(), prop3.m_lastexec_date.GetDay(),
				prop3.m_lastexec_time.GetHour(), prop3.m_lastexec_time.GetMinute(),
				prop3.m_lastexec_time.GetSecond());
		ChgViewStyle();	
		DisplayTasks();

	}
	return TRUE;
}

// 選択中のタスクの実行順序を、ひとつ上へ
void CSTaskView::OnTaskUp() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CListCtrl& ListCtrl = GetListCtrl();
	UINT i = (UINT)ListCtrl.GetFirstSelectedItemPosition();

	if(i<=2)
	{	// 現在のタスクを上に移せない
		// 0:選択なし、1: 「タスク追加」、2:「一番上のタスク」
		return;
	}
	theApp->m_tasks.SwapTask(i-2,i-3);
	ChgViewStyle();	
	DisplayTasks();

}

// 選択中のタスクの実行順序を、ひとつ下へ
void CSTaskView::OnTaskDown() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CListCtrl& ListCtrl = GetListCtrl();
	UINT i = (UINT)ListCtrl.GetFirstSelectedItemPosition();

	if(i==0) return;	// 選択なし
	if(i>theApp->m_tasks.maxtask)
	{	// 現在のタスクを下に移せない
		return;
	}
	theApp->m_tasks.SwapTask(i-2,i-1);
	ChgViewStyle();	
	DisplayTasks();
}

// 選択中のタスクを停止中にする。
void CSTaskView::OnTaskOff() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CListCtrl& ListCtrl = GetListCtrl();
	UINT i = (UINT)ListCtrl.GetFirstSelectedItemPosition();

	if(!i)
	{	// どれも選択されていなかったら
		return;
	}
	i -= 2;	// タスク配列を０から始まるようにする
	theApp->m_tasks.tasks[i].exec = FALSE;
	ChgViewStyle();	
	DisplayTasks();
}

// レジストリに保存する（強制）
void CSTaskView::OnWriteReg() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CString strAppName, strTmp;
	theApp->RegWriteAll();

	strAppName.LoadString(STR_APPNAME);	// プログラム名
	strTmp.LoadString(STR_MES_REGSAVED);
	::MessageBox(this->m_hWnd, strTmp, strAppName, MB_OK|MB_ICONINFORMATION);

}

// 選択中タスクを実行する
void CSTaskView::OnTaskExec() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CString strTmp, strAppName;
	strAppName.LoadString(STR_APPNAME);	// プログラム名
	CString mes;
	CListCtrl& ListCtrl = GetListCtrl();
	UINT i = (UINT)ListCtrl.GetFirstSelectedItemPosition();

	if(i<=1) return;	// 選択なし
	i -= 2;	// タスク配列を０から始まるようにする

	if(!theApp->m_tasks.ExecTask(i, this->m_hWnd))
	{
		if(theApp->m_tasks.g_faildlg)
		{	// ダイアログを表示する
			strTmp.LoadString(STR_NEXEC_EXECERR);	// タスク 「%s」 は実行できませんでした\n\nファイルが存在しないか壊れている可能性があります
			mes.Format(strTmp,
				theApp->m_tasks.tasks[i].name);
			::MessageBox(this->m_hWnd, mes, strAppName, MB_OK|MB_ICONEXCLAMATION|MB_APPLMODAL );
		}
	}
	ChgViewStyle();	
	DisplayTasks();
}

// 環境設定（全般）
void CSTaskView::OnConfig() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CString strTmp;
	CPropCnf1 prop1;
	CPropCnf2 prop2;
//	CPropCnf3 prop3;
	strTmp.LoadString(STR_CONFIG_SHORT);	// 環境設定
	CPropertySheet DlgAdd(strTmp, this);
	DlgAdd.AddPage(&prop1);
	DlgAdd.AddPage(&prop2);
//	DlgAdd.AddPage(&prop3);
	// 「適用」ボタンを表示しない
	DlgAdd.m_psh.dwFlags=(DlgAdd.m_psh.dwFlags|PSH_NOAPPLYNOW);

	prop1.m_delay = theApp->m_tasks.g_delay;
	prop1.m_dialog = theApp->m_tasks.g_dialog;
	prop1.m_dialog0 = theApp->m_tasks.g_dialog0;
	prop1.m_faildlg = theApp->m_tasks.g_faildlg;
	prop1.m_time = theApp->m_tasks.g_time;
	prop2.m_secure = theApp->m_tasks.g_secure;
	if(DlgAdd.DoModal() == TRUE)
	{
		theApp->m_tasks.g_delay = prop1.m_delay;
		theApp->m_tasks.g_dialog = prop1.m_dialog;
		theApp->m_tasks.g_dialog0 = prop1.m_dialog0;
		theApp->m_tasks.g_faildlg = prop1.m_faildlg;
		theApp->m_tasks.g_time = prop1.m_time;
		theApp->m_tasks.g_secure = prop2.m_secure;
	}
	ChgViewStyle();	
	DisplayTasks();
}

// デバッグモード
void CSTaskView::OnDebug() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CString strTmp, strAppName;
	strAppName.LoadString(STR_APPNAME);	// プログラム名
	if(!theApp->m_tasks.g_debug)
	{
		CDlgDebug dlg;
		dlg.m_date = CTime::GetCurrentTime();
		dlg.m_tim = CTime::GetCurrentTime();
		dlg.m_dlg0 = theApp->m_tasks.g_dialog0;
		dlg.m_dlg = theApp->m_tasks.g_dialog;
		if(dlg.DoModal() == TRUE)
		{
			theApp->m_tasks.g_debug = TRUE;	// デバッグモードに入る
			theApp->m_tasks.g_dialog0 = dlg.m_dlg0;
			theApp->m_tasks.g_dialog = dlg.m_dlg;
			theApp->m_tasks.g_debugtime = CTime(dlg.m_date.GetYear(),
				dlg.m_date.GetMonth(), dlg.m_date.GetDay(),
				dlg.m_tim.GetHour(), dlg.m_tim.GetMinute(),
				dlg.m_tim.GetSecond());
			theApp->m_tasks.ExecAllTask(this->m_hWnd);
			ChgViewStyle();	
			DisplayTasks();
		}
	}
	else
	{
		strTmp.LoadString(STR_MES_EXIT_DEBUG);	// 現在までの編集結果をリセットして、デバッグモードから抜けますか ？\n「いいえ」の場合は、再びデバッグ実行されます
		if(::MessageBox(this->m_hWnd, strTmp, strAppName, MB_YESNO|MB_ICONQUESTION|MB_APPLMODAL) == IDYES)
		{
			theApp->m_tasks.g_debug = FALSE;	// デバッグモードから抜ける
			theApp->RegReadAll();
			ChgViewStyle();	
			DisplayTasks();
		}
		else
		{
			CDlgDebug dlg;
			dlg.m_date = theApp->m_tasks.g_debugtime;
			dlg.m_tim = theApp->m_tasks.g_debugtime;
			dlg.m_dlg0 = theApp->m_tasks.g_dialog0;
			dlg.m_dlg = theApp->m_tasks.g_dialog;
			dlg.m_disable_cancel = TRUE;
			dlg.DoModal();
			theApp->m_tasks.g_dialog0 = dlg.m_dlg0;
			theApp->m_tasks.g_dialog = dlg.m_dlg;
			theApp->m_tasks.g_debugtime = CTime(dlg.m_date.GetYear(),
				dlg.m_date.GetMonth(), dlg.m_date.GetDay(),
				dlg.m_tim.GetHour(), dlg.m_tim.GetMinute(),
				dlg.m_tim.GetSecond());
			theApp->m_tasks.ExecAllTask(this->m_hWnd);
			ChgViewStyle();	
			DisplayTasks();
		}
	}
}

// これ以降、メニューやボタンの見かけの処理

void CSTaskView::OnUpdateDebug(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	if(theApp->m_tasks.g_debug)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);

}

void CSTaskView::OnUpdateTaskCut(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	ChangeMenuGray(pCmdUI);	
}

void CSTaskView::ChangeMenuGray(CCmdUI *pCmdUI)
{
	CListCtrl& ListCtrl = GetListCtrl();
	UINT i = (UINT)ListCtrl.GetFirstSelectedItemPosition();
	if(i>1) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);

}

void CSTaskView::OnUpdateTaskExec(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	ChangeMenuGray(pCmdUI);	
	
}

void CSTaskView::OnUpdateTaskOff(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	ChangeMenuGray(pCmdUI);	
	
}

void CSTaskView::OnUpdateTaskConfig(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	ChangeMenuGray(pCmdUI);	
	
}

void CSTaskView::OnUpdateTaskUp(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	CListCtrl& ListCtrl = GetListCtrl();
	UINT i = (UINT)ListCtrl.GetFirstSelectedItemPosition();
	if(i>2) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
	
}

void CSTaskView::OnUpdateTaskDown(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	CListCtrl& ListCtrl = GetListCtrl();
	UINT i = (UINT)ListCtrl.GetFirstSelectedItemPosition();
	if(i>1 && i<=theApp->m_tasks.maxtask) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

void CSTaskView::OnUpdateTaskNew(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください

	// タスクが２１個あったら、もう追加できない。
	if(theApp->m_tasks.maxtask > 20) pCmdUI->Enable(FALSE);
	else pCmdUI->Enable(TRUE);
}

void CSTaskView::OnHelpFinder() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CSTaskApp *theApp;
	theApp = (CSTaskApp *)AfxGetApp();

	theApp->WinHelp(0);

	return ;
}

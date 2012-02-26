// **********************************
// タスク配列の管理と、タスクの実行・実行判定
// **********************************

#include "stdafx.h"
#include "sTask.h"
#include "TaskArray.h"

#include "ProgressThread.h"		// プログレス・ダイアログ スレッド用

#include "sTaskProgress.h"			// デバッグ時のプログレス・ダイアログ用

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CTaskArray::CTaskArray()
{
	maxtask = 0;
	g_debug = FALSE;
}

CTaskArray::~CTaskArray()
{
}

// **********************************
// 新しいタスクを追加する
// **********************************
BOOL CTaskArray::NewTask(CString name, CString fpass, CString param, CString mes,
			BOOL exec, UINT apart, UINT interval, UINT date,
			BOOL excludetime, CTime time0, CTime time1,
			BOOL waitexit, UINT waitsec, BOOL taskoff, UINT taskoffcount,
			BOOL dialog, BOOL syncprev, int syncexec)
{
	if(maxtask>20) return FALSE;
	int i=maxtask;
	maxtask++;

	tasks[i].name=name;
	tasks[i].fpass=fpass;
	tasks[i].param=param;
	tasks[i].mes=mes;
	tasks[i].exec=exec;
	tasks[i].apart=apart;
	tasks[i].interval=interval;
	tasks[i].date=date;
	tasks[i].excludetime=excludetime;
	tasks[i].time0=time0;
	tasks[i].time1=time1;
	tasks[i].waitexit=waitexit;
	tasks[i].waitsec=waitsec;
	tasks[i].taskoff=taskoff;
	tasks[i].taskoffcount=taskoffcount;
	tasks[i].dialog=dialog;
	tasks[i].syncprev=syncprev;
	tasks[i].syncexec=syncexec;
	tasks[i].time_lastexec= CTime::GetCurrentTime();
	tasks[i].cnt_check = 0;
	tasks[i].cnt_exec = 0;

	return TRUE;
}

// **********************************
// 次回はいつからタスクを実行できるか、日時を返す
// 引数 i : タスク配列中のタスク番号
// **********************************
CTime CTaskArray::CalcNextTime(UINT i)
{
	int j;
	CTime prev = tasks[i].time_lastexec;
	CTime ret;

	if(tasks[i].apart == 0) // 何回かの起動に１回
		ret = CTime(1990,1,1,0,0,0);	// 現在時刻を返す
	else if(tasks[i].apart == 1)
	{	// 何日かに１回
		ret = prev + CTimeSpan::CTimeSpan(tasks[i].interval,0,0,0);
	}
	else if(tasks[i].apart == 2)
	{	// 何週かに１回
		ret = prev + CTimeSpan::CTimeSpan(tasks[i].interval*7,0,0,0);
	}
	else if(tasks[i].apart == 3)
	{	// 何ヶ月かに１回
		int year = prev.GetYear();
		int month = prev.GetMonth();
		month += tasks[i].interval;
		// 日付処理のバグ取り 99.11.25
		year += (month - 1) / 12;
		month = month % 12;
		if(month == 0) month = 12;
		ret = CTime(year, month, prev.GetDay(), prev.GetHour(), prev.GetMinute(), prev.GetSecond());
	}
	else ret = CTime::GetCurrentTime();

	// 曜日除外処理
	for(j=0;j<2;j++)
	{
		if((tasks[i].date & 0x40) && (ret.GetDayOfWeek() == 1)) ret += CTimeSpan::CTimeSpan(1,0,0,0); // 日曜
		if((tasks[i].date & 0x20) && (ret.GetDayOfWeek() == 2)) ret += CTimeSpan::CTimeSpan(1,0,0,0); // 月曜
		if((tasks[i].date & 0x10) && (ret.GetDayOfWeek() == 3)) ret += CTimeSpan::CTimeSpan(1,0,0,0);
		if((tasks[i].date & 0x08) && (ret.GetDayOfWeek() == 4)) ret += CTimeSpan::CTimeSpan(1,0,0,0);
		if((tasks[i].date & 0x04) && (ret.GetDayOfWeek() == 5)) ret += CTimeSpan::CTimeSpan(1,0,0,0);
		if((tasks[i].date & 0x02) && (ret.GetDayOfWeek() == 6)) ret += CTimeSpan::CTimeSpan(1,0,0,0);
		if((tasks[i].date & 0x01) && (ret.GetDayOfWeek() == 7)) ret += CTimeSpan::CTimeSpan(1,0,0,0);
	}

	// 時間の取り方（時間も間隔とするか）
	if(g_time)	// 時間を無視する
		ret = CTime(ret.GetYear(),ret.GetMonth(),ret.GetDay(),0,0,0);

	return ret;
}

// **********************************
// タスクをひとつ消去する
// **********************************
BOOL CTaskArray::DeleteTask(UINT i)
{
	// i : 選択されているアイテム 0 ... maxtask-1
	if(i>=maxtask) return FALSE;
	if(i==maxtask-1)
	{	// 最終アイテムの削除の場合
		maxtask--;
		return TRUE;
	}
	// 後ろを詰める処理をする
	// tasks[0] ... tasks[maxtask-2] となる
	for(UINT j=i; j<maxtask-1; j++)
		tasks[j] = tasks[j+1];	// j+1がmaxtaskになるまで繰り返し
	maxtask--;
	return TRUE;
}

// **********************************
// タスクを入れ替える
// **********************************
BOOL CTaskArray::SwapTask(UINT i, UINT j)
{
	if(i>=maxtask || j>=maxtask) return FALSE;
	CTask dummyTask;
	dummyTask = tasks[i];
	tasks[i] = tasks[j];
	tasks[j] = dummyTask;
	return TRUE;
}

// **********************************
// タスク（１つ）の実行
// **********************************
// 正常にタスクが実行されたら TRUE を返す
//
#include <process.h>	// _P_WAIT 用
BOOL CTaskArray::ExecTask(UINT i, HWND hWnd)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	CString strTmp1, strTmp2;
	PROCESS_INFORMATION pi;		// CreateProcess 用
	STARTUPINFO si;				// CreateProcess 用
	char strTmp3[MAX_PATH];		// CreateProcess 用
   
	_splitpath((LPCSTR)tasks[i].fpass, drive, dir, fname, ext);

	// デバッグモードでの表示
	if(g_debug)
	{

		strTmp2.LoadString(STR_EXEC_DEBUG);
		strTmp1.Format(strTmp2, tasks[i].name, tasks[i].fpass, tasks[i].param, ext);
		strTmp2.LoadString(STR_EXEC_DEBUG_TITLE);
		if(::MessageBox(hWnd, strTmp1, strTmp2, MB_YESNO|MB_ICONQUESTION|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND)
			!= IDYES) return FALSE;
	}
	// 非デバッグモードでの表示
	else if(tasks[i].dialog)
	{
		strTmp2.LoadString(STR_EXEC_NORM);
		if(tasks[i].mes == "")
			strTmp1.Format(strTmp2, tasks[i].name);
		else strTmp1 = tasks[i].mes;
		strTmp2.LoadString(STR_APPNAME);
		if(::MessageBox(hWnd, strTmp1, strTmp2, MB_YESNO|MB_ICONQUESTION|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND)
			!= IDYES) return FALSE;
	}

	// ウエイト挿入
	if(tasks[i].waitsec > 0) ::Sleep(tasks[i].waitsec*1000);

	// **********************************
	// 拡張子により、実行可能ファイルは SPAWN 、それ以外はシェル実行
	// **********************************
//			******************** ver 1.61b *.cmd も CreateProcess で実行
	if(!strcmpi(ext,".exe") || !strcmpi(ext,".com") || !strcmpi(ext,".bat") || !strcmpi(ext,".cmd"))	
	{
		if(tasks[i].waitexit)
		{	// 次のタスクを待機させる
			// (2002/05/11) spawnl から spawnlp に変更

//			******************** ver 1.61b プロセス実行方法を変更（スペースのあるパス名に対応）
//			if(_spawnlp(_P_WAIT,tasks[i].fpass,tasks[i].fpass,tasks[i].param,NULL) < 0)
//				return FALSE;


			// 途中にスペースのあるパス名への対応
			if(strstr(tasks[i].fpass, " ") != NULL)
				sprintf(strTmp3, "\"%s\" %s", tasks[i].fpass, tasks[i].param);
			else
				sprintf(strTmp3, "%s %s", tasks[i].fpass, tasks[i].param);

			// プロセスの作成・実行
			ZeroMemory(&si,sizeof(si));
			si.cb=sizeof(si);
			if(!::CreateProcess(NULL, strTmp3, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi))
				return FALSE;	// プロセス作成に失敗

			::WaitForSingleObject(pi.hProcess, INFINITE);		// プログラムが終了するまで待機
		}
		else
		{
//			******************** ver 1.61b プロセス実行方法を変更（スペースのあるパス名に対応）
//			if(_spawnlp(_P_NOWAIT,tasks[i].fpass,tasks[i].fpass,tasks[i].param,NULL) < 0)
//				return FALSE;
			// 途中にスペースのあるパス名への対応
			if(strstr(tasks[i].fpass, " ") != NULL)
				sprintf(strTmp3, "\"%s\" %s", tasks[i].fpass, tasks[i].param);
			else
				sprintf(strTmp3, "%s %s", tasks[i].fpass, tasks[i].param);

			// プロセスの作成・実行
			ZeroMemory(&si,sizeof(si));
			si.cb=sizeof(si);
			if(!::CreateProcess(NULL, strTmp3, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi))
				return FALSE;	// プロセス作成に失敗

		}

	}
	else
	{	// データファイルなどのシェル実行
		if((UINT)::ShellExecute(NULL,"OPEN",tasks[i].fpass,tasks[i].param,"",SW_SHOW) <= 32)
			return FALSE;
	}

	tasks[i].cnt_exec++;	// カウントアップ
	tasks[i].time_lastexec = CTime::GetCurrentTime();	// 現在時刻の代入
	if(tasks[i].cnt_exec >= tasks[i].taskoffcount && tasks[i].taskoff)
		tasks[i].exec = FALSE;	// 指定回数で待機状態へ
	tasks[i].cnt_check = 0;	// チェックカウンターはリセットされる
	if(i < maxtask-1)
	{	// 次のタスクが存在して、直前のタスクに同期させるモードだったら
		if(tasks[i+1].syncprev) tasks[i+1].time_lastexec = tasks[i].time_lastexec;
	}

	return TRUE;	// 正常に実行された
}

// **********************************
// すべてのタスクの実行条件を調べて、実行できる期日なら実行する
// **********************************
BOOL CTaskArray::ExecAllTask(HWND hWnd)
{
	UINT i;
	CTime Tnow, Tnext;
	CString strTmp1, strTmp2, strAppName;
	// ****** スレッド用 ******
	static ThreadArg p_ThreadParam;	// スレッド用パラメータ
	HANDLE h_ThreadHandle = NULL;	// スレッド・ハンドル
	HANDLE h_EventHandle = NULL;	// スレッド間通信用のイベント・ハンドル
	DWORD i_ThreadPID;
	// ****** モードレスダイアログ用（デバッグ時のみ使用） ******
	CDlgProgress *dlgDebugProgress;				// プログレス表示 モードレスダイアログ
	BOOL debug_dlg_OK = FALSE;		// モードレス・ダイアログ作成フラグ

	BOOL bLastTaskExec;				// 一つ前のタスクが実行された （syncexec 判定用）

	if(maxtask==0) return FALSE;	// 実行すべきタスクがない
	strAppName.LoadString(STR_APPNAME);	// あらかじめプログラム名をロードしておく（多用するため）

	// **********************************
	//  スレッド間通信用 イベント・ハンドルの作成
	// **********************************
	h_EventHandle = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	p_ThreadParam.sigHandle = h_EventHandle;	// スレッド内で使用するため
	// **********************************
	//  スレッドを作成する（プログレス・ダイアログ用） （デバッグ時は使用しない）
	// **********************************
	// (2002/05/11) デバッグモードでエラーのため、スレッドを利用しない
	if(g_dialog && !g_debug)
	{
		Thread_Global_m_progress = 0;	// プログレス・バーは 初期値 0
		// (2002/05/13) PID を受け取る引数を渡す（Win98でのスレッド作成不可に対応）
		h_ThreadHandle = ::CreateThread(NULL, 0, ::ProgressThreadMain, (LPVOID)&p_ThreadParam, 0, &i_ThreadPID);
		if(h_ThreadHandle == NULL)
		{
			// スレッドが作成できなかった
			::MessageBox(NULL, "Internal Error (Fail to Create Progress Dialog Thread)", "sTask (Warning)", MB_ICONWARNING|MB_OK|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND);
		}
		else
		{
			if(h_EventHandle != NULL)
			{
				// **********************************
				//  スレッドの開始を待つ  （３秒でタイムアウト）
				// **********************************
				if(::WaitForSingleObject(h_EventHandle, 3000) == WAIT_TIMEOUT)
				{
					// スレッドが正常に作成されていないと判断
					// スレッドを終了させる
					::CloseHandle(h_ThreadHandle);
					h_ThreadHandle = NULL;		// NULL をセットして、この後使われないようにする
				}
				else
				{
					::Sleep(500);	// ダイアログ作成まで余裕を与える 0.5sec
				}
				// イベントのリセット
				::ResetEvent(h_EventHandle);
			}
			else ::Sleep(2000);	// イベントが利用できないときは 2 秒待つ（安全策）
		}
	}
	// **********************************
	//  プログレス・ダイアログの作成 （デバッグ時のみ）
	//  モードレス・ダイアログを作成する
	// **********************************
	// (2002/05/11) デバッグモードでエラーのため、スレッドを利用しない
	// (2002/05/14) スレッド作成失敗時対応 (g_dialog && h_ThreadHandle == NULL)
	if(g_debug || (g_dialog && h_ThreadHandle == NULL))
	{
		dlgDebugProgress = new CDlgProgress();
		if(dlgDebugProgress->Create() != TRUE)
		{	// ダイアログが作成できない
			::MessageBox(NULL, "Internal Error (Fail to Create Progress Dialog (Modeless))", "sTask (Warning)", MB_ICONWARNING|MB_OK|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND);
			delete dlgDebugProgress;
		}
		else debug_dlg_OK = TRUE;
	}


	bLastTaskExec = FALSE;		// 前回のタスク実行状態のリセット

	// **********************************
	//  タスクを順に実行判定し、実行する
	// **********************************
	for(i=0; i<maxtask; i++)
	{
		// **********************************
		//  プログレス・ダイアログ表示のアップデート
		// **********************************
		if(g_dialog && h_ThreadHandle != NULL)
		{
			strTmp2.LoadString(STR_EXEC_DLGPRGRESS);
			strTmp1.Format(strTmp2, tasks[i].name, i+1, maxtask);
			::SetWindowText(Thread_lfn, strTmp1);	// ダイアログに直接書き込み
			Thread_Global_m_progress = ((i+1)*100)/maxtask;
			::PostMessage(Thread_dialog, WM_PAINT, 0, 0);	// 再描画メッセージ
		}
		// **********************************
		//  モードレス・ダイアログ表示のアップデート （デバッグ時のみ）
		// **********************************
		else if(debug_dlg_OK)
		{
			strTmp2.LoadString(STR_EXEC_DLGPRGRESS);
			strTmp1.Format(strTmp2, tasks[i].name, i+1, maxtask);
			dlgDebugProgress->SetDlgItemText(IDC_LFNAME, strTmp1);
			// ダイアログのプログレスコントロール
			dlgDebugProgress->m_progress_ctrl.SetPos(((i+1)*100)/maxtask);
			// ダイアログをアップデート
			dlgDebugProgress->RedrawWindow();
			dlgDebugProgress->UpdateWindow();
		}

		if(!tasks[i].exec)
		{	// OFFの場合
			if(g_dialog0)
			{	// ユーザー設定で停止と設定されたもの
				strTmp2.LoadString(STR_EXEC_STOP);
				strTmp1.Format(strTmp2, tasks[i].name);
				::MessageBox(hWnd, strTmp1, strAppName, MB_OK|MB_ICONINFORMATION|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND);
			}
			continue;	// 次のタスクへ
		}	// OFFの場合

		// 現在時刻の設定（デバッグ対応）
		if(g_debug) Tnow = g_debugtime;
		else Tnow = CTime::GetCurrentTime();

		tasks[i].cnt_check++;	// チェック回数のカウンター


		// 直前のタスクとの連動判定
		// syncecec = 0 : この機能を使わない
		// syncecec = 1 : 直前のタスク：実行 → このタスク：強制実行
		// syncecec = 2 : 直前のタスク：実実行 → このタスク：強制実行
		// syncecec = 3 : 直前のタスク：実行 → このタスク：強制実実行
		// syncecec = 4 : 直前のタスク：実実行 → このタスク：強制実実行

		if(tasks[i].syncexec == 3 && bLastTaskExec == TRUE && i != 0)
		{
			bLastTaskExec = FALSE;
			continue;		// 強制中止
		}
		if(tasks[i].syncexec == 4 && bLastTaskExec == FALSE && i != 0)
		{
			bLastTaskExec = FALSE;
			continue;		// 強制中止
		}

		if(((tasks[i].syncexec == 1 && bLastTaskExec == TRUE) || (tasks[i].syncexec == 2 && bLastTaskExec == FALSE)) && i != 0)
		{
			bLastTaskExec = FALSE;
			// 強制実行
		}
		else
		{	// 通常どおり条件判定を行う
			bLastTaskExec = FALSE;

			Tnext = CalcNextTime(i);
			if(Tnext > Tnow)
			{	// 日時が実行可能時期に達していない場合
				if(g_dialog0)
				{
					strTmp2.LoadString(STR_NEXEC_NREACH);
					strTmp1.Format(strTmp2,
						tasks[i].name, Tnext.GetYear(), Tnext.GetMonth(), Tnext.GetDay(), Tnext.GetHour(), Tnext.GetMinute(), Tnext.GetSecond(),
						Tnow.GetYear(), Tnow.GetMonth(), Tnow.GetDay(), Tnow.GetHour(), Tnow.GetMinute(), Tnow.GetSecond());
					::MessageBox(hWnd, strTmp1, strAppName, MB_OK|MB_ICONINFORMATION|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND);
				}
				continue;	// 次のタスクへ
			}	// 期限チェック終わり
			if(tasks[i].excludetime)
			{	// 時間除外がある場合
				long int t0,t1,tn;	// １日を秒単位に直し、前後関係を計算する
				t0 = tasks[i].time0.GetHour() * 3600 + tasks[i].time0.GetMinute() * 60 + tasks[i].time0.GetSecond();
				t1 = tasks[i].time1.GetHour() * 3600 + tasks[i].time1.GetMinute() * 60 + tasks[i].time1.GetSecond();
				tn = Tnow.GetHour() * 3600 + Tnow.GetMinute() * 60 + Tnow.GetSecond();
				if((t0 <= tn) && (tn <= t1))
				{	// 同一日内の計算 (t0 <= tn <= t1)
					if(g_dialog0)
					{
						strTmp2.LoadString(STR_NEXEC_EXCLUDETIME);
						strTmp1.Format(strTmp2,
							tasks[i].name, tasks[i].time0.GetHour(), tasks[i].time0.GetMinute(), tasks[i].time0.GetSecond(),
							tasks[i].time1.GetHour(), tasks[i].time1.GetMinute(), tasks[i].time1.GetSecond(),
							Tnow.GetHour(), Tnow.GetMinute(), Tnow.GetSecond());
						::MessageBox(hWnd, strTmp1, strAppName, MB_OK|MB_ICONINFORMATION|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND);
					}
					continue;	// 次のタスクへ
				}
				else if((t1 < t0) && ((t0 <= tn) || (tn <= t1)))
				{	// 同一日内の計算 (t0 <= tnと0:00 <= t1)
					if(g_dialog0)
					{
						strTmp2.LoadString(STR_NEXEC_EXCLUDETIME);
						strTmp1.Format(strTmp2,
							tasks[i].name, tasks[i].time0.GetHour(), tasks[i].time0.GetMinute(), tasks[i].time0.GetSecond(),
							tasks[i].time1.GetHour(), tasks[i].time1.GetMinute(), tasks[i].time1.GetSecond(),
							Tnow.GetHour(), Tnow.GetMinute(), Tnow.GetSecond());
						::MessageBox(hWnd, strTmp1, strAppName, MB_OK|MB_ICONINFORMATION|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND);
					}
					continue;	// 次のタスクへ
				}
			}	// 時間除外終わり
			if(tasks[i].date)
			{	// 曜日除外のチェック
				CString mes2 = "";
				if((Tnow.GetDayOfWeek() == 1) && (tasks[i].date & 0x40)) mes2.LoadString(STR_SUN);
				if((Tnow.GetDayOfWeek() == 2) && (tasks[i].date & 0x20)) mes2.LoadString(STR_MON);
				if((Tnow.GetDayOfWeek() == 3) && (tasks[i].date & 0x10)) mes2.LoadString(STR_TUE);
				if((Tnow.GetDayOfWeek() == 4) && (tasks[i].date & 0x08)) mes2.LoadString(STR_WED);
				if((Tnow.GetDayOfWeek() == 5) && (tasks[i].date & 0x04)) mes2.LoadString(STR_THU);
				if((Tnow.GetDayOfWeek() == 6) && (tasks[i].date & 0x02)) mes2.LoadString(STR_FRI);
				if((Tnow.GetDayOfWeek() == 7) && (tasks[i].date & 0x01)) mes2.LoadString(STR_SAT);
				
				if(mes2 != "")
				{
					if(g_dialog0)
					{
						strTmp2.LoadString(STR_NEXEC_EXCLUDEDAY);
						strTmp1.Format(strTmp2,
							tasks[i].name, mes2);
						::MessageBox(hWnd, strTmp1, strAppName, MB_OK|MB_ICONINFORMATION|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND);
					}
					continue;	// 次のタスクへ
				}
			}	// 曜日除外終わり
			if(tasks[i].apart == 0)
			{	// 起動回数カウントもの
				if(tasks[i].cnt_check < tasks[i].interval)
				{
					if(g_dialog0)
					{
						strTmp2.LoadString(STR_NEXEC_COUNTER);
						strTmp1.Format(strTmp2,
							tasks[i].name, tasks[i].interval, tasks[i].cnt_check);
						::MessageBox(hWnd, strTmp1, strAppName, MB_OK|MB_ICONINFORMATION|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND);
					}
					continue;
				}
			}

		}

		if(!ExecTask(i, hWnd))
		{	// タスクが実行できなかったら
			if(g_faildlg)
			{	// ダイアログを表示する （タスクが実行できなかったとき）
				strTmp2.LoadString(STR_NEXEC_EXECERR);
				strTmp1.Format(strTmp2,
					tasks[i].name);
					::MessageBox(hWnd, strTmp1, strAppName, MB_OK|MB_ICONEXCLAMATION|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND);
			}
			bLastTaskExec = FALSE;	// (syncexec 判定用）
		}
		else bLastTaskExec = TRUE;	// （syncexec 判定用）
	}

	if(g_dialog && h_ThreadHandle != NULL)
	{
		::Sleep(500);	// 500msec
		// **********************************
		//  プログレス・ダイアログを終了
		// **********************************
		::PostMessage(Thread_dialog, WM_CLOSE, 0, 0);
		// **********************************
		//  ウインドウが閉じられたことのシグナルを待つ  （３秒でタイムアウト）
		// **********************************
		if(h_EventHandle != NULL)
		{
			::WaitForSingleObject(h_EventHandle, 3000);
			::ResetEvent(h_EventHandle);	// イベントのリセット
		}
		::Sleep(500);	// 500msec
	}
	if(g_dialog && h_ThreadHandle != NULL)
	{
		// **********************************
		//  スレッドの消去（プログレス・ダイアログ用）
		// **********************************
		::CloseHandle(h_ThreadHandle);
	}
	// **********************************
	//  スレッド間通信用 イベント・ハンドルの開放
	// **********************************
	if(h_EventHandle != NULL) ::CloseHandle(h_EventHandle);

	// **********************************
	//  モードレス・ダイアログの開放 （デバッグ時のみ）
	// **********************************
	if(debug_dlg_OK)
	{
		::Sleep(500);	// 500msec
		dlgDebugProgress->DestroyWindow();
		delete dlgDebugProgress;
	}

	return TRUE;
}	


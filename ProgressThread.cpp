// **********************************
//  プログレス・ダイアログ表示用のスレッド
//
// (2002/05/11) 新規に作成
// **********************************
#include "stdafx.h"
#include "sTask.h"
#include "sTaskProgress.h"
#include "ProgressThread.h"

// スレッド間で相互に参照し合う変数
volatile HWND Thread_dialog;
volatile HWND Thread_lfn;
volatile HWND Thread_mes;
volatile int Thread_Global_m_progress;


// **********************************
// スレッドのメイン関数
// **********************************
DWORD WINAPI ProgressThreadMain(LPVOID p)
{
	ThreadArg *pArg=(ThreadArg*)p;	// 関数内でアクセスするために宣言しなおす

	CDlgProgress dlg;		// プログレス・ダイアログ

	// **********************************
	//  別スレッド用の初期化をするかどうかの判定フラグ
	// **********************************
	dlg.m_is_in_thread = TRUE;

	// 呼び出し元のスレッドで利用するため、グローバル変数に格納
	// なお、ウインド・ハンドラは、CDlgProgress の InitDialog で
	// 値がセットされるため、ポインタを渡している
	dlg.ptr_m_dialog = (HWND *)&Thread_dialog;
	dlg.ptr_m_lfn = (HWND *)&Thread_lfn;
	dlg.ptr_m_mes = (HWND *)&Thread_mes;
	dlg.m_progress_data = (int *)&Thread_Global_m_progress;

	// **********************************
	//  スレッドの開始を作成元のスレッドに通知 (スレッド間通信)
	// **********************************
	if(pArg->sigHandle != NULL) ::SetEvent(pArg->sigHandle);	// イベントのセット

	// **********************************
	//  プログレス・ダイアログの表示
	//  （表示の更新は、作成元スレッドより WM_PAINT メッセージを受けて行う）
	// **********************************
	dlg.DoModal();

	// **********************************
	//  スレッドの開始を作成元のスレッドに通知 (スレッド間通信)
	//  （ウインドウを閉じたことを通知）
	// **********************************
	if(pArg->sigHandle != NULL) ::SetEvent(pArg->sigHandle);	// イベントのセット

	return 0L;
}

// EOF

// **********************************
//  プログレス・ダイアログ表示用のスレッド
// **********************************
#include "stdafx.h"

// スレッド間で相互に参照し合う変数
// volatile で宣言すること
// コンパイラに最適化しないように指示する
volatile extern HWND Thread_dialog;
volatile extern HWND Thread_lfn;
volatile extern HWND Thread_mes;
volatile extern int Thread_Global_m_progress;

// スレッド起動時に渡すパラメータ
struct ThreadArg
{
	HANDLE sigHandle;
};

// スレッドのメイン関数
DWORD WINAPI ProgressThreadMain(LPVOID p);

// EOF
// ************************************************************
// Windows バージョン関連関数
//
// ************************************************************

#include "StdAfx.h"
#include "CheckWinVer.h"


// 自動実行のレジストリ位置
#define STR_REGKEY_RUN		"Software\\Microsoft\\Windows\\CurrentVersion\\Run"


// ************************************************************
// WindowsNT 系列と Windows 95 系列の判定
// WinNT 系列のとき TRUE
// ************************************************************
BOOL IsWinNT(void)
{
	OSVERSIONINFO ver;

	// OSバージョン
	ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx((LPOSVERSIONINFO)&ver);


	if(ver.dwPlatformId == VER_PLATFORM_WIN32_NT)
		return TRUE;

	return FALSE;
}

// ************************************************************
// HKLM の レジストリ Run に書き込み権限があるか調べる
// 権限あるとき TRUE
// ************************************************************

BOOL IsPossibleWriteHklmRun(void)
{
	BOOL ret = FALSE;
	HKEY hCU;
	char szProg[MAX_PATH] = "hi_soft_test_data";

	// テストデータが存在している場合は消去しておく
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, STR_REGKEY_RUN, 0, KEY_WRITE, &hCU) == ERROR_SUCCESS)
	{
		RegDeleteValue( hCU, "hi_soft_test");
		RegCloseKey(hCU);
	}

	// とりあえずテストデータを書き込んでみて、判定する
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, STR_REGKEY_RUN, 0, KEY_WRITE, &hCU) == ERROR_SUCCESS)
	{	// レジストリが書き込みモードでオープン出来たとき
		if(RegSetValueEx( hCU, "hi_soft_test", 0, REG_SZ, (const unsigned char *)szProg, strlen(szProg)) != ERROR_SUCCESS)
			ret = FALSE;	// 書き込みできなかったとき
		else
			ret = TRUE;		// 書き込み成功したとき
		RegCloseKey(hCU);
	}
	else
	{	// レジストリが書き込みモードでオープンできなかったとき
		ret = FALSE;
	}

	// テストデータを消去する
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, STR_REGKEY_RUN, 0, KEY_WRITE, &hCU) == ERROR_SUCCESS)
	{
		RegDeleteValue( hCU, "hi_soft_test");
		RegCloseKey(hCU);
	}
	
	return ret;
}
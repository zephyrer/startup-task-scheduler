// **********************************
// タスク配列の管理と、タスクの実行・実行判定
// クラス CTaskArray と クラス CTask
// **********************************

#if !defined(AFX_TASKARRAY_H__59BC1EE2_072D_11D3_9DA0_0090CCA1066A__INCLUDED_)
#define AFX_TASKARRAY_H__59BC1EE2_072D_11D3_9DA0_0090CCA1066A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// **********************************
// クラス CTask
// **********************************
class CTask
{
public:
	UINT cnt_exec;
	UINT cnt_check;
	CTime time_lastexec;
	CString name;
	CString fpass;
	CString param;
	CString mes;
	BOOL exec;
	UINT apart;
	UINT interval;
	UINT date;
	BOOL excludetime;
	CTime time0;
	CTime time1;
	BOOL waitexit;
	UINT waitsec;
	BOOL taskoff;
	UINT taskoffcount;
	BOOL dialog;
	BOOL syncprev;
	int syncexec;
	int wndstyle;
	int execdir_mode;
	CString execdir;
};

// **********************************
// クラス CTaskArray
// **********************************
class CTaskArray
{
public:
	BOOL ExecAllTask(HWND hWnd);
	CTime g_debugtime;
	BOOL g_debug;
	CString g_passwd;
	BOOL g_secure;
	BOOL g_dialog0;
	BOOL g_dialog;
	BOOL g_faildlg;
	BOOL g_time;
	UINT g_delay;
	int g_curdir_mode;
	CString g_curdir;
	BOOL ExecTask(UINT i, HWND hWnd=NULL);
	BOOL SwapTask(UINT i, UINT j);
	BOOL DeleteTask(UINT i);
	CTime CalcNextTime(UINT i);
	BOOL NewTask(CString name, CString fpass, CString param="", CString mes="",
			BOOL exec=FALSE, int wndstyle=0,  int execdir_mode=0, CString execdir="",
			UINT apart =2, UINT interval=1, UINT date=0,
			BOOL excludetime=FALSE, CTime time0=CTime(1999,1,1,0,0,0), CTime time1=CTime(1999,1,1,0,0,0),
			BOOL waitexit=FALSE, UINT waitsec=0, BOOL taskoff=FALSE, UINT taskoffcount=0,
			BOOL dialog=0, BOOL syncprev=FALSE, int syncexec=0);
	UINT maxtask;
	CTask tasks[21];
	CTaskArray();
	virtual ~CTaskArray();

};

#endif // !defined(AFX_TASKARRAY_H__59BC1EE2_072D_11D3_9DA0_0090CCA1066A__INCLUDED_)



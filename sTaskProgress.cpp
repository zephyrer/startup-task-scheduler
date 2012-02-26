// sTaskProgress.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "sTask.h"
#include "sTaskProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress ダイアログ


CDlgProgress::CDlgProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProgress)
	//}}AFX_DATA_INIT

	// **********************************
	//  別スレッド用の初期化をするかどうかの判定フラグ
	// **********************************
	m_is_in_thread = FALSE;		// 標準は、しない。
}


void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProgress)
	DDX_Control(pDX, IDC_PRGRESS, m_progress_ctrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProgress, CDialog)
	//{{AFX_MSG_MAP(CDlgProgress)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress メッセージ ハンドラ

BOOL CDlgProgress::Create(CWnd *wnd) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	return CDialog::Create(IDD, wnd);
}

BOOL CDlgProgress::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	// **********************************
	//  別スレッド用の初期化
	// **********************************
	if(m_is_in_thread)
	{
		*this->ptr_m_dialog = this->m_hWnd;
		*this->ptr_m_lfn = GetDlgItem(IDC_LFNAME)->m_hWnd;
		*this->ptr_m_mes = GetDlgItem(IDC_MES)->m_hWnd;
	}

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDlgProgress::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください

	// **********************************
	//  別スレッド用のダイアログ・アップデート
	//  *m_progress_data にデータをセットして WM_PAINT メッセージを送ってください
	// **********************************
	if(m_is_in_thread)
	{
		m_progress_ctrl.SetPos(*m_progress_data);
	}
	// 描画用メッセージとして CDialog::OnPaint() を呼び出してはいけません
}

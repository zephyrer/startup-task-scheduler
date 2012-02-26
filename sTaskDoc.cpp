// sTaskDoc.cpp : CSTaskDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "sTask.h"

#include "sTaskDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTaskDoc

IMPLEMENT_DYNCREATE(CSTaskDoc, CDocument)

BEGIN_MESSAGE_MAP(CSTaskDoc, CDocument)
	//{{AFX_MSG_MAP(CSTaskDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTaskDoc クラスの構築/消滅

CSTaskDoc::CSTaskDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

CSTaskDoc::~CSTaskDoc()
{
}

BOOL CSTaskDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSTaskDoc シリアライゼーション

void CSTaskDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: この位置に保存用のコードを追加してください。
	}
	else
	{
		// TODO: この位置に読み込み用のコードを追加してください。
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSTaskDoc クラスの診断

#ifdef _DEBUG
void CSTaskDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSTaskDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSTaskDoc コマンド

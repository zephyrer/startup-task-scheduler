// sTaskView.h : CSTaskView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STASKVIEW_H__E19A286D_0616_11D3_9DA0_0090CCA1066A__INCLUDED_)
#define AFX_STASKVIEW_H__E19A286D_0616_11D3_9DA0_0090CCA1066A__INCLUDED_

#include "sTask.h"	// ClassView によって追加されました。
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSTaskView : public CListView
{
protected: // シリアライズ機能のみから作成します。
	CSTaskView();
	DECLARE_DYNCREATE(CSTaskView)

// アトリビュート
public:
	CSTaskDoc* GetDocument();

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSTaskView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	CSTaskApp *theApp;
	void ChangeMenuGray(CCmdUI *pCmdUI);
	BOOL EditTask(UINT i);
	BOOL DisplayTasks();
	virtual ~CSTaskView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	BOOL ChgViewStyle(void);
	DWORD m_ViewStyle;
	CImageList m_sIconList,m_lIconList;
	//{{AFX_MSG(CSTaskView)
	afx_msg void OnViewIcon();
	afx_msg void OnViewReport();
	afx_msg void OnTaskNew();
	afx_msg void OnUpdateViewIcon(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewReport(CCmdUI* pCmdUI);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTaskCut();
	afx_msg void OnTaskUp();
	afx_msg void OnTaskDown();
	afx_msg void OnTaskOff();
	afx_msg void OnTaskConfig();
	afx_msg void OnWriteReg();
	afx_msg void OnTaskExec();
	afx_msg void OnConfig();
	afx_msg void OnDebug();
	afx_msg void OnUpdateDebug(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnUpdateTaskCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTaskExec(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTaskOff(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTaskConfig(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTaskUp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTaskDown(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTaskNew(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // sTaskView.cpp ファイルがデバッグ環境の時使用されます。
inline CSTaskDoc* CSTaskView::GetDocument()
   { return (CSTaskDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STASKVIEW_H__E19A286D_0616_11D3_9DA0_0090CCA1066A__INCLUDED_)

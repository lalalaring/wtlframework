

#include <sstream>
#include <ExtDockingWindow.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CColorPanelDockingWindow :
	public dockwins::CTitleDockingWindowImpl < CColorPanelDockingWindow, CWindow, dockwins::COutlookLikeExTitleDockingWindowTraits >
{
	typedef CColorPanelDockingWindow	thisClass;
	typedef dockwins::CTitleDockingWindowImpl< CColorPanelDockingWindow, CWindow, dockwins::COutlookLikeExTitleDockingWindowTraits> baseClass;
protected:
	void FillTree(CTreeViewCtrl& tree)
	{
		for (int i = 0; i < 10; i++)
		{
			std::basic_stringstream<TCHAR> text;
			text << _T("item ") << i;
			HTREEITEM hItem = tree.InsertItem(text.str().c_str(), 0, 1, TVI_ROOT, TVI_LAST);
			for (int j = 0; j < 5; j++)
			{
				std::basic_stringstream<TCHAR> text;
				text << _T("sub item ") << j;
				tree.InsertItem(text.str().c_str(), 0, 1, hItem, TVI_LAST);
			}
		}

	}
public:
	void OnDocked(HDOCKBAR hBar, bool bHorizontal)
	{
		DWORD dwStyle = GetWindowLong(GWL_STYLE)&(~WS_SIZEBOX);
		SetWindowLong(GWL_STYLE, dwStyle);

		baseClass::OnDocked(hBar, bHorizontal);
	}
	void OnUndocked(HDOCKBAR hBar)
	{
		DWORD dwStyle = GetWindowLong(GWL_STYLE) | WS_SIZEBOX;
		SetWindowLong(GWL_STYLE, dwStyle);

		baseClass::OnUndocked(hBar);
	}

	DECLARE_WND_CLASS(_T("CColorPanelDockingWindow"))
	BEGIN_MSG_MAP(thisClass)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		m_images.Create(16, 16, TRUE, 3, 0);

		assert(m_images.m_hImageList);
		CBitmap bmp;

		bmp.LoadBitmap(IDB_DUMMYBMP);

		m_images.Add(bmp, RGB(255, 255, 255));

		m_tree.Create(m_hWnd, NULL, NULL,
			TVS_SHOWSELALWAYS | TVS_HASBUTTONS |
			TVS_LINESATROOT | TVS_HASLINES |
			TVS_EDITLABELS | TVS_SHOWSELALWAYS |
			TVS_DISABLEDRAGDROP |
			WS_CHILD | WS_VISIBLE);
		m_tree.SetImageList(m_images, TVSIL_NORMAL);
		FillTree(m_tree);
		return 0;
	}
	LRESULT OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if (wParam != SIZE_MINIMIZED)
		{
			RECT rc;
			GetClientRect(&rc);
			::SetWindowPos(m_tree.m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
		}
		bHandled = FALSE;
		return 1;
	}
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		m_images.Destroy();
		return 0;
	}
protected:
	CTreeViewCtrl	m_tree;
	CImageList		m_images;
};


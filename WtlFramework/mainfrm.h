// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__45B9F696_4912_4555_8EA9_E985A36A3671__INCLUDED_)
#define AFX_MAINFRM_H__45B9F696_4912_4555_8EA9_E985A36A3671__INCLUDED_

#include <dbstate.h>
#ifdef STORE_PRIVATE_DATA_IN_XML
#include <stgxml.h>
#else
#include <stgreg.h>
#endif //STORE_PRIVATE_DATA_IN_XML


#include <DockingFrame.h>
#include "FoldersDockingWindow.h"
#include "ColorPanelDockWindow.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define CWM_INITIALIZE	(WMDF_LAST+1)

class CMainFrame :		
		public dockwins::CMDIDockingFrameImpl<CMainFrame>,
		public CUpdateUI<CMainFrame>,
		public CMessageFilter, 
		public CIdleHandler
{

	typedef dockwins::CMDIDockingFrameImpl<CMainFrame> baseClass;
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CMDICommandBarCtrl m_CmdBar;
	CFoldersDockingWindow	m_foldersDockWnd;

	CColorPanelDockingWindow m_colorPanelWnd;
#ifdef STORE_PRIVATE_DATA_IN_XML
	sstate::CWindowStateMgr<sstate::CStgXML> m_stateMgr;
	class  CConfig :	 
		public sstate::CStgXML
	{
	public:
		CConfig(void)
		{
            TCHAR name[_MAX_PATH];
			int len=::GetModuleFileName(_Module.GetModuleInstance(), name,_MAX_PATH);
			if(len!=0)
			{
				m_filename.assign(name,len);
				m_filename+=_T(".config");
				Load(m_filename.c_str());
			}
			if(Root()==0)
				Create();
		}
		~CConfig(void)
		{
			if(!m_filename.empty()
				&& (Root()!=0))
				Save(m_filename.c_str());
		}
	private:
		std::basic_string<TCHAR> m_filename;
	} m_config;
#else
	sstate::CWindowStateMgr<sstate::CStgRegistry> m_stateMgr;
#endif //STORE_PRIVATE_DATA_IN_XML

	
	CMainFrame(void)
	{
	}	
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(m_hWnd!=0)
		{
			if(baseClass::PreTranslateMessage(pMsg))
				return TRUE;

			HWND hWnd = MDIGetActive();
			if(hWnd != NULL)
				return (BOOL)::SendMessage(hWnd, WM_FORWARDMSG, 0, (LPARAM)pMsg);
		}
		return FALSE;
	}

	virtual BOOL OnIdle()
	{
		UIUpdateToolBar();
		UISetCheck(ID_VIEW_FOLDERS,m_foldersDockWnd.IsVisible());
		//UISetCheck(ID_VIEW_OUTPUT,m_outputDockWnd.IsVisible());
		//UISetCheck(ID_VIEW_TASKLIST,m_taskListDockWnd.IsVisible());
		UISetCheck(ID_VIEW_COLORPENEL,m_colorPanelWnd.IsVisible());
		//UISetCheck(ID_VIEW_COLORWHEEL,m_colorWheelWnd.IsVisible());
		//UISetCheck(ID_VIEW_LAYERS,m_layersWnd.IsVisible());
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_FOLDERS, UPDUI_MENUPOPUP)
		//UPDATE_ELEMENT(ID_VIEW_OUTPUT, UPDUI_MENUPOPUP)
		//UPDATE_ELEMENT(ID_VIEW_TASKLIST, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_COLORPENEL, UPDUI_MENUPOPUP)
		//UPDATE_ELEMENT(ID_VIEW_COLORWHEEL, UPDUI_MENUPOPUP)
		//UPDATE_ELEMENT(ID_VIEW_LAYERS, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(CWM_INITIALIZE, OnInitialize)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)


		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_WINDOW_CASCADE, OnWindowCascade)
		COMMAND_ID_HANDLER(ID_WINDOW_TILE_HORZ, OnWindowTile)
		COMMAND_ID_HANDLER(ID_WINDOW_ARRANGE, OnWindowArrangeIcons)

		COMMAND_TOGGLE_MEMBER_HANDLER(ID_VIEW_FOLDERS,m_foldersDockWnd)
		//COMMAND_TOGGLE_MEMBER_HANDLER(ID_VIEW_OUTPUT,m_outputDockWnd)
		//COMMAND_TOGGLE_MEMBER_HANDLER(ID_VIEW_TASKLIST,m_taskListDockWnd)
		COMMAND_TOGGLE_MEMBER_HANDLER(ID_VIEW_COLORPENEL, m_colorPanelWnd)
		//COMMAND_TOGGLE_MEMBER_HANDLER(ID_VIEW_COLORWHEEL, m_colorWheelWnd)
		//COMMAND_TOGGLE_MEMBER_HANDLER(ID_VIEW_LAYERS, m_layersWnd)

		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// create command bar window
		HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
		// attach menu
		//m_CmdBar.AttachMenu(GetMenu());
		// load command bar images
		//m_CmdBar.LoadImages(IDR_MAINFRAME);
		// remove old menu
		SetMenu(GetMenu());

		HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

		CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
		//AddSimpleReBarBand(hWndCmdBar);
		AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

		CreateSimpleStatusBar();

		CreateMDIClient();
		m_CmdBar.SetMDIClient(m_hWndMDIClient);

		UIAddToolBar(hWndToolBar);
		UISetCheck(ID_VIEW_TOOLBAR, 1);
		UISetCheck(ID_VIEW_STATUS_BAR, 1);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);
////////////////////////////////////////////
		InitializeDockingFrame(/*CStyle::sIgnoreSysSettings | CStyle::sGhostDrag*/);
		
		CRect rcDef(0,0,400,100);
		DWORD dwStyle=WS_OVERLAPPEDWINDOW | WS_POPUP| WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		m_foldersDockWnd.Create(m_hWnd,rcDef,_T("Folders"),dwStyle);
		DockWindow(m_foldersDockWnd,dockwins::CDockingSide(dockwins::CDockingSide::sLeft),
						0/*nBar*/,float(0.0)/*fPctPos*/,150/*nWidth*/,100/* nHeight*/);

		//m_taskListDockWnd.Create(m_hWnd,rcDef,_T("Task List"),dwStyle);
		//DockWindow(m_taskListDockWnd,dockwins::CDockingSide(dockwins::CDockingSide::sBottom),
		//				0/*nBar*/,float(0.0)/*fPctPos*/,100/*nWidth*/,100/* nHeight*/);

		//m_outputDockWnd.Create(m_hWnd,rcDef,_T("Output"),dwStyle);
		//DockWindow(m_outputDockWnd,dockwins::CDockingSide(dockwins::CDockingSide::sBottom),
		//				0/*nBar*/,float(0.5)/*fPctPos*/,100/*nWidth*/,100/* nHeight*/);

		m_colorPanelWnd.Create(m_hWnd, rcDef, _T("ColorPanel"), dwStyle);
		DockWindow(m_colorPanelWnd, dockwins::CDockingSide(dockwins::CDockingSide::sRight),
			0/*nBar*/, float(0.5)/*fPctPos*/, 100/*nWidth*/, 100/* nHeight*/);
		//m_colorWheelWnd.Create(m_hWnd, rcDef, _T("<<<<2>>>>"), dwStyle);
		//DockWindow(m_colorWheelWnd, dockwins::CDockingSide(dockwins::CDockingSide::sRight),
		//	0/*nBar*/, float(0.5)/*fPctPos*/, 100/*nWidth*/, 100/* nHeight*/);
		//m_layersWnd.Create(m_hWnd, rcDef, _T("<<<<<<3>>>>>>>"), dwStyle);
		//DockWindow(m_layersWnd, dockwins::CDockingSide(dockwins::CDockingSide::sRight),
		//	0/*nBar*/, float(0.5)/*fPctPos*/, 100/*nWidth*/, 100/* nHeight*/);

		PostMessage(CWM_INITIALIZE);
		return 0;
	}
	LRESULT OnInitialize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		sstate::CDockWndMgrEx mgrDockWnds(m_hWnd);
		//mgrDockWnds.Add(sstate::CDockingWindowStateAdapter<COutputDockingWindow> (m_outputDockWnd));
		mgrDockWnds.Add(sstate::CDockingWindowStateAdapter<CFoldersDockingWindow>(m_foldersDockWnd));
		//mgrDockWnds.Add(sstate::CDockingWindowStateAdapter<CTaskListDockingWindow>(m_taskListDockWnd));
		mgrDockWnds.Add(sstate::CDockingWindowStateAdapter<CColorPanelDockingWindow>(m_colorPanelWnd));
		//mgrDockWnds.Add(sstate::CDockingWindowStateAdapter<CColorWheelDockingWindow>(m_colorWheelWnd));
		//mgrDockWnds.Add(sstate::CDockingWindowStateAdapter<CLayersDockingWindow>(m_layersWnd));

		m_stateMgr.Initialize(m_hWnd);		

		m_stateMgr.Add(mgrDockWnds);

		m_stateMgr.Add(sstate::CRebarStateAdapter(m_hWndToolBar));
		m_stateMgr.Add(sstate::CToggleWindowAdapter(m_hWndStatusBar));

#ifdef STORE_PRIVATE_DATA_IN_XML
		if(!m_config
			 || !m_stateMgr.Restore(m_config) )
					m_stateMgr.RestoreDefault();
#else

		CRegKey key;
		if(key.Open(HKEY_CURRENT_USER,_T("SOFTWARE\\WTL Docking Window\\MDIExSample"),KEY_READ)==ERROR_SUCCESS)
		{
			sstate::CStgRegistry reg(key.Detach());
			m_stateMgr.Restore(reg);
		}
		else
			m_stateMgr.RestoreDefault();
#endif //STORE_PRIVATE_DATA_IN_XML
		UpdateLayout();
		return 0;
	}

	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled=FALSE;
#ifdef STORE_PRIVATE_DATA_IN_XML
		if(!(!m_config))
			m_stateMgr.Store(m_config);
#else
		CRegKey key;
		if(key.Create(HKEY_CURRENT_USER,_T("SOFTWARE\\WTL Docking Window\\MDIExSample"))==ERROR_SUCCESS)
		{
			sstate::CStgRegistry reg(key.Detach());
			m_stateMgr.Store(reg);
		}
#endif //STORE_PRIVATE_DATA_IN_XML
		return 0;
	}

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		PostMessage(WM_CLOSE);
		return 0;
	}

	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CChildFrame* pChild = new CChildFrame;
		pChild->CreateEx(m_hWndClient);

		// TODO: add code to initialize document

		return 0;
	}

	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		static BOOL bVisible = TRUE;	// initially visible
		bVisible = !bVisible;
		CReBarCtrl rebar = m_hWndToolBar;
		int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
		rebar.ShowBand(nBandIndex, bVisible);
		UISetCheck(ID_VIEW_TOOLBAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
		::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
		UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnWindowCascade(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		MDICascade();
		return 0;
	}

	LRESULT OnWindowTile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		MDITile();
		return 0;
	}

	LRESULT OnWindowArrangeIcons(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		MDIIconArrange();
		return 0;
	}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__45B9F696_4912_4555_8EA9_E985A36A3671__INCLUDED_)

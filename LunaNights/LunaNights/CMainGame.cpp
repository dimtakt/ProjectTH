#include "pch.h"
#include "CMainGame.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"
//#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CCameraMgr.h"
#include "CBmpMgr.h"
#include "CSceneMgr.h"
//#include "CTileMgr.h"

CMainGame::CMainGame() :
	m_dwTime(GetTickCount()),
	m_iFPS(0),
	m_DC(nullptr)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_DC = GetDC(g_hWnd);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/_Temp_Image/Back.bmp", L"Back");

	CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_LOGO);


#ifdef _DEBUG

	if (::AllocConsole() == TRUE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}

#endif // _DEBUG
}

void CMainGame::Update()
{
	CSceneMgr::Get_Instance()->Update();
	CCameraMgr::Get_Instance()->Update_CameraPos();
}

void CMainGame::Late_Update()
{
	CSceneMgr::Get_Instance()->Late_Update();
	CKeyMgr::Get_Instance()->Key_Update();

	//CCameraMgr::Get_Instance();
}

void CMainGame::Render()
{
	// FPS Ãâ·Â

	++m_iFPS;

	if (m_dwTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);
		SetWindowText(g_hWnd, m_szFPS);
		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}

	HDC	hBackDC = CBmpMgr::Get_Instance()->Find_Image(L"Back");


	CSceneMgr::Get_Instance()->Render(hBackDC);

	BitBlt(m_DC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);
}

void CMainGame::Release()
{
#ifdef _DEBUG

	FreeConsole();

#endif // _DEBUG

	CBmpMgr::Destroy_Instance();
	//CCameraMgr::Destroy_Instance();

	CKeyMgr::Destroy_Instance();
	//CLineMgr::Destroy_Instance();
	//CTileMgr::Destroy_Instance();

	CSceneMgr::Destroy_Instance();
	CObjMgr::Destroy_Instance();

	ReleaseDC(g_hWnd, m_DC);
}

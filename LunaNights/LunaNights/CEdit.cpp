#include "pch.h"
#include "CEdit.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CPlayer.h"
#include "CTileMgr.h"

#include "CSpritePropertyMgr.h"
#include "CCameraMgr.h"

CEdit::CEdit()
{
}

CEdit::~CEdit()
{
	Release();
}

void CEdit::Initialize()
{
	std::cout << "[INFO] Init Edit Scene.." << std::endl;
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/_Temp_Image/TempBG.bmp", L"STAGE1_01_BG");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Front.bmp", L"BG_Front");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/_Temp_Image/Tile.bmp", L"TILE");		// 테스트용 임시 타일
	FRAME_PROP tTILE = { TILECX, TILECY, 2, 1, 2 };							// 타일의 가로세로 길이 정보
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tTILE, L"TILE");	// 타일의 가로세로 길이 정보를 싱글톤에 담음

	CTileMgr::Get_Instance()->Initialize();

	std::cout << "[INFO] Init Completed." << std::endl;
}

void CEdit::Update()
{
	CTileMgr::Get_Instance()->Update();

	Key_Input();
}

void CEdit::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update();
}

void CEdit::Render(HDC _DC)
{
	CTileMgr::Get_Instance()->Render(_DC);
}

void CEdit::Release()
{
}

void CEdit::Key_Input()
{
	float fCameraMoveSpeed = 5.f;

	float fCameraX, fCameraY;
	// 현재 카메라가 비추는 좌표를 가져올 방법을 찾아야 함.
	CCameraMgr::Get_Instance()->Get_CameraPos(fCameraX, fCameraY);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		fCameraX -= fCameraMoveSpeed;

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		fCameraX += fCameraMoveSpeed;

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		fCameraY -= fCameraMoveSpeed;

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		fCameraY += fCameraMoveSpeed;

	CCameraMgr::Get_Instance()->Set_Pos(fCameraX, fCameraY);


	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		POINT	ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		float fTmpX, fTmpY;
		CCameraMgr::Get_Instance()->Get_CameraPos(fTmpX, fTmpY);
		fTmpX = fTmpX - (WINCX / 2);
		fTmpY = fTmpY - (WINCY / 2);

		POINT	ptCalcedMouse{ ptMouse.x + fTmpX, ptMouse.y + fTmpY };

		//ptMouse.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
		//ptMouse.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();


		// 좌클릭 시 0번 인자의 좌표에, 1번 인자의 타일 인덱스를, 2번 인자의 속성으로 변경
		// 현재 좌표 기준이 이상함. 수정해야 함
		CTileMgr::Get_Instance()->Picking_Tile(ptCalcedMouse, 1, 0);
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_PRIOR))	// PageUp
	{
		CTileMgr::Get_Instance()->Save_Tile();
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_NEXT))		// PageDown
	{
		CTileMgr::Get_Instance()->Load_Tile();
	}

}

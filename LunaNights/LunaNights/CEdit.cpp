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
	std::cout << "[INFO][CEdit::Initialize] Init Edit Scene.." << std::endl;
	
	// BmpMgr 을 통한 이미지 로드, 난잡해서 따로 뺐음
	LoadImages();

	iSelectedTileIndex = 0;

	
	CTileMgr::Get_Instance()->Initialize();

	std::cout << "[INFO][CEdit::Initialize] Initialize Complete!" << std::endl;
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
	//if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	//	fCameraX -= fCameraMoveSpeed;
	//if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	//	fCameraX += fCameraMoveSpeed;
	//if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
	//	fCameraY -= fCameraMoveSpeed;
	//if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
	//	fCameraY += fCameraMoveSpeed;
	//CCameraMgr::Get_Instance()->Set_Pos(fCameraX, fCameraY);
	
	if (CKeyMgr::Get_Instance()->Key_Down(VK_LEFT))
	{
		iSelectedTileIndex = (iSelectedTileIndex > 1) ? iSelectedTileIndex - 1 : 0;
		std::cout << "[INFO][CEdit::Key_Input] Current Selected Tile Index : " << iSelectedTileIndex << std::endl;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RIGHT))
	{
		int iMaxIndex = CSpritePropertyMgr::Get_Instance()->Find_Property(L"BG_Front").iFrameAmount;
		iSelectedTileIndex = (iSelectedTileIndex < iMaxIndex - 2) ? iSelectedTileIndex + 1 : iMaxIndex - 1;
		std::cout << "[INFO][CEdit::Key_Input] Current Selected Tile Index : " << iSelectedTileIndex << std::endl;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_UP))
	{
		int iMaxIndex = CSpritePropertyMgr::Get_Instance()->Find_Property(L"BG_Front").iFrameAmount;
		iSelectedTileIndex = (iSelectedTileIndex < iMaxIndex - 12) ? iSelectedTileIndex + 10 : iMaxIndex - 1;
		std::cout << "[INFO][CEdit::Key_Input] Current Selected Tile Index : " << iSelectedTileIndex << std::endl;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_DOWN))
	{
		iSelectedTileIndex = (iSelectedTileIndex > 11) ? iSelectedTileIndex - 10 : 0;
		std::cout << "[INFO][CEdit::Key_Input] Current Selected Tile Index : " << iSelectedTileIndex << std::endl;
	}


	// 좌클 시 선택된 인덱스의 타일을 칠함
	// 우클 시 선택된 인덱스의 타일을 0으로 되돌림
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

		CTileMgr::Get_Instance()->Picking_Tile(ptCalcedMouse, iSelectedTileIndex, 0);
	}
	else if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
	{
		POINT	ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		float fTmpX, fTmpY;
		CCameraMgr::Get_Instance()->Get_CameraPos(fTmpX, fTmpY);
		fTmpX = fTmpX - (WINCX / 2);
		fTmpY = fTmpY - (WINCY / 2);

		POINT	ptCalcedMouse{ ptMouse.x + fTmpX, ptMouse.y + fTmpY };

		CTileMgr::Get_Instance()->Picking_Tile(ptCalcedMouse, 0, 0);
	}




	if (CKeyMgr::Get_Instance()->Key_Down('O'))
	{
		CTileMgr::Get_Instance()->Save_Tile();
	}

	if (CKeyMgr::Get_Instance()->Key_Down('L'))
	{
		CTileMgr::Get_Instance()->Load_Tile();
	}

}

void CEdit::LoadImages()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/_Temp_Image/TempBG.bmp", L"STAGE1_01_BG");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Front.bmp", L"BG_Front");
	FRAME_PROP tBG_Front = { TILECX, TILECY, 28, 29, 800 };							// 타일의 가로세로 길이 정보
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tBG_Front, L"BG_Front");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Back.bmp", L"BG_Back");
	FRAME_PROP tBG_Back = { TILECX, TILECY, 28, 29, 800 };							// 타일의 가로세로 길이 정보
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tBG_Back, L"BG_Back");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_ForestTile.bmp", L"BG_ForestTile");
	FRAME_PROP tBG_ForestTile = { TILECX, TILECY, 21, 21, 440 };							// 타일의 가로세로 길이 정보
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tBG_ForestTile, L"BG_ForestTile");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/_Temp_Image/Tile.bmp", L"TILE");		// 테스트용 임시 타일
	FRAME_PROP tTILE = { TILECX, TILECY, 2, 1, 2 };							// 타일의 가로세로 길이 정보
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tTILE, L"TILE");	// 타일의 가로세로 길이 정보를 싱글톤에 담음

}
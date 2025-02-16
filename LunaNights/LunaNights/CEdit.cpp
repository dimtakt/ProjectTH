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
	iEditMode = 0;	// 0은 맵 편집, 1은 콜라이더 편집
	
	CTileMgr::Get_Instance()->Initialize();

	std::cout << "[INFO][CEdit::Initialize] Initialize Complete!" << std::endl;
}

void CEdit::Update()
{
	Key_Input();

	CTileMgr::Get_Instance()->Update();
	
	CCameraMgr::Get_Instance()->Update_CameraPos(TILECX * TILEX, TILECY * TILEY);
}

void CEdit::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update();
}

void CEdit::Render(HDC _DC)
{
	CTileMgr::Get_Instance()->Render(_DC);

	for (int i = 0; i < TILEX; i++)
		for (int j = 0; j < TILEY; j++)
		{
			POINT ptLUp{ i * (TILECX), j * (TILECY) };
			POINT ptRDown{ (i + 1) * (TILECX) - 1, (j + 1) * (TILECY) - 1};
			CCameraMgr::Get_Instance()->Get_RenderPos(ptLUp.x, ptLUp.y);
			CCameraMgr::Get_Instance()->Get_RenderPos(ptRDown.x, ptRDown.y);
			
			//Rectangle(_DC, ptLUp.x, ptLUp.y, ptRDown.x, ptRDown.y);
			MoveToEx(_DC, ptLUp.x, ptLUp.y, nullptr);
			LineTo(_DC, ptLUp.x, ptRDown.y);
			LineTo(_DC, ptRDown.x, ptRDown.y);
			LineTo(_DC, ptRDown.x, ptLUp.y);
			LineTo(_DC, ptLUp.x, ptLUp.y);
		}
}

void CEdit::Release()
{
}

void CEdit::Key_Input()
{

	float fCameraMoveSpeed = 5.f;
	float fCameraX, fCameraY;
	CCameraMgr::Get_Instance()->Get_CameraPos(fCameraX, fCameraY);
	
	int iMaxIndex;	// 선택 가능한 타일의 최대 인덱스
	if (iEditMode == 0)
		iMaxIndex = CSpritePropertyMgr::Get_Instance()->Find_Property(L"BG_Front").iFrameAmount;
	else if (iEditMode == 1)
		iMaxIndex = CSpritePropertyMgr::Get_Instance()->Find_Property(L"Collision_Tile").iFrameAmount;
	

	if (CKeyMgr::Get_Instance()->Key_Down(VK_LEFT))
	{
		iSelectedTileIndex = (iSelectedTileIndex > 1) ? iSelectedTileIndex - 1 : 0;
		std::cout << "[INFO][CEdit::Key_Input] Current Selected Tile Index : " << iSelectedTileIndex << std::endl;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RIGHT))
	{
		iSelectedTileIndex = (iSelectedTileIndex < iMaxIndex - 2) ? iSelectedTileIndex + 1 : iMaxIndex - 1;
		std::cout << "[INFO][CEdit::Key_Input] Current Selected Tile Index : " << iSelectedTileIndex << std::endl;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_UP))
	{
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
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
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
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RBUTTON))
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
		int iInfo = MessageBox(g_hWnd, L"타일 정보를 저장하시겠습니까?", L"Save", MB_YESNO);

		if (iInfo == IDYES)
		{
			if (iEditMode == 0)
				CTileMgr::Get_Instance()->Save_Tile(L"../Data/Tile.dat");
			else if (iEditMode == 1)
				CTileMgr::Get_Instance()->Save_Tile(L"../Data/Tile_Collision.dat");
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Down('L'))
	{
		int iInfo = MessageBox(g_hWnd, L"타일 정보를 불러오시겠습니까?", L"Load", MB_YESNO);

		if (iInfo == IDYES)
		{
			if (iEditMode == 0)
				CTileMgr::Get_Instance()->Load_Tile(L"../Data/Tile.dat", L"BG_Front");
			else if (iEditMode == 1)
				CTileMgr::Get_Instance()->Load_Tile(L"../Data/Tile_Collision.dat", L"Collision_Tile");
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_TAB))
	{
		//int iInfo = MessageBox(g_hWnd, L"편집 모드를 바꾸시겠습니까?", L"Change EditMode", MB_YESNO);

		//if (iInfo == IDYES)
		{
			if (iEditMode == 0)
			{
				CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile.dat");
				CTileMgr::Get_Instance()->Load_Tile(L"../Data/TempData_Tile_Collision.dat", L"Collision_Tile");
				iEditMode = 1;
			}
			else
			{
				CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile_Collision.dat");
				CTileMgr::Get_Instance()->Load_Tile(L"../Data/TempData_Tile.dat", L"BG_Front");
				iEditMode = 0;
			}

			std::cout << "[INFO][CEdit::Key_Input] Current EditMode is " << iEditMode << "!" << std::endl;

			iSelectedTileIndex = 0;
			std::cout << "[INFO][CEdit::Key_Input] Current Selected Tile Index : " << iSelectedTileIndex << std::endl;

			if (iEditMode == 0)
				CTileMgr::Get_Instance()->Set_PropName(L"BG_Front");
			else if (iEditMode == 1)
				CTileMgr::Get_Instance()->Set_PropName(L"Collision_Tile");
		}
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

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/Collision_Tile.bmp", L"Collision_Tile");
	FRAME_PROP tCollision_Tile = { TILECX, TILECY, 8, 4, 32 };							// 타일의 가로세로 길이 정보
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tCollision_Tile, L"Collision_Tile");



	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/_Temp_Image/Tile.bmp", L"TILE");		// 테스트용 임시 타일
	//FRAME_PROP tTILE = { TILECX, TILECY, 2, 1, 2 };							// 타일의 가로세로 길이 정보
	//CSpritePropertyMgr::Get_Instance()->Insert_Property(tTILE, L"TILE");	// 타일의 가로세로 길이 정보를 싱글톤에 담음

}
#include "pch.h"
#include "CEdit.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CPlayer.h"
#include "CTileMgr.h"
#include "CObjMgr.h"

#include "CSpritePropertyMgr.h"
#include "CCameraMgr.h"
#include "CAbstractFactory.h"
#include "CWolf.h"
#include "CWisp.h"
#include "CCollideRect.h"

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
	iSelectedMonsterIndex = 0;
	iEditMode = 1;	// 0은 맵 편집, 1은 콜라이더 편집
	iEditStage = 1;
	iTileX = 3;
	iTileY = 1;
	
	m_isPressedRect = false;

	CTileMgr::Get_Instance()->Initialize();
	CTileMgr::Get_Instance()->Set_PropName(L"Collision_Tile");

	std::cout << "[INFO][CEdit::Initialize] Initialize Complete!" << std::endl;
}

void CEdit::Update()
{
	Key_Input();

	CTileMgr::Get_Instance()->Update();
	

	// 스테이지마다 스테이지의 가로/세로 길이가 다름. 그것을 관리.
	switch (iEditStage)
	{
	case 1:			iTileX = 3; iTileY = 1; break;
	case 2:			iTileX = 2; iTileY = 1; break;
	case 3:			iTileX = 1; iTileY = 1; break;
	case 4:			iTileX = 2; iTileY = 1; break;
	default:		break;
	}

	CObjMgr::Get_Instance()->Update();
	CCameraMgr::Get_Instance()->Update_CameraPos(TILECX * TILEX * iTileX, TILECY * TILEY * iTileY);
}

void CEdit::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
	CTileMgr::Get_Instance()->Late_Update();
}

void CEdit::Render(HDC _DC)
{
	// 로드 한 이미지를, 현재의 iEditStage 에 맞게 보여줘야 함.
	HDC	hMemDC;
	FRAME_PROP tBGOriginProp;
	int iOutX = 0;
	int iOutY = 0;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // 최종적으로 렌더시킬 좌표.

	// 마우스 위치
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);


	if (iEditMode == 0)
	{

		switch (iEditStage)
		{
		case 1:
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_01_FRONT");
			tBGOriginProp = CSpritePropertyMgr::Get_Instance()->Find_Property(L"STAGE1_01_FRONT");
			BitBlt(_DC, iOutX, iOutY, tBGOriginProp.iCX, tBGOriginProp.iCY, hMemDC, 0, 0, SRCCOPY);

			break;

		case 2:
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_02_FRONT");
			tBGOriginProp = CSpritePropertyMgr::Get_Instance()->Find_Property(L"STAGE1_02_FRONT");
			BitBlt(_DC, iOutX, iOutY, tBGOriginProp.iCX, tBGOriginProp.iCY, hMemDC, 0, 0, SRCCOPY);

			break;

		case 3:
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_03_FRONT");
			tBGOriginProp = CSpritePropertyMgr::Get_Instance()->Find_Property(L"STAGE1_03_FRONT");
			BitBlt(_DC, iOutX, iOutY, tBGOriginProp.iCX, tBGOriginProp.iCY, hMemDC, 0, 0, SRCCOPY);

			break;

		case 4:
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_BOSS_FRONT");
			tBGOriginProp = CSpritePropertyMgr::Get_Instance()->Find_Property(L"STAGE1_BOSS_FRONT");
			BitBlt(_DC, iOutX, iOutY, tBGOriginProp.iCX, tBGOriginProp.iCY, hMemDC, 0, 0, SRCCOPY);

			break;


		default:
			break;
		}

	}

	else
	{

		// iEditMode 가 1일 때만 타일 렌더
		CTileMgr::Get_Instance()->Render(_DC);

	}


	// 편집용 격자 생성
	for (int i = 0; i < TILEX * iTileX; i++)
		for (int j = 0; j < TILEY * iTileY; j++)
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


	// 타일 렌더 이후, 그 위에 몬스터 등 CObj 렌더
	CObjMgr::Get_Instance()->Render(_DC);



	// 사각 콜라이더 설치할 곳 보여주기
	if (CKeyMgr::Get_Instance()->Key_Pressing('R'))
	{
		Rectangle(_DC, m_ptPressedRectStart.x + iOutX, m_ptPressedRectStart.y + iOutY, ptMouse.x, ptMouse.y); // 대신 LineTo로?
	}
}

void CEdit::Release()
{
}

void CEdit::Key_Input()
{

	// 현재 마우스의 위치를 계산하는 부분
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	float fTmpX, fTmpY;
	CCameraMgr::Get_Instance()->Get_CameraPos(fTmpX, fTmpY);
	fTmpX = fTmpX - (WINCX / 2);
	fTmpY = fTmpY - (WINCY / 2);

	POINT	ptCalcedMouse{ ptMouse.x + fTmpX, ptMouse.y + fTmpY };




	//float fCameraMoveSpeed = 5.f;
	//float fCameraX, fCameraY;
	//CCameraMgr::Get_Instance()->Get_CameraPos(fCameraX, fCameraY);
	
	int iMaxIndex;	// 선택 가능한 타일의 최대 인덱스
	int iMaxMonsterIndex;



	//if (iEditMode == 0)
	//	iMaxIndex = CSpritePropertyMgr::Get_Instance()->Find_Property(L"BG_Front").iFrameAmount;
	//else if (iEditMode == 1)

	
	// 선택한 타일의 변경 부분

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

	



	if (CKeyMgr::Get_Instance()->Key_Down('R'))
	{
		// 임의의 콜라이더 박스 시작점 생성
		m_isPressedRect = true;
		m_ptPressedRectStart = { ptMouse.x + (LONG)fTmpX, ptMouse.y + (LONG)fTmpY };
	}
	if (CKeyMgr::Get_Instance()->Key_Up('R'))
	{
		// 임의의 콜라이더 박스 끝점 생성
		m_isPressedRect = false;

		// m_ptPressedRectStart 와 뗄 당시의 ptMouse 값을 비교하여 나온 사각형의 Info 정보를 Create에 대입하면 될 듯
		// Create 함수를 수정해서라도 넣어야 할 것 같음
		CObjMgr::Get_Instance()->Add_Object(OBJ_COLLIDERECT, CAbstractFactory<CCollideRect>::CreateRectCollider(m_ptPressedRectStart.x, m_ptPressedRectStart.y, ptMouse.x + fTmpX, ptMouse.y + fTmpY));
	}






	// 선택한 몬스터의 변경 부분

	iMaxMonsterIndex = 1;	// 0 은 Wolf, 1 은 Wisp

	if (CKeyMgr::Get_Instance()->Key_Down('Q'))
	{
		iSelectedMonsterIndex = (iSelectedMonsterIndex > 1) ? iSelectedMonsterIndex - 1 : 0;
		std::cout << "[INFO][CEdit::Key_Input] Monster Index List.. [0 : Wolf] [1 : Wisp], Press [M] to Create" << std::endl;
		std::cout << "[INFO][CEdit::Key_Input] Current Selected Monster Index : " << iSelectedMonsterIndex << std::endl;
	}
	if (CKeyMgr::Get_Instance()->Key_Down('E'))
	{
		iSelectedMonsterIndex = (iSelectedMonsterIndex < iMaxMonsterIndex) ? iSelectedMonsterIndex + 1 : iMaxMonsterIndex;
		std::cout << "[INFO][CEdit::Key_Input] Monster Index List.. [0 : Wolf] [1 : Wisp], Press [M] to Create" << std::endl;
		std::cout << "[INFO][CEdit::Key_Input] Current Selected Monster Index : " << iSelectedMonsterIndex << std::endl;
	}


	// ? 키 누를 시 선택된 인덱스의 몬스터를 배치함
	// 몬스터의 리스트 삽입 방법은 이전 코드 참고하기
	// ? 키를 누르면 커서 위에 있는 몬스터는 다시 삭제됨 .. 
	// laterEdit
	if (CKeyMgr::Get_Instance()->Key_Down('M'))
	{
		switch (iSelectedMonsterIndex)
		{
		case 0:	// Wolf
			// 생성만 되고 push_back 이 안된듯?
			CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER_WOLF,CAbstractFactory<CWolf>::Create(ptCalcedMouse.x, ptCalcedMouse.y));
			break;

		case 1:	// Wisp
			CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER_WISP, CAbstractFactory<CWisp>::Create(ptCalcedMouse.x, ptCalcedMouse.y));
			break;

		default:
			break;
		}

		//CObjMgr::Get_Instance()->Add_Object();
	}





	// 좌클 시 선택된 인덱스의 타일을 칠함
	// 우클 시 선택된 인덱스의 타일을 0으로 되돌림
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		CTileMgr::Get_Instance()->Picking_Tile(ptCalcedMouse, iSelectedTileIndex, 0);
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RBUTTON))
	{
		CTileMgr::Get_Instance()->Picking_Tile(ptCalcedMouse, 0, 0);
	}


	if (CKeyMgr::Get_Instance()->Key_Down('1'))
	{
		if (iEditStage != 1)
		{
			switch (iEditStage)
			{
			//case 1:
			//	CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile_Collision_1-1.dat");
			//	CObjMgr::Get_Instance()->Save_Data(L"../Data/TempData_Monster_Info_1-1.dat");
			//	break;
			case 2:
				CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile_Collision_1-2.dat");
				CObjMgr::Get_Instance()->Save_Data(L"../Data/TempData_Monster_Info_1-2.dat");
				break;
			case 3:
				CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile_Collision_1-3.dat");
				CObjMgr::Get_Instance()->Save_Data(L"../Data/TempData_Monster_Info_1-3.dat");
				break;
			case 4:
				CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile_Collision_1-Boss.dat");
				CObjMgr::Get_Instance()->Save_Data(L"../Data/TempData_Monster_Info_1-Boss.dat");
				break;
			default:	break;
			}
			
			iEditStage = 1;
			CTileMgr::Get_Instance()->Load_Tile(L"../Data/TempData_Tile_Collision_1-1.dat", L"Collision_Tile");
			CObjMgr::Get_Instance()->Load_Data(L"../Data/Monster_Info_1-1.dat");

		}
		
		std::cout << "[INFO][CEdit::Key_Input] Current EditStage is " << iEditStage << "!" << std::endl;
	}
	if (CKeyMgr::Get_Instance()->Key_Down('2'))
	{
		if (iEditStage != 2)
		{
			switch (iEditStage)
			{
			case 1:
				CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile_Collision_1-1.dat");
				CObjMgr::Get_Instance()->Save_Data(L"../Data/TempData_Monster_Info_1-1.dat");
				break;
			//case 2:
			//	CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile_Collision_1-2.dat");
			//	CObjMgr::Get_Instance()->Save_Data(L"../Data/TempData_Monster_Info_1-2.dat");
			//	break;
			case 3:
				CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile_Collision_1-3.dat");
				CObjMgr::Get_Instance()->Save_Data(L"../Data/TempData_Monster_Info_1-3.dat");
				break;
			case 4:
				CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile_Collision_1-Boss.dat");
				CObjMgr::Get_Instance()->Save_Data(L"../Data/TempData_Monster_Info_1-Boss.dat");
				break;
			default:	break;
			}
			
			iEditStage = 2;
			CTileMgr::Get_Instance()->Load_Tile(L"../Data/TempData_Tile_Collision_1-2.dat", L"Collision_Tile");
			CObjMgr::Get_Instance()->Load_Data(L"../Data/Monster_Info_1-2.dat");

		}

		std::cout << "[INFO][CEdit::Key_Input] Current EditStage is " << iEditStage << "!" << std::endl;
	}
	if (CKeyMgr::Get_Instance()->Key_Down('3'))
	{
		if (iEditStage != 3)
		{
			switch (iEditStage)
			{
			case 1:
				CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile_Collision_1-1.dat");
				CObjMgr::Get_Instance()->Save_Data(L"../Data/TempData_Monster_Info_1-1.dat");
				break;
			case 2:
				CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile_Collision_1-2.dat");
				CObjMgr::Get_Instance()->Save_Data(L"../Data/TempData_Monster_Info_1-2.dat");
				break;
			//case 3:
			//	CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile_Collision_1-3.dat");
			//	CObjMgr::Get_Instance()->Save_Data(L"../Data/TempData_Monster_Info_1-3.dat");
			//	break;
			case 4:
				CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile_Collision_1-Boss.dat");
				CObjMgr::Get_Instance()->Save_Data(L"../Data/TempData_Monster_Info_1-Boss.dat");
				break;
			default:	break;
			}

			iEditStage = 3;
			CTileMgr::Get_Instance()->Load_Tile(L"../Data/TempData_Tile_Collision_1-3.dat", L"Collision_Tile");
			CObjMgr::Get_Instance()->Load_Data(L"../Data/Monster_Info_1-3.dat");


		}

		std::cout << "[INFO][CEdit::Key_Input] Current EditStage is " << iEditStage << "!" << std::endl;
	}
	if (CKeyMgr::Get_Instance()->Key_Down('4'))
	{
		if (iEditStage != 4)
		{
			switch (iEditStage)
			{
			case 1:
				CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile_Collision_1-1.dat");
				CObjMgr::Get_Instance()->Save_Data(L"../Data/TempData_Monster_Info_1-1.dat");
				break;
			case 2:
				CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile_Collision_1-2.dat");
				CObjMgr::Get_Instance()->Save_Data(L"../Data/TempData_Monster_Info_1-2.dat");
				break;
			case 3:
				CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile_Collision_1-3.dat");
				CObjMgr::Get_Instance()->Save_Data(L"../Data/TempData_Monster_Info_1-3.dat");
				break;
			//case 4:
			//	CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile_Collision_1-Boss.dat");
			//	CObjMgr::Get_Instance()->Save_Data(L"../Data/TempData_Monster_Info_1-Boss.dat");
			//	break;
			default:	break;
			}
			
			iEditStage = 4;
			CTileMgr::Get_Instance()->Load_Tile(L"../Data/TempData_Tile_Collision_1-Boss.dat", L"Collision_Tile");
			CObjMgr::Get_Instance()->Load_Data(L"../Data/Monster_Info_1-Boss.dat");


		}

		std::cout << "[INFO][CEdit::Key_Input] Current EditStage is " << iEditStage << "!" << std::endl;
	}


	if (CKeyMgr::Get_Instance()->Key_Down('O') && iEditMode == 1)
	{
		int iInfo = MessageBox(g_hWnd, L"타일 정보를 저장하시겠습니까?", L"Save", MB_YESNO);

		if (iInfo == IDYES)
		{
			// 맵 타일 편집 모드 제거함.
			//if (iEditMode == 0)
			//	CTileMgr::Get_Instance()->Save_Tile(L"../Data/Tile.dat");
			//else 
			
			if (iEditMode == 1)
			{
				switch (iEditStage)
				{
				case 1:
					CTileMgr::Get_Instance()->Save_Tile(L"../Data/Tile_Collision_1-1.dat");
					CObjMgr::Get_Instance()->Save_Data(L"../Data/Monster_Info_1-1.dat");
					break;
				case 2:
					CTileMgr::Get_Instance()->Save_Tile(L"../Data/Tile_Collision_1-2.dat");
					CObjMgr::Get_Instance()->Save_Data(L"../Data/Monster_Info_1-2.dat");
					break;
				case 3:
					CTileMgr::Get_Instance()->Save_Tile(L"../Data/Tile_Collision_1-3.dat");
					CObjMgr::Get_Instance()->Save_Data(L"../Data/Monster_Info_1-3.dat");
					break;
				case 4:
					CTileMgr::Get_Instance()->Save_Tile(L"../Data/Tile_Collision_1-Boss.dat");
					CObjMgr::Get_Instance()->Save_Data(L"../Data/Monster_Info_1-Boss.dat");
					break;
				default:	break;
				}

			}
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Down('L') && iEditMode == 1)
	{
		int iInfo = MessageBox(g_hWnd, L"타일 정보를 불러오시겠습니까?", L"Load", MB_YESNO);

		if (iInfo == IDYES)
		{
			// 맵 타일 편집 모드 제거함.
			//if (iEditMode == 0)
			//	CTileMgr::Get_Instance()->Load_Tile(L"../Data/Tile.dat", L"BG_Front");
			//else
			if (iEditMode == 1)
			{
				switch (iEditStage)
				{
				case 1:
					CTileMgr::Get_Instance()->Load_Tile(L"../Data/Tile_Collision_1-1.dat", L"Collision_Tile");
					CObjMgr::Get_Instance()->Load_Data(L"../Data/Monster_Info_1-1.dat");
					break;
				case 2:
					CTileMgr::Get_Instance()->Load_Tile(L"../Data/Tile_Collision_1-2.dat", L"Collision_Tile");
					CObjMgr::Get_Instance()->Load_Data(L"../Data/Monster_Info_1-2.dat");
					break;
				case 3:
					CTileMgr::Get_Instance()->Load_Tile(L"../Data/Tile_Collision_1-3.dat", L"Collision_Tile");
					CObjMgr::Get_Instance()->Load_Data(L"../Data/Monster_Info_1-3.dat");
					break;
				case 4:
					CTileMgr::Get_Instance()->Load_Tile(L"../Data/Tile_Collision_1-Boss.dat", L"Collision_Tile");
					CObjMgr::Get_Instance()->Load_Data(L"../Data/Monster_Info_1-Boss.dat");
					break;
				default:	break;
				}

			}
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_TAB))
	{
		//int iInfo = MessageBox(g_hWnd, L"편집 모드를 바꾸시겠습니까?", L"Change EditMode", MB_YESNO);

		//if (iInfo == IDYES)
		{
			if (iEditMode == 0)
			{
				switch (iEditStage)
				{
				case 1:	CTileMgr::Get_Instance()->Load_Tile(L"../Data/TempData_Tile_Collision_1-1.dat", L"Collision_Tile"); break;
				case 2:	CTileMgr::Get_Instance()->Load_Tile(L"../Data/TempData_Tile_Collision_1-2.dat", L"Collision_Tile"); break;
				case 3:	CTileMgr::Get_Instance()->Load_Tile(L"../Data/TempData_Tile_Collision_1-3.dat", L"Collision_Tile"); break;
				case 4:	CTileMgr::Get_Instance()->Load_Tile(L"../Data/TempData_Tile_Collision_1-Boss.dat", L"Collision_Tile"); break;

				default:
					break;
				}
				iEditMode = 1;
			}
			else
			{
				switch (iEditStage)
				{
				case 1:	CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile_Collision_1-1.dat"); break;
				case 2:	CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile_Collision_1-2.dat"); break;
				case 3:	CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile_Collision_1-3.dat"); break;
				case 4:	CTileMgr::Get_Instance()->Save_Tile(L"../Data/TempData_Tile_Collision_1-Boss.dat"); break;

				default:
					break;
				}
				iEditMode = 0;
			}

			std::cout << "[INFO][CEdit::Key_Input] Current EditMode is " << iEditMode << "!" << std::endl;
			std::cout << "[INFO][CEdit::Key_Input] Current EditStage is " << iEditStage << "!" << std::endl;

			//iSelectedTileIndex = 0;
			//std::cout << "[INFO][CEdit::Key_Input] Current Selected Tile Index : " << iSelectedTileIndex << std::endl;

			if (iEditMode == 1)
				CTileMgr::Get_Instance()->Set_PropName(L"Collision_Tile");
		}
	}
}

void CEdit::LoadImages()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/_Temp_Image/TempBG.bmp", L"STAGE1_01_BG");

#pragma region legacy
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Front.bmp", L"BG_Front");
	//FRAME_PROP tBG_Front = { TILECX, TILECY, 28, 29, 800 };							// 타일의 가로세로 길이 정보
	//CSpritePropertyMgr::Get_Instance()->Insert_Property(tBG_Front, L"BG_Front");

	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Back.bmp", L"BG_Back");
	//FRAME_PROP tBG_Back = { TILECX, TILECY, 28, 29, 800 };							// 타일의 가로세로 길이 정보
	//CSpritePropertyMgr::Get_Instance()->Insert_Property(tBG_Back, L"BG_Back");

	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_ForestTile.bmp", L"BG_ForestTile");
	//FRAME_PROP tBG_ForestTile = { TILECX, TILECY, 21, 21, 440 };							// 타일의 가로세로 길이 정보
	//CSpritePropertyMgr::Get_Instance()->Insert_Property(tBG_ForestTile, L"BG_ForestTile");

	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/_Temp_Image/Tile.bmp", L"TILE");		// 테스트용 임시 타일
	//FRAME_PROP tTILE = { TILECX, TILECY, 2, 1, 2 };							// 타일의 가로세로 길이 정보
	//CSpritePropertyMgr::Get_Instance()->Insert_Property(tTILE, L"TILE");	// 타일의 가로세로 길이 정보를 싱글톤에 담음

#pragma endregion



	// 콜라이더 타일 로드
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/Collision_Tile.bmp", L"Collision_Tile");
	FRAME_PROP tCollision_Tile = { TILECX, TILECY, 8, 4, 32 };							// 타일의 가로세로 길이 정보
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tCollision_Tile, L"Collision_Tile");



	// 스테이지 1-1 이미지 로드
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Stage1/1-01_Merge0103.bmp", L"STAGE1_01_FRONT");
	FRAME_PROP tSTAGE1_01_FRONT = { 4080, 816 };							// 타일의 가로세로 길이 정보
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tSTAGE1_01_FRONT, L"STAGE1_01_FRONT");

	// 스테이지 1-2 이미지 로드
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Stage1/1-02_Merge0405.bmp", L"STAGE1_02_FRONT");
	FRAME_PROP tSTAGE1_02_FRONT = { 2720, 816 };							// 타일의 가로세로 길이 정보
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tSTAGE1_02_FRONT, L"STAGE1_02_FRONT");

	// 스테이지 1-3 이미지 로드
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Stage1/1-03_Merge.bmp", L"STAGE1_03_FRONT");
	FRAME_PROP tSTAGE1_03_FRONT = { 1360, 816 };							// 타일의 가로세로 길이 정보
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tSTAGE1_03_FRONT, L"STAGE1_03_FRONT");

	// 스테이지 1-Boss 이미지 로드
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Stage1/1-Boss_Merge.bmp", L"STAGE1_BOSS_FRONT");
	FRAME_PROP tSTAGE1_BOSS_FRONT = { 2720, 816 };							// 타일의 가로세로 길이 정보
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tSTAGE1_BOSS_FRONT, L"STAGE1_BOSS_FRONT");



}
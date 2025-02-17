#include "pch.h"
#include "CStage1_01.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CPlayer.h"
#include "CTileMgr.h"
#include "CSpritePropertyMgr.h"

#include "SoundMgr.h"
#include "CCameraMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CUI.h"


CStage1_01::CStage1_01()
{
}

CStage1_01::~CStage1_01()
{
	Release();
}

void CStage1_01::Initialize()
{
	m_iTileTimesX = 3;
	m_iTileTimesY = 1;

	// 플레이어 생성
	CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());


	// 이후 몬스터나 타일 등의 생성 및 이미지 불러오기는 여기에..
	//..
	CObjMgr::Get_Instance()->Add_Object(OBJ_UI, CAbstractFactory<CUI>::Create());
	
	
	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/_Temp_Image/TempBG.bmp", L"STAGE1_01_BG");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Stage1/1-01_Merge0103.bmp", L"STAGE1_01_FRONT");
	FRAME_PROP tSTAGE1_01_FRONT = { 4080, 816 };							// 타일의 가로세로 길이 정보
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tSTAGE1_01_FRONT, L"STAGE1_01_FRONT");


	CTileMgr::Get_Instance()->Initialize();



	// 맵을 하나의 통 텍스쳐를 쓸 것으로 변경함에 따라,
	// 기존 타일 텍스쳐를 로드하지 않고, 콜라이더 정보만을 로드하도록 수정
	CTileMgr::Get_Instance()->Load_Tile(nullptr, nullptr, true,
										L"../Data/Tile_Collision_1-1.dat", L"Collision_Tile");
	
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Front.bmp", L"BG_Front");
	//FRAME_PROP tBG_Front = { TILECX, TILECY, 28, 29, 800 };							// 타일의 가로세로 길이 정보
	//CSpritePropertyMgr::Get_Instance()->Insert_Property(tBG_Front, L"BG_Front");

	//CTileMgr::Get_Instance()->Load_Tile(L"../Data/Tile.dat", L"BG_Front", true,
	//									L"../Data/Tile_Collision.dat", L"Collision_Tile");

	
	
	CObjMgr::Get_Instance()->Update();		// ㅋㅋ;
	// CTileMgr::Get_Instance()->Load_Tile(L"../Data/Tile_Collision.dat", L"Collision_Tile");	// 콜라이더 데이터

	CSoundMgr::Get_Instance()->PlayBGM(L"bgm00.ogg", 0.05f);
}

void CStage1_01::Update()
{
	CCameraMgr::Get_Instance()->Update_CameraPos(TILECX * TILEX * 3, TILECY * TILEY);

	CTileMgr::Get_Instance()->Update();
	CObjMgr::Get_Instance()->Update();
}

void CStage1_01::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update();
	CObjMgr::Get_Instance()->Late_Update();

	//CCameraMgr::Get_Instance()->Lock_Camera(TILECX * TILEX, TILECY * TILEY);
}

void CStage1_01::Render(HDC _DC)
{
	// 카메라에 따른 몬스터 좌표의 계산은 몬스터 클래스에서 구현. (Player처럼. Update_Rect 함수 종류 선택에 유의)
	// 카메라에 따른 타일 좌표의 계산은 여기에서 진행.

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_01_BG");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);



	int iOutX = 0;
	int iOutY = 0;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // 최종적으로 렌더시킬 좌표.


	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_01_FRONT");
	FRAME_PROP tBGOriginProp = CSpritePropertyMgr::Get_Instance()->Find_Property(L"STAGE1_01_FRONT");
	
	BitBlt(_DC, iOutX, iOutY, tBGOriginProp.iCX, tBGOriginProp.iCY, hMemDC, 0, 0, SRCCOPY);

	//GdiTransparentBlt(	_DC,
	//					iOutX,
	//					iOutY,
	//					tBGOriginProp.iCX,
	//					tBGOriginProp.iCY,
	//					hMemDC,
	//					0,
	//					0,
	//					tBGOriginProp.iCX,
	//					tBGOriginProp.iCY,
	//					RGB(255, 0, 255));

	CTileMgr::Get_Instance()->Render(_DC);
	CObjMgr::Get_Instance()->Render(_DC);
}

void CStage1_01::Release()
{
}

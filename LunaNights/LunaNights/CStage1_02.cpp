#include "pch.h"
#include "CStage1_02.h"
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

CStage1_02::CStage1_02()
{
}

CStage1_02::~CStage1_02()
{
	Release();
}

void CStage1_02::Initialize()
{
	m_iTileTimesX = 2;
	m_iTileTimesY = 1;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Stage1/1-02_Merge0405.bmp", L"STAGE1_02_FRONT");
	FRAME_PROP tSTAGE1_02_FRONT = { 2720, 816 };							// 타일의 가로세로 길이 정보
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tSTAGE1_02_FRONT, L"STAGE1_02_FRONT");

	CTileMgr::Get_Instance()->Load_Tile(nullptr, nullptr, true,
										L"../Data/Tile_Collision_1-2.dat", L"Collision_Tile");
	CObjMgr::Get_Instance()->Load_Data(L"../Data/Monster_Info_1-2.dat");
}

void CStage1_02::Update()
{
	CTileMgr::Get_Instance()->Update();
	CObjMgr::Get_Instance()->Update();

	CCameraMgr::Get_Instance()->Update_CameraPos(TILECX * TILEX * 2, TILECY * TILEY);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	if (pPlayer->Get_Info()->fX >= 2700)
	{
		pPlayer->Set_Pos(20, pPlayer->Get_Info()->fY);
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_STAGE1_03);
	}
	else if (pPlayer->Get_Info()->fX <= 15)
	{
		pPlayer->Set_Pos(4070, pPlayer->Get_Info()->fY);
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_STAGE1_01);
	}
}

void CStage1_02::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update();
	CObjMgr::Get_Instance()->Late_Update();


}

void CStage1_02::Render(HDC _DC)
{
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_01_BG");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);



	int iOutX = 0;
	int iOutY = 0;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // 최종적으로 렌더시킬 좌표.


	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_02_FRONT");
	FRAME_PROP tBGOriginProp = CSpritePropertyMgr::Get_Instance()->Find_Property(L"STAGE1_02_FRONT");
	
	//BitBlt(_DC, iOutX, iOutY, tBGOriginProp.iCX, tBGOriginProp.iCY, hMemDC, 0, 0, SRCCOPY);

	GdiTransparentBlt(	_DC,
						iOutX,
						iOutY,
						tBGOriginProp.iCX,
						tBGOriginProp.iCY,
						hMemDC,
						0,
						0,
						tBGOriginProp.iCX,
						tBGOriginProp.iCY,
						RGB(255, 0, 255));

	//CTileMgr::Get_Instance()->Render(_DC);
	CObjMgr::Get_Instance()->Render(_DC);
}

void CStage1_02::Release()
{
}

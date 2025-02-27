#include "pch.h"
#include "CStage1_Boss.h"
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
#include "CBoss_HonMeirin.h"

CStage1_Boss::CStage1_Boss()
{
}

CStage1_Boss::~CStage1_Boss()
{
	Release();
}

void CStage1_Boss::Initialize()
{
	m_iTileTimesX = 2;
	m_iTileTimesY = 1;


	// 보스전 임시 확인용.
	// 나중에 완성 시 이 부분들 지우고 SceneMgr 초기 진입 부분도 스테이지 1-01로 바꾸기
	//if (CObjMgr::Get_Instance()->Get_Player() == nullptr)
	//	CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapBG/background_sprite.bmp", L"STAGE1_01_BG");
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapBG/background_sprite_GRAY.bmp", L"STAGE1_01_BG_GRAY");
	//CObjMgr::Get_Instance()->Add_Object(OBJ_UI, CAbstractFactory<CUI>::Create());
	// *****************************************







	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Stage1/1-Boss_Merge.bmp", L"STAGE1_BOSS_FRONT");
	FRAME_PROP tSTAGE1_BOSS_FRONT = { 2720, 816 };							// 타일의 가로세로 길이 정보
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tSTAGE1_BOSS_FRONT, L"STAGE1_BOSS_FRONT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Stage1/1-Boss_Merge_GRAY.bmp", L"STAGE1_BOSS_FRONT_GRAY");
	FRAME_PROP tSTAGE1_BOSS_FRONT_GRAY = { 2720, 816 };							// 타일의 가로세로 길이 정보
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tSTAGE1_BOSS_FRONT_GRAY, L"STAGE1_BOSS_FRONT_GRAY");

	CTileMgr::Get_Instance()->Load_Tile(nullptr, nullptr, true,
		L"../Data/Tile_Collision_1-Boss.dat", L"Collision_Tile");
	CObjMgr::Get_Instance()->Load_Data(L"../Data/Monster_Info_1-Boss.dat");

	// 보스를 Load_Data 보다 앞에 두면 Delete_ID(OBJ_BOSS); 로 인해 삭제되는 문제가 생김
	CObjMgr::Get_Instance()->Add_Object(OBJ_BOSS, CAbstractFactory<CBoss_HonMeirin>::Create(4000, 0)); // 1480, 440

}

void CStage1_Boss::Update()
{
	CTileMgr::Get_Instance()->Update();
	CObjMgr::Get_Instance()->Update();

	CCameraMgr::Get_Instance()->Update_CameraPos(TILECX * TILEX * 2, TILECY * TILEY);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	if (pPlayer->Get_Info()->fX >= 2700)
	{
		//pPlayer->Set_Pos(20, pPlayer->Get_Info()->fY);
		//CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_STAGE1_BOSS);
	}
	else if (pPlayer->Get_Info()->fX <= 20)
	{
		pPlayer->Set_Pos(1335, pPlayer->Get_Info()->fY);
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_STAGE1_03);
	}



	// 보스전 시작 판정


	if (CKeyMgr::Get_Instance()->Key_Down(VK_UP))
	{
		// 메이린과 대화한 적이 없고, 보스전 중이 아니라면
		if (!pPlayer->Get_isBossStart() &&
			pPlayer->Get_Stat(CPlayer::ISTALKED_MEIRIN) == false &&
			!pPlayer->Get_isClearedBoss(1))
		{
			// 대화를 통해 보스전 진입
			if (pPlayer->Get_Info()->fX >= 1200)
			{
				if (pPlayer->Get_MessageWith() != 5)
				{
					pPlayer->Set_MessageWith(5);
					pPlayer->Set_MessagePic(5);

				}
				else
					pPlayer->Set_MessageOrder(pPlayer->Get_MessageOrder() + 1);
			}
		}

		// 메이린과 대화를 했으며(클리어완), 보스전 중이 아니라면
		else if (!pPlayer->Get_isBossStart() &&
			pPlayer->Get_Stat(CPlayer::ISTALKED_MEIRIN) == true &&
			!pPlayer->Get_isClearedBoss(1))
		{
			if (pPlayer->Get_MessageWith() != 5)
			{
				pPlayer->Set_MessageWith(5);
				pPlayer->Set_MessagePic(5);

			}
			else
				pPlayer->Set_MessageOrder(pPlayer->Get_MessageOrder() + 1);

		}


	}
	

}

void CStage1_Boss::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update();
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage1_Boss::Render(HDC _DC)
{
	int iOutX = 0;
	int iOutY = 0;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // 최종적으로 렌더시킬 좌표.
	HDC	hMemDC;
	FRAME_PROP tBGOriginProp;


	if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Stat(CPlayer::TIMEMODE) == 2)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_01_BG_GRAY");
		BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_BOSS_FRONT_GRAY");
		tBGOriginProp = CSpritePropertyMgr::Get_Instance()->Find_Property(L"STAGE1_BOSS_FRONT_GRAY");
	}
	else
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_01_BG");
		BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_BOSS_FRONT");
		tBGOriginProp = CSpritePropertyMgr::Get_Instance()->Find_Property(L"STAGE1_BOSS_FRONT");
	}


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

void CStage1_Boss::Release()
{
}

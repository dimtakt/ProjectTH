#include "pch.h"
#include "CStage1_03.h"
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
#include "CNpc.h"

CStage1_03::CStage1_03()
{
	ZeroMemory(&ptAkyuuPos , sizeof(FPOINT));
	ZeroMemory(&ptNitoriPos, sizeof(FPOINT));
}

CStage1_03::~CStage1_03()
{
	Release();
}

void CStage1_03::Initialize()
{
	m_iTileTimesX = 1;
	m_iTileTimesY = 1;

	// Npc 둘의 위치
	ptAkyuuPos = { 438 + 128 , 544 };
	ptNitoriPos = { 438 + 128 + 250, 544 };


	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Stage1/1-03_Merge.bmp", L"STAGE1_03_FRONT");
	FRAME_PROP tSTAGE1_03_FRONT = { 1360, 816 };							// 타일의 가로세로 길이 정보
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tSTAGE1_03_FRONT, L"STAGE1_03_FRONT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Stage1/1-03_Merge_GRAY.bmp", L"STAGE1_03_FRONT_GRAY");
	FRAME_PROP tSTAGE1_03_FRONT_GRAY = { 1360, 816 };							// 타일의 가로세로 길이 정보
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tSTAGE1_03_FRONT_GRAY, L"STAGE1_03_FRONT_GRAY");

	CTileMgr::Get_Instance()->Load_Tile(nullptr, nullptr, true,
										L"../Data/Tile_Collision_1-3.dat", L"Collision_Tile");
	CObjMgr::Get_Instance()->Load_Data(L"../Data/Monster_Info_1-3.dat");

	CObjMgr::Get_Instance()->Add_Object(OBJ_NPC, CAbstractFactory<CNpc>::Create(ptAkyuuPos.x, ptAkyuuPos.y, 0,  L"STAGEOBJ_AKYUU"));
	CObjMgr::Get_Instance()->Add_Object(OBJ_NPC, CAbstractFactory<CNpc>::Create(ptNitoriPos.x, ptNitoriPos.y, 0,  L"STAGEOBJ_NITORI"));
}

void CStage1_03::Update()
{
	CTileMgr::Get_Instance()->Update();
	CObjMgr::Get_Instance()->Update();

	CCameraMgr::Get_Instance()->Update_CameraPos(TILECX * TILEX * 1, TILECY * TILEY);
	


	// 해당 좌표에 해당할 시 다른 스테이지로 이동

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());

	if (pPlayer->Get_Info()->fX >= 1340)
	{
		pPlayer->Set_Pos(25, pPlayer->Get_Info()->fY);
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_STAGE1_BOSS);
	}
	else if (pPlayer->Get_Info()->fX <= 10)
	{
		pPlayer->Set_Pos(2695, pPlayer->Get_Info()->fY);
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_STAGE1_02);
	}


	// Npc와 근접한 상태에서 플레이어가 윗 방향키를 누르면 이벤트

	// 대화는 다음과 같은 순서로 이루어짐
	// 1. 위치에 맞는 상태에서 윗방향키 누르면, 플레이어의 m_iMessegeWith 가 바뀜
	// 2. CUI 에서 m_iMessageWith 가 0이 아닌 경우를 감지하고 대화창 및 대화내용 출력
	// 3. 윗방향키를 다시 누르면, m_iMessageOrder 가 1씩 올라가며 CUI에서 다음 대화내용 출력
	// 4. 위치를 벗어나거나 대화가 끝나면 m_iMessageWith, m_iMessageOrder 둘 다 0으로 초기화

	if (CKeyMgr::Get_Instance()->Key_Down(VK_UP))
	{
		if (pPlayer->Get_Info()->fX >= ptAkyuuPos.x - 200 &&
			pPlayer->Get_Info()->fX <= ptAkyuuPos.x + 200)
		{
			// 아큐 대화 이벤트
			if (pPlayer->Get_MessageWith() != 3)
			{
				pPlayer->Set_MessagePic(3);
				pPlayer->Set_MessageWith(3);
			}
			else // 다음 대화로 넘기기
				pPlayer->Set_MessageOrder(pPlayer->Get_MessageOrder() + 1);
			
			std::cout << "!!!!!!!!!!!!!!!!!!!!! 아큐 대화!" << std::endl;

		}
		else if (pPlayer->Get_Info()->fX >= ptNitoriPos.x - 200 &&
				pPlayer->Get_Info()->fX <= ptNitoriPos.x + 200)
		{
			// 니토리 대화 이벤트
			if (pPlayer->Get_MessageWith() != 4)
			{
				pPlayer->Set_MessagePic(4);
				pPlayer->Set_MessageWith(4);
			}
			else // 다음 대화로 넘기기
			{
				if (pPlayer->Get_Stat(CPlayer::ISGETWATCH) == false &&
					pPlayer->Get_Stat(CPlayer::GOLD) >= 500 &&
					pPlayer->Get_MessageOrder() == 1)
				{
					pPlayer->Set_Stat_isGetWatch(true);
					pPlayer->Set_Gold(pPlayer->Get_Gold() - 500);
					CSoundMgr::Get_Instance()->PlaySound(L"s1012_item_get_2.wav", SOUND_SKILLGET, 0.2f);
				}
				else
					pPlayer->Set_MessageOrder(pPlayer->Get_MessageOrder() + 1);

			}

			std::cout << "!!!!!!!!!!!!!!!!!!!!! 니토리 대화!" << std::endl;

		}
	}
	else
	{
		if (pPlayer->Get_MessageWith() == 3)
		{
			if (not (pPlayer->Get_Info()->fX >= ptAkyuuPos.x - 200 &&
					pPlayer->Get_Info()->fX <= ptAkyuuPos.x + 200))
			{
				pPlayer->Set_MessageWith(0);
				pPlayer->Set_MessagePic(0);
				pPlayer->Set_MessageOrder(0);
			}
		}
		else if (pPlayer->Get_MessageWith() == 4)
		{
			if (not (pPlayer->Get_Info()->fX >= ptNitoriPos.x - 200 &&
					pPlayer->Get_Info()->fX <= ptNitoriPos.x + 200))
			{
				pPlayer->Set_MessageWith(0);
				pPlayer->Set_MessagePic(0);
				pPlayer->Set_MessageOrder(0);
			}
		}
	}




}

void CStage1_03::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update();
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage1_03::Render(HDC _DC)
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

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_03_FRONT_GRAY");
		tBGOriginProp = CSpritePropertyMgr::Get_Instance()->Find_Property(L"STAGE1_03_FRONT_GRAY");
	}
	else
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_01_BG");
		BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_03_FRONT");
		tBGOriginProp = CSpritePropertyMgr::Get_Instance()->Find_Property(L"STAGE1_03_FRONT");
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

void CStage1_03::Release()
{
}

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
#include "CNpc.h"


CStage1_01::CStage1_01()
{
	ZeroMemory(&ptNitoriPos, sizeof(FPOINT));
}

CStage1_01::~CStage1_01()
{
	Release();
}

void CStage1_01::Initialize()
{
	m_iTileTimesX = 3;
	m_iTileTimesY = 1;

	// 플레이어가 없다면, 생성
	if (CObjMgr::Get_Instance()->Get_Player() == nullptr)
		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());

	// Npc 위치
	ptNitoriPos = { 2350, 544 };

	// 이후 몬스터나 타일 등의 생성 및 이미지 불러오기는 여기에..
	//..
	CObjMgr::Get_Instance()->Add_Object(OBJ_UI, CAbstractFactory<CUI>::Create());
	
	
	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapBG/background_sprite.bmp", L"STAGE1_01_BG");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapBG/background_sprite_GRAY.bmp", L"STAGE1_01_BG_GRAY");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Stage1/1-01_Merge0103.bmp", L"STAGE1_01_FRONT");
	FRAME_PROP tSTAGE1_01_FRONT = { 4080, 816 };							// 타일의 가로세로 길이 정보
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tSTAGE1_01_FRONT, L"STAGE1_01_FRONT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Stage1/1-01_Merge0103_GRAY.bmp", L"STAGE1_01_FRONT_GRAY");
	FRAME_PROP tSTAGE1_01_FRONT_GRAY = { 4080, 816 };							// 타일의 가로세로 길이 정보
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tSTAGE1_01_FRONT, L"STAGE1_01_FRONT_GRAY");


	CTileMgr::Get_Instance()->Initialize();



	// 맵을 하나의 통 텍스쳐를 쓸 것으로 변경함에 따라,
	// 기존 타일 텍스쳐를 로드하지 않고, 콜라이더 정보만을 로드하도록 수정
	CTileMgr::Get_Instance()->Load_Tile(nullptr, nullptr, true,
										L"../Data/Tile_Collision_1-1.dat", L"Collision_Tile");
	
	// 몬스터 위치, 종류 정보 불러오기
	CObjMgr::Get_Instance()->Load_Data(L"../Data/Monster_Info_1-1.dat");
	CObjMgr::Get_Instance()->Update();		// ㅋㅋ;
	
	CObjMgr::Get_Instance()->Add_Object(OBJ_NPC, CAbstractFactory<CNpc>::Create(ptNitoriPos.x, ptNitoriPos.y, 0, L"STAGEOBJ_NITORI"));
}

void CStage1_01::Update()
{
	CTileMgr::Get_Instance()->Update();
	CObjMgr::Get_Instance()->Update();

	CCameraMgr::Get_Instance()->Update_CameraPos(TILECX * TILEX * 3, TILECY * TILEY);
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	if (pPlayer->Get_Info()->fX >= 4070)
	{
		pPlayer->Set_Pos(20, pPlayer->Get_Info()->fY);
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_STAGE1_02);
	}


	if (CKeyMgr::Get_Instance()->Key_Down(VK_UP))
	{
		// ** 니토리

		if (pPlayer->Get_Info()->fX >= ptNitoriPos.x - 200 &&
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
					pPlayer->Set_MessageOrder(0);
				}
				else
					pPlayer->Set_MessageOrder(pPlayer->Get_MessageOrder() + 1);

			}

			std::cout << "!!!!!!!!!!!!!!!!!!!!! 니토리 대화!" << std::endl;

		}

		// ** 도움말 1
		
		//if (pPlayer->Get_Info()->fX >= 768 &&
		//	pPlayer->Get_Info()->fX <= 832)
		//{
		//	if (pPlayer->Get_MessageWith() != 1)
		//	{
		//		pPlayer->Set_MessagePic(1);
		//		pPlayer->Set_MessageWith(1);
		//	}
		//	else
		//	{

		//	}


		//}
	}
	else
	{
		if (pPlayer->Get_MessageWith() == 4)
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


	int iOutX = 0;
	int iOutY = 0;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // 최종적으로 렌더시킬 좌표.
	HDC	hMemDC;
	FRAME_PROP tBGOriginProp;
	

	if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Stat(CPlayer::TIMEMODE) == 2)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_01_BG_GRAY");
		BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_01_FRONT_GRAY");
		tBGOriginProp = CSpritePropertyMgr::Get_Instance()->Find_Property(L"STAGE1_01_FRONT_GRAY");
	}
	else
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_01_BG");
		BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_01_FRONT");
		tBGOriginProp = CSpritePropertyMgr::Get_Instance()->Find_Property(L"STAGE1_01_FRONT");
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

void CStage1_01::Release()
{
}

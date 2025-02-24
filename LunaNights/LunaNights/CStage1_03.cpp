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

	// Npc ���� ��ġ
	ptAkyuuPos = { 438 + 128 , 544 };
	ptNitoriPos = { 438 + 128 + 250, 544 };


	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Stage1/1-03_Merge.bmp", L"STAGE1_03_FRONT");
	FRAME_PROP tSTAGE1_03_FRONT = { 1360, 816 };							// Ÿ���� ���μ��� ���� ����
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tSTAGE1_03_FRONT, L"STAGE1_03_FRONT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Stage1/1-03_Merge_GRAY.bmp", L"STAGE1_03_FRONT_GRAY");
	FRAME_PROP tSTAGE1_03_FRONT_GRAY = { 1360, 816 };							// Ÿ���� ���μ��� ���� ����
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
	


	// �ش� ��ǥ�� �ش��� �� �ٸ� ���������� �̵�

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


	// Npc�� ������ ���¿��� �÷��̾ �� ����Ű�� ������ �̺�Ʈ

	// ��ȭ�� ������ ���� ������ �̷����
	// 1. ��ġ�� �´� ���¿��� ������Ű ������, �÷��̾��� m_iMessegeWith �� �ٲ�
	// 2. CUI ���� m_iMessageWith �� 0�� �ƴ� ��츦 �����ϰ� ��ȭâ �� ��ȭ���� ���
	// 3. ������Ű�� �ٽ� ������, m_iMessageOrder �� 1�� �ö󰡸� CUI���� ���� ��ȭ���� ���
	// 4. ��ġ�� ����ų� ��ȭ�� ������ m_iMessageWith, m_iMessageOrder �� �� 0���� �ʱ�ȭ

	if (CKeyMgr::Get_Instance()->Key_Down(VK_UP))
	{
		if (pPlayer->Get_Info()->fX >= ptAkyuuPos.x - 200 &&
			pPlayer->Get_Info()->fX <= ptAkyuuPos.x + 200)
		{
			// ��ť ��ȭ �̺�Ʈ
			if (pPlayer->Get_MessageWith() != 3)
			{
				pPlayer->Set_MessagePic(3);
				pPlayer->Set_MessageWith(3);
			}
			else // ���� ��ȭ�� �ѱ��
				pPlayer->Set_MessageOrder(pPlayer->Get_MessageOrder() + 1);
			
			std::cout << "!!!!!!!!!!!!!!!!!!!!! ��ť ��ȭ!" << std::endl;

		}
		else if (pPlayer->Get_Info()->fX >= ptNitoriPos.x - 200 &&
				pPlayer->Get_Info()->fX <= ptNitoriPos.x + 200)
		{
			// ���丮 ��ȭ �̺�Ʈ
			if (pPlayer->Get_MessageWith() != 4)
			{
				pPlayer->Set_MessagePic(4);
				pPlayer->Set_MessageWith(4);
			}
			else // ���� ��ȭ�� �ѱ��
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

			std::cout << "!!!!!!!!!!!!!!!!!!!!! ���丮 ��ȭ!" << std::endl;

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
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // ���������� ������ų ��ǥ.
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

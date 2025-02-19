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

	// �÷��̾ ���ٸ�, ����
	if (CObjMgr::Get_Instance()->Get_Player() == nullptr)
		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());


	// ���� ���ͳ� Ÿ�� ���� ���� �� �̹��� �ҷ������ ���⿡..
	//..
	CObjMgr::Get_Instance()->Add_Object(OBJ_UI, CAbstractFactory<CUI>::Create());
	
	
	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapBG/background_sprite.bmp", L"STAGE1_01_BG");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Stage1/1-01_Merge0103.bmp", L"STAGE1_01_FRONT");
	FRAME_PROP tSTAGE1_01_FRONT = { 4080, 816 };							// Ÿ���� ���μ��� ���� ����
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tSTAGE1_01_FRONT, L"STAGE1_01_FRONT");


	CTileMgr::Get_Instance()->Initialize();



	// ���� �ϳ��� �� �ؽ��ĸ� �� ������ �����Կ� ����,
	// ���� Ÿ�� �ؽ��ĸ� �ε����� �ʰ�, �ݶ��̴� �������� �ε��ϵ��� ����
	CTileMgr::Get_Instance()->Load_Tile(nullptr, nullptr, true,
										L"../Data/Tile_Collision_1-1.dat", L"Collision_Tile");
	
	// ���� ��ġ, ���� ���� �ҷ�����
	CObjMgr::Get_Instance()->Load_Data(L"../Data/Monster_Info_1-1.dat");
	CObjMgr::Get_Instance()->Update();		// ����;
	
	
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
}

void CStage1_01::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update();
	CObjMgr::Get_Instance()->Late_Update();



	//CCameraMgr::Get_Instance()->Lock_Camera(TILECX * TILEX, TILECY * TILEY);
}

void CStage1_01::Render(HDC _DC)
{
	// ī�޶� ���� ���� ��ǥ�� ����� ���� Ŭ�������� ����. (Playeró��. Update_Rect �Լ� ���� ���ÿ� ����)
	// ī�޶� ���� Ÿ�� ��ǥ�� ����� ���⿡�� ����.

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_01_BG");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);



	int iOutX = 0;
	int iOutY = 0;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // ���������� ������ų ��ǥ.


	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_01_FRONT");
	FRAME_PROP tBGOriginProp = CSpritePropertyMgr::Get_Instance()->Find_Property(L"STAGE1_01_FRONT");
	
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

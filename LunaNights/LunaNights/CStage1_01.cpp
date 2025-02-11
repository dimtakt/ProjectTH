#include "pch.h"
#include "CStage1_01.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CPlayer.h"
#include "CTileMgr.h"
#include "CSpritePropertyMgr.h"


#include "CCameraMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"


CStage1_01::CStage1_01()
{
}

CStage1_01::~CStage1_01()
{
	Release();
}

void CStage1_01::Initialize()
{
	// �÷��̾� ����
	CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());

	// ���� ���ͳ� Ÿ�� ���� ���� �� �̹��� �ҷ������ ���⿡..
	//..
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/_Temp_Image/TempBG.bmp", L"STAGE1_01_BG");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Front.bmp", L"BG_Front");
	FRAME_PROP tBG_Front = { TILECX, TILECY, 28, 29, 800 };							// Ÿ���� ���μ��� ���� ����
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tBG_Front, L"BG_Front");
	
	CObjMgr::Get_Instance()->Update();		// ����;

	CTileMgr::Get_Instance()->Initialize();
	CTileMgr::Get_Instance()->Load_Tile(L"../Data/Tile.dat", L"BG_Front");
	// CTileMgr::Get_Instance()->Load_Tile(L"../Data/Tile_Collision.dat", L"Collision_Tile");	// �ݶ��̴� ������
}

void CStage1_01::Update()
{
	CObjMgr::Get_Instance()->Update();
}

void CStage1_01::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage1_01::Render(HDC _DC)
{
	// ī�޶� ���� ���� ��ǥ�� ����� ���� Ŭ�������� ����. (Playeró��. Update_Rect �Լ� ���� ���ÿ� ����)
	// ī�޶� ���� Ÿ�� ��ǥ�� ����� ���⿡�� ����.

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_01_BG");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CTileMgr::Get_Instance()->Render(_DC);
	CObjMgr::Get_Instance()->Render(_DC);
}

void CStage1_01::Release()
{
}

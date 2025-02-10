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
	std::cout << "[INFO] Init Edit Scene.." << std::endl;
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/_Temp_Image/TempBG.bmp", L"STAGE1_01_BG");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/MapTiles/BG_Front.bmp", L"BG_Front");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/_Temp_Image/Tile.bmp", L"TILE");		// �׽�Ʈ�� �ӽ� Ÿ��
	FRAME_PROP tTILE = { TILECX, TILECY, 2, 1, 2 };							// Ÿ���� ���μ��� ���� ����
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tTILE, L"TILE");	// Ÿ���� ���μ��� ���� ������ �̱��濡 ����

	CTileMgr::Get_Instance()->Initialize();

	std::cout << "[INFO] Init Completed." << std::endl;
}

void CEdit::Update()
{
	CTileMgr::Get_Instance()->Update();

	Key_Input();
}

void CEdit::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update();
}

void CEdit::Render(HDC _DC)
{
	CTileMgr::Get_Instance()->Render(_DC);
}

void CEdit::Release()
{
}

void CEdit::Key_Input()
{
	float fCameraMoveSpeed = 5.f;

	float fCameraX, fCameraY;
	// ���� ī�޶� ���ߴ� ��ǥ�� ������ ����� ã�ƾ� ��.
	CCameraMgr::Get_Instance()->Get_CameraPos(fCameraX, fCameraY);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		fCameraX -= fCameraMoveSpeed;

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		fCameraX += fCameraMoveSpeed;

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		fCameraY -= fCameraMoveSpeed;

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		fCameraY += fCameraMoveSpeed;

	CCameraMgr::Get_Instance()->Set_Pos(fCameraX, fCameraY);


	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		POINT	ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		float fTmpX, fTmpY;
		CCameraMgr::Get_Instance()->Get_CameraPos(fTmpX, fTmpY);
		fTmpX = fTmpX - (WINCX / 2);
		fTmpY = fTmpY - (WINCY / 2);

		POINT	ptCalcedMouse{ ptMouse.x + fTmpX, ptMouse.y + fTmpY };

		//ptMouse.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
		//ptMouse.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();


		// ��Ŭ�� �� 0�� ������ ��ǥ��, 1�� ������ Ÿ�� �ε�����, 2�� ������ �Ӽ����� ����
		// ���� ��ǥ ������ �̻���. �����ؾ� ��
		CTileMgr::Get_Instance()->Picking_Tile(ptCalcedMouse, 1, 0);
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_PRIOR))	// PageUp
	{
		CTileMgr::Get_Instance()->Save_Tile();
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_NEXT))		// PageDown
	{
		CTileMgr::Get_Instance()->Load_Tile();
	}

}

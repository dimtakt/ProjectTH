#include "pch.h"
#include "CStage1_01.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CPlayer.h"


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

	CObjMgr::Get_Instance()->Render(_DC);
}

void CStage1_01::Release()
{
}

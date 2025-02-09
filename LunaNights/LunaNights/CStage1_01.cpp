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
	// 플레이어 생성
	CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());

	// 이후 몬스터나 타일 등의 생성 및 이미지 불러오기는 여기에..
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
	// 카메라에 따른 몬스터 좌표의 계산은 몬스터 클래스에서 구현. (Player처럼. Update_Rect 함수 종류 선택에 유의)
	// 카메라에 따른 타일 좌표의 계산은 여기에서 진행.

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"STAGE1_01_BG");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CObjMgr::Get_Instance()->Render(_DC);
}

void CStage1_01::Release()
{
}

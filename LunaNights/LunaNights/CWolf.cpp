#include "pch.h"
#include "CWolf.h"

#include "CBmpMgr.h"
#include "CSpritePropertyMgr.h"
#include "CPlayer.h"
#include "CObjMgr.h"

CWolf::CWolf()
{
}

CWolf::~CWolf()
{
}

void CWolf::Initialize()
{
	m_fSpeed = 2.f;
	m_isFlying = true;

	m_iHp = 20;
	m_iMp = 5;
	m_fAtk = 15.f;


	m_tFrame.iFrameCur = 0;

	m_tFrame.dwSpeed = 100;				// 프레임 전환 속도
	m_tFrame.dwTime = GetTickCount();	// 모션이 바뀌었을 때 흐른 시간을 저장
	m_eRender = RENDER_GAMEOBJECT;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Monsters/wolf_jump_sprite/wolf_jump_sprite.bmp", L"Wolf_Jump");				// 64_64_X5Y5_21
	FRAME_PROP tWolf_Jump = { 96 * 2, 64 * 2, 1, 3, 3, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tWolf_Jump, L"Wolf_Jump");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Monsters/wolf_jump_sprite/wolf_jump_sprite_R.bmp", L"Wolf_Jump_R");				// 64_64_X5Y5_21
	FRAME_PROP tWolf_Jump_R = { 96 * 2, 64 * 2, 1, 3, 3, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tWolf_Jump_R, L"Wolf_Jump_R");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Monsters/wolf_run_sprite/wolf_run_sprite.bmp", L"Wolf_Run");				// 64_64_X5Y5_21
	FRAME_PROP tWolf_Run = { 64 * 2, 64 * 2, 1, 4, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tWolf_Run, L"Wolf_Run");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Monsters/wolf_run_sprite/wolf_run_sprite_R.bmp", L"Wolf_Run_R");				// 64_64_X5Y5_21
	FRAME_PROP tWolf_Run_R = { 64 * 2, 64 * 2, 1, 4, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tWolf_Run_R, L"Wolf_Run_R");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Monsters/wolf_stand_sprite/wolf_stand_sprite.bmp", L"Wolf_Stand");				// 64_64_X5Y5_21
	FRAME_PROP tWolf_Stand = { 64 * 2, 64 * 2, 1, 4, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tWolf_Stand, L"Wolf_Stand");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Monsters/wolf_stand_sprite/wolf_stand_sprite_R.bmp", L"Wolf_Stand_R");				// 64_64_X5Y5_21
	FRAME_PROP tWolf_Stand_R = { 64 * 2, 64 * 2, 1, 4, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tWolf_Stand_R, L"Wolf_Stand_R");
}

int CWolf::Update()
{



	return 0;
}

void CWolf::Late_Update()
{
}

void CWolf::Render(HDC hDC)
{
}

void CWolf::Release()
{
}

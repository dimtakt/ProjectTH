#include "pch.h"
#include "CWisp.h"
#include "CBmpMgr.h"
#include "CSpritePropertyMgr.h"

CWisp::CWisp()
{
}

CWisp::~CWisp()
{
}

void CWisp::Initialize()
{
	m_fSpeed = 2.f;
	m_isFlying = true;

	m_iHp = 20;
	m_iMp = 5;
	m_fAtk = 15.f;



	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Monsters/will_o_wisp_sprite/will_o_wisp_sprite.bmp", L"Wisp");				// 64_64_X5Y5_21
	FRAME_PROP tWisp = { 32 * 2, 32 * 2, 4, 2, 8 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tWisp, L"Wisp");
	
	m_pFrameKey = L"Wisp";
}

int CWisp::Update()
{
	if (m_isDead)
		return OBJ_DEAD;

	m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(m_pFrameKey);
	Set_FrameProperty(m_tFramePropCur);	// 현재 프레임에 현 상태에 맞는 정보를 반영
	Set_Scale(m_tFramePropCur.iCX, m_tFramePropCur.iCY);

	m_tCollideInfo = m_tInfo;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CWisp::Late_Update()
{
}

void CWisp::Render(HDC hDC)
{
}

void CWisp::Release()
{
}

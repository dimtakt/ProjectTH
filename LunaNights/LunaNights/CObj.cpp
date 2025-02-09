#include "pch.h"
#include "CObj.h"
#include "CSpritePropertyMgr.h"

CObj::CObj() :
	m_eState(OBJST_END),
	m_eRender(RENDER_END),
	m_pTarget(nullptr),
	m_fSpeed(0.f),
	m_isDead(false),
	m_fAngle(0.f),
	m_isStretch(false),
	m_pFrameKey(nullptr)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CObj::~CObj()
{
}

void CObj::Update_Rect()
{
	m_tRect.left    = static_cast<LONG>(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top		= static_cast<LONG>(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right   = static_cast<LONG>(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom  = static_cast<LONG>(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}
void CObj::Update_Rect2X()
{
	m_tRect.left    = static_cast<LONG>(m_tInfo.fX - (m_tInfo.fCX * 1.0f));
	m_tRect.top		= static_cast<LONG>(m_tInfo.fY - (m_tInfo.fCY * 1.0f));
	m_tRect.right   = static_cast<LONG>(m_tInfo.fX + (m_tInfo.fCX * 1.0f));
	m_tRect.bottom  = static_cast<LONG>(m_tInfo.fY + (m_tInfo.fCY * 1.0f));
}

void CObj::Update_Rect_UpStand()
{
	m_tRect.left	= static_cast<LONG>(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top		= static_cast<LONG>(m_tInfo.fY - (m_tInfo.fCY * 1.0f));
	m_tRect.right	= static_cast<LONG>(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom	= static_cast<LONG>(m_tInfo.fY + (m_tInfo.fCY * 0.0f));
}
void CObj::Update_Rect_UpStand2X()
{
	m_tRect.left	= static_cast<LONG>(m_tInfo.fX - (m_tInfo.fCX * 1.0f));
	m_tRect.top		= static_cast<LONG>(m_tInfo.fY - (m_tInfo.fCY * 2.0f));
	m_tRect.right	= static_cast<LONG>(m_tInfo.fX + (m_tInfo.fCX * 1.0f));
	m_tRect.bottom	= static_cast<LONG>(m_tInfo.fY + (m_tInfo.fCY * 0.0f));
}

void CObj::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iFrameCur;

		if (m_tFrame.iFrameCur >= m_tFrame.iFrameAmount)
		{
			m_tFrame.iFrameCur = CSpritePropertyMgr::Get_Instance()->Find_Property(m_pFrameKey).iFrameStart;
		}

		m_tFrame.dwTime = GetTickCount();

	}
}

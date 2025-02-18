#include "pch.h"
#include "CObj.h"
#include "CSpritePropertyMgr.h"

CObj::CObj() :
	m_eCurState(OBJST_END),
	m_eRender(RENDER_END),
	m_pTarget(nullptr),
	m_fSpeed(0.f),
	m_isDead(false),
	m_fAngle(0.f),
	m_isStretch(false),
	m_pFrameKey(nullptr),
	m_isJumping(false),
	m_isFlying(false),
	m_fVelocityX(0),
	m_fVelocityY(0),
	m_isGod(false),
	m_isPreGod(false),
	m_iHp(0),
	m_iMp(0),
	m_fAtk(0.f),
	m_iMaxHp(0),
	m_iMaxMp(0),
	m_iGold(0)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tCollideInfo, sizeof(RECT));
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

void CObj::Move_Frame(FRAME& _tFrame)
{
	if (_tFrame.dwTime + _tFrame.dwSpeed < GetTickCount())
	{
		++_tFrame.iFrameCur;

		if (_tFrame.iFrameCur >= _tFrame.iFrameAmount)
		{
			_tFrame.iFrameCur = CSpritePropertyMgr::Get_Instance()->Find_Property(m_pFrameKey).iFrameStart;
		}

		_tFrame.dwTime = GetTickCount();

	}
}


void CObj::RotatePoints(float centerX, float centerY, POINT points[3], float angle)
{
	float radian = angle * DEG2RAD;  // 각도를 라디안으로 변환
	float cosA = cos(radian);
	float sinA = sin(radian);

	for (int i = 0; i < 3; ++i)
	{
		float x = static_cast<float>(points[i].x);
		float y = static_cast<float>(points[i].y);

		// 회전 변환 공식 적용
		points[i].x = static_cast<LONG>(centerX + (x - centerX) * cosA - (y - centerY) * sinA);
		points[i].y = static_cast<LONG>(centerY + (x - centerX) * sinA + (y - centerY) * cosA);
	}
}
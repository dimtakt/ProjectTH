#include "pch.h"
#include "CCollideRect.h"
#include "CSceneMgr.h"

CCollideRect::CCollideRect()
{
}

CCollideRect::~CCollideRect()
{
	Release();
}

void CCollideRect::Initialize()
{
	m_eRender = RENDER_TILE_BACKGROUND;
}

int CCollideRect::Update()
{
	return 0;
}

void CCollideRect::Late_Update()
{
}

void CCollideRect::Render(HDC hDC)
{
	// Edit 창에서만 콜라이더를 시각적으로 보여줌
	if (CSceneMgr::Get_Instance()->Get_CurScene() == CSceneMgr::SC_EDIT)
	{
		Rectangle(hDC, m_tInfo.fX - m_tInfo.fCX / 2, m_tInfo.fY - m_tInfo.fCY / 2,
				m_tInfo.fX + m_tInfo.fCX / 2, m_tInfo.fY + m_tInfo.fCY / 2);
	}
}

void CCollideRect::Release()
{
}

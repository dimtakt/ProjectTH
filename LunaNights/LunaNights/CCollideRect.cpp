#include "pch.h"
#include "CCollideRect.h"
#include "CSceneMgr.h"
#include "CCameraMgr.h"

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

	//if (CSceneMgr::Get_Instance()->Get_CurScene() == CSceneMgr::SC_EDIT)
	{
		int iOutX = m_tRect.left;
		int iOutY = m_tRect.top;
		CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // 최종적으로 렌더시킬 좌표.
	
		Rectangle(hDC,	m_tInfo.fX - m_tInfo.fCX / 2 + iOutX, m_tInfo.fY - m_tInfo.fCY / 2 + iOutY,
						m_tInfo.fX + m_tInfo.fCX / 2 + iOutX, m_tInfo.fY + m_tInfo.fCY / 2 + iOutY);
	}
}

void CCollideRect::Release()
{
}

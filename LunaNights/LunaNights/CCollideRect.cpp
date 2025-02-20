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
	// Edit â������ �ݶ��̴��� �ð������� ������

	//if (CSceneMgr::Get_Instance()->Get_CurScene() == CSceneMgr::SC_EDIT)
	{
		int iOutX = m_tRect.left;
		int iOutY = m_tRect.top;
		CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // ���������� ������ų ��ǥ.
	
		Rectangle(hDC,	m_tInfo.fX - m_tInfo.fCX / 2 + iOutX, m_tInfo.fY - m_tInfo.fCY / 2 + iOutY,
						m_tInfo.fX + m_tInfo.fCX / 2 + iOutX, m_tInfo.fY + m_tInfo.fCY / 2 + iOutY);
	}
}

void CCollideRect::Release()
{
}

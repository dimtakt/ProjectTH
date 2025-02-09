#include "pch.h"
#include "CTile.h"

#include "CBmpMgr.h"
#include "CCameraMgr.h"
#include "CSpritePropertyMgr.h"

CTile::CTile() : m_iDrawID(0), m_iOption(0)
{
}

CTile::~CTile()
{
	Release();
}

void CTile::Initialize()
{
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;

	m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(L"TILE");	// Ÿ���� ������ �޾ƿ�
	Set_FrameProperty(m_tFramePropCur);	// m_tFrame �� ��� �޾ƿ� ������ �ݿ�
}

int CTile::Update()
{

	__super::Update_Rect2X();

	return 0;
}

void CTile::Late_Update()
{
}

void CTile::Render(HDC hDC)
{
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"TILE");

	int iOutX = m_tRect.left;
	int iOutY = m_tRect.top;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // ���������� ������ų ��ǥ.

	// https://blog.naver.com/tipsware/220994341959
	
	//BitBlt(		hDC,					// �纻 DC
	//			iOutX,					// �׸����� ���� X ��ǥ
	//			iOutY,					// �׸����� ���� Y ��ǥ
	//			TILECX, TILECY,			// �׸����� ���� X, Y Size
	//			hMemDC,					// ���� DC
	//			m_iDrawID * TILECX,		// ������ �̹����� X ��ǥ
	//			0,						// ������ �̹����� Y ��ǥ
	//			SRCCOPY);

	GdiTransparentBlt(	hDC,
						iOutX,
						iOutY,
						TILECX * 2, TILECY * 2,
						hMemDC,
						TILECX * ((m_iDrawID) % (m_tFrame.iFrameMaxX)),
						TILECY * ((m_iDrawID) / (m_tFrame.iFrameMaxX)),
						TILECX,
						TILECY,
						RGB(255, 0, 255));

	std::cout << "[INFO][CTile::Render] Tile Generateed on.. : " << iOutX << ", " << iOutY << " (Size : " << TILECX * 2 << ", " << TILECY * 2 << ")" << std::endl;

		
}

void CTile::Release()
{
}

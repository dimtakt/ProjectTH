#include "pch.h"
#include "CTile.h"

#include "CBmpMgr.h"
#include "CCameraMgr.h"
#include "CSpritePropertyMgr.h"
#include "CSceneMgr.h"

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

	m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(L"BG_Front");	// Ÿ���� ������ �޾ƿ�
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
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"BG_Front");

	int iOutX = m_tRect.left;
	int iOutY = m_tRect.top;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // ���������� ������ų ��ǥ.

	// https://blog.naver.com/tipsware/220994341959
	
	//BitBlt(		hDC,					// �纻 DC
	//			iOutX,					// �׸����� ���� X ��ǥ
	//			iOutY,					// �׸����� ���� Y ��ǥ
	//			TILECX * 2, TILECY * 2,	// �׸����� ���� X, Y Size
	//			hMemDC,					// ���� DC
	//			TILECX * ((m_iDrawID) % (m_tFrame.iFrameMaxX)),		// ������ �̹����� X ��ǥ
	//			TILECY * ((m_iDrawID) / (m_tFrame.iFrameMaxX)),		// ������ �̹����� Y ��ǥ
	//			SRCCOPY);


	if (CSceneMgr::Get_Instance()->Get_CurScene() != CSceneMgr::SC_EDIT)
	{
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
	}
	else
	{
		StretchBlt(			hDC,					// �纻 DC
							iOutX,					// �׸����� ���� X ��ǥ
							iOutY,					// �׸����� ���� Y ��ǥ
							TILECX * 2, TILECY * 2,	// �׸����� ���� X, Y Size
							hMemDC,					// ���� DC
							TILECX * ((m_iDrawID) % (m_tFrame.iFrameMaxX)),		// ������ �̹����� X ��ǥ
							TILECY * ((m_iDrawID) / (m_tFrame.iFrameMaxX)),		// ������ �̹����� Y ��ǥ
							TILECX, TILECY,
							SRCCOPY);
	}


	// �׽�Ʈ��, ���� ���� ���� StretchBlt�� ��ȯ
	//StretchBlt(	GetDC(g_hWnd),					// �纻 DC
	//			iOutX,					// �׸����� ���� X ��ǥ
	//			iOutY,					// �׸����� ���� Y ��ǥ
	//			TILECX * 2, TILECY * 2,	// �׸����� ���� X, Y Size
	//			hMemDC,					// ���� DC
	//			TILECX * ((m_iDrawID) % (m_tFrame.iFrameMaxX)),		// ������ �̹����� X ��ǥ
	//			TILECY * ((m_iDrawID) / (m_tFrame.iFrameMaxX)),		// ������ �̹����� Y ��ǥ
	//			TILECX, TILECY,
	//			SRCCOPY);

	//std::cout << "[INFO][CTile::Render] Tile Generated on.. : " << iOutX << ", " << iOutY << " (Size : " << TILECX * 2 << ", " << TILECY * 2 << ")" << std::endl;

		
}

void CTile::Release()
{
}

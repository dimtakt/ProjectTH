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

	m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(L"BG_Front");	// 타일의 정보를 받아옴
	Set_FrameProperty(m_tFramePropCur);	// m_tFrame 에 방금 받아온 정보를 반영
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
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // 최종적으로 렌더시킬 좌표.

	// https://blog.naver.com/tipsware/220994341959
	
	//BitBlt(		hDC,					// 사본 DC
	//			iOutX,					// 그리려는 곳의 X 좌표
	//			iOutY,					// 그리려는 곳의 Y 좌표
	//			TILECX * 2, TILECY * 2,	// 그리려는 곳의 X, Y Size
	//			hMemDC,					// 원본 DC
	//			TILECX * ((m_iDrawID) % (m_tFrame.iFrameMaxX)),		// 가져올 이미지의 X 좌표
	//			TILECY * ((m_iDrawID) / (m_tFrame.iFrameMaxX)),		// 가져올 이미지의 Y 좌표
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
		StretchBlt(			hDC,					// 사본 DC
							iOutX,					// 그리려는 곳의 X 좌표
							iOutY,					// 그리려는 곳의 Y 좌표
							TILECX * 2, TILECY * 2,	// 그리려는 곳의 X, Y Size
							hMemDC,					// 원본 DC
							TILECX * ((m_iDrawID) % (m_tFrame.iFrameMaxX)),		// 가져올 이미지의 X 좌표
							TILECY * ((m_iDrawID) / (m_tFrame.iFrameMaxX)),		// 가져올 이미지의 Y 좌표
							TILECX, TILECY,
							SRCCOPY);
	}


	// 테스트용, 수정 이후 위의 StretchBlt와 전환
	//StretchBlt(	GetDC(g_hWnd),					// 사본 DC
	//			iOutX,					// 그리려는 곳의 X 좌표
	//			iOutY,					// 그리려는 곳의 Y 좌표
	//			TILECX * 2, TILECY * 2,	// 그리려는 곳의 X, Y Size
	//			hMemDC,					// 원본 DC
	//			TILECX * ((m_iDrawID) % (m_tFrame.iFrameMaxX)),		// 가져올 이미지의 X 좌표
	//			TILECY * ((m_iDrawID) / (m_tFrame.iFrameMaxX)),		// 가져올 이미지의 Y 좌표
	//			TILECX, TILECY,
	//			SRCCOPY);

	//std::cout << "[INFO][CTile::Render] Tile Generated on.. : " << iOutX << ", " << iOutY << " (Size : " << TILECX * 2 << ", " << TILECY * 2 << ")" << std::endl;

		
}

void CTile::Release()
{
}

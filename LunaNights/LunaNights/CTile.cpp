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
	m_tPropertyName = L"BG_Front";
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
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_tPropertyName);

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


	// 이거를 받아온 프로퍼티에 맞게 수정해야 함
	// 그 뒤, CTIleMgr의 Render 부분에서 주석처리한 부분을 해제해보기. (프로퍼티프레임 지정하는 부분)
	// 해제했을 때에도 좌표가 이상하면 프로퍼티프레임을 지정하는 함수 호출을 어느 타이밍에 해야 할 지 생각해본 뒤 반영
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

void CTile::Set_PropName(const TCHAR* _propertyName)
{
	m_tPropertyName = _propertyName;
	m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(m_tPropertyName);
}
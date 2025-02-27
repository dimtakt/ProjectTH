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
	m_tInfo.fCY = TILECY;
	m_tInfo.fCX = TILECX;
	m_tPropertyName = L"BG_Front";
	m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(L"BG_Front");	// 타일의 정보를 받아옴
	Set_FrameProperty(m_tFramePropCur);	// m_tFrame 에 방금 받아온 정보를 반영
}


int CTile::Update()
{
	__super::Update_Rect();
	//__super::Update_Rect();

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
	

	// 이거를 받아온 프로퍼티에 맞게 수정해야 함
	// 그 뒤, CTIleMgr의 Render 부분에서 주석처리한 부분을 해제해보기. (프로퍼티프레임 지정하는 부분)
	// 해제했을 때에도 좌표가 이상하면 프로퍼티프레임을 지정하는 함수 호출을 어느 타이밍에 해야 할 지 생각해본 뒤 반영
	if (CSceneMgr::Get_Instance()->Get_CurScene() != CSceneMgr::SC_EDIT)
	{
		FRAME_PROP tOriginProp = CSpritePropertyMgr::Get_Instance()->Find_Property(m_tPropertyName);
		GdiTransparentBlt(	hDC,
							iOutX,
							iOutY,
							TILECX,
							TILECY,
							hMemDC,
							tOriginProp.iCX * ((m_iDrawID) % (m_tFrame.iFrameMaxX)),
							//WINCX * ((iSelectedTileIndex) % (tCollision_Tile.iFrameMaxX)),
							tOriginProp.iCY * ((m_iDrawID) / (m_tFrame.iFrameMaxX)),
							tOriginProp.iCX,
							tOriginProp.iCY,
							RGB(255, 0, 255));
	}
	else
	{
		FRAME_PROP tOriginProp = CSpritePropertyMgr::Get_Instance()->Find_Property(m_tPropertyName);
		StretchBlt(			hDC,					// 사본 DC
							iOutX,					// 그리려는 곳의 X 좌표
							iOutY,					// 그리려는 곳의 Y 좌표
							TILECX, TILECY,			// 그리려는 곳의 X, Y Size
							hMemDC,					// 원본 DC
							tOriginProp.iCX * ((m_iDrawID) % (m_tFrame.iFrameMaxX)),		// 가져올 이미지의 X 좌표
							tOriginProp.iCY * ((m_iDrawID) / (m_tFrame.iFrameMaxX)),		// 가져올 이미지의 Y 좌표
							tOriginProp.iCX ,
							tOriginProp.iCY ,
							SRCCOPY);
	}



	//std::cout << "[INFO][CTile::Render] Tile Generated on.. : " << iOutX << ", " << iOutY << " (Size : " << TILECX << ", " << TILECY << ")" << std::endl;
	//std::cout << "[INFO][CTile::Render] Generated TIle Info : " << m_tInfo.fX << ", " << m_tInfo.fY << " (Size : " << m_tInfo.fCX << ", " << m_tInfo.fCY << ")" << std::endl;
}

void CTile::Release()
{
}

void CTile::Set_PropName(const TCHAR* _propertyName)
{
	m_tPropertyName = _propertyName;
	m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(m_tPropertyName);
}
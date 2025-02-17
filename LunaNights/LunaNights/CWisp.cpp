#include "pch.h"
#include "CWisp.h"
#include "CBmpMgr.h"
#include "CSpritePropertyMgr.h"
#include "CCameraMgr.h"

CWisp::CWisp()
{
}

CWisp::~CWisp()
{
}

void CWisp::Initialize()
{
	m_fSpeed = 2.f;
	m_isFlying = true;

	m_iHp = 20;
	m_iMp = 5;
	m_fAtk = 15.f;


	m_tFrame.iFrameCur = 0;

	m_tFrame.dwSpeed = 100;				// 프레임 전환 속도
	m_tFrame.dwTime = GetTickCount();	// 모션이 바뀌었을 때 흐른 시간을 저장


	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Monsters/will_o_wisp_sprite/will_o_wisp_sprite.bmp", L"Wisp");				// 64_64_X5Y5_21
	FRAME_PROP tWisp = { 32 * 2, 32 * 2, 4, 2, 8 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tWisp, L"Wisp");
	
	m_pFrameKey = L"Wisp";
}

int CWisp::Update()
{
	if (m_isDead)
		return OBJ_DEAD;

	// 이미지 파일 크기에 맞게 크기 구성
	m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(m_pFrameKey);
	Set_FrameProperty(m_tFramePropCur);	// 현재 프레임에 현 상태에 맞는 정보를 반영
	Set_Scale(m_tFramePropCur.iCX, m_tFramePropCur.iCY);


	// 이미지 파일 크기에 맞게 충돌판정 구성
	m_tCollideInfo = m_tInfo;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CWisp::Late_Update()
{

}

void CWisp::Render(HDC hDC)
{
	int iOutX = m_tRect.left;
	int iOutY = m_tRect.top;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // 최종적으로 렌더시킬 좌표.

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(	hDC,					// 최종적으로 그릴 DC
						iOutX,					// 복사받을 위치 X, Y좌표
						iOutY,
						//iOutX - m_tInfo.fCX/2,					// 복사받을 위치 X, Y좌표
						//iOutY - m_tInfo.fCY,
						m_tInfo.fCX,	// 복사 받을 가로, 세로 길이.
						m_tInfo.fCY,
						hMemDC,					// 비트맵을 가지고 있는 DC
						m_tInfo.fCX * ((m_tFrame.iFrameCur) % (m_tFrame.iFrameMaxX)),	// 출력하려는 스트라이트 이미지 내에서의 좌표
						m_tInfo.fCY * ((m_tFrame.iFrameCur) / (m_tFrame.iFrameMaxX)),
						m_tInfo.fCX,	// 비트맵을 출력할 가로, 세로 길이
						m_tInfo.fCY,
						RGB(255, 0, 255));		// 제거할 색상

}

void CWisp::Release()
{
}

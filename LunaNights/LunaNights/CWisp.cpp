#include "pch.h"
#include "CWisp.h"
#include "CBmpMgr.h"
#include "CSpritePropertyMgr.h"
#include "CCameraMgr.h"
#include "CPlayer.h"
#include "CObjMgr.h"

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

	m_iHp = 10;
	m_iMp = 5;
	m_fAtk = 15.f;



	m_tFrame.iFrameCur = 0;

	m_tFrame.dwSpeed = 50;				// 프레임 전환 속도
	m_tFrame.dwTime = GetTickCount();	// 모션이 바뀌었을 때 흐른 시간을 저장
	m_eRender = RENDER_GAMEOBJECT;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Monsters/will_o_wisp_sprite/will_o_wisp_sprite.bmp", L"Wisp");				// 64_64_X5Y5_21
	FRAME_PROP tWisp = { 32 * 2, 32 * 2, 4, 2, 8 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tWisp, L"Wisp");
	
	Set_FrameProperty(tWisp);

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
	m_tCollideInfo = { m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX / 2, m_tInfo.fCY / 2 };


	__super::Update_Rect();


	// 플레이어 위치를 받아와, 충분히 가까워지면 플레이어를 추적. (화면 안에 있을 때만)

	int iOutX = 0, iOutY = 0;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY);

	if (!((m_tInfo.fX + m_tInfo.fCX < 0 - iOutX || m_tInfo.fX - iOutY - m_tInfo.fCX > WINCX - iOutX) ||
		(m_tInfo.fY + m_tInfo.fCY < 0 - iOutY || m_tInfo.fY - iOutY - m_tInfo.fCY > WINCY - iOutY)))
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
		if (pPlayer == nullptr)
			return OBJ_DEAD;

		INFO tPlayerInfo = *pPlayer->Get_Info();
		tPlayerInfo.fY -= (tPlayerInfo.fCY / 2);

		float fDirX = tPlayerInfo.fX - m_tInfo.fX;
		float fDirY = tPlayerInfo.fY - m_tInfo.fY;

		float fAngle = atan2(fDirY, fDirX);
		float fMoveX = cos(fAngle);
		float fMoveY = sin(fAngle);

		m_tInfo.fX += fMoveX * m_fSpeed;
		m_tInfo.fY += fMoveY * m_fSpeed;
	}





	return OBJ_NOEVENT;
}

void CWisp::Late_Update()
{
	Move_Frame();
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

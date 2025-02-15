#include "pch.h"
#include "CPlayerBullet.h"
#include "CBmpMgr.h"
#include "CSpritePropertyMgr.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"
#include "CPlayer.h"

CPlayerBullet::CPlayerBullet()
{
}

CPlayerBullet::~CPlayerBullet()
{
	Release();
}

void CPlayerBullet::Initialize()
{
	pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	Set_Scale(64, 64);

	// ImageLoad
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/PlayerBullet/bullet_sprite/bullet_sprite.bmp", L"Player_Bullet");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/PlayerBullet/bullet_sprite/bullet_sprite_R.bmp", L"Player_Bullet_R");
	FRAME_PROP tPlayer_Bullet = { 64, 64, 1, 1, 1 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_Bullet, L"Player_Bullet");
		

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/PlayerBullet/bullet_sprite/bullet_Reset.bmp", L"Player_Bullet_Reset");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/PlayerBullet/bullet_sprite/bullet_Plg.bmp", L"Player_Bullet_Plg");

	m_isStretch = pPlayer->Get_Stretch();
	m_eRender = RENDER_GAMEOBJECT;
	m_fSpeed = 16.f;
}

int CPlayerBullet::Update()
{
	if (m_isDead)
		return OBJ_DEAD;

	m_tInfo.fX += m_fSpeed * cosf(m_fAngle * DEG2RAD);
	m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * DEG2RAD);

	m_tCollideInfo = { m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, 20 };


	// Info 정보에 따라 Rect 값 갱신
	__super::Update_Rect();
	m_tPoint[0] = { m_tRect.left, m_tRect.top };
	m_tPoint[1] = { m_tRect.right, m_tRect.top };
	m_tPoint[2] = { m_tRect.left, m_tRect.bottom };



	return OBJ_NOEVENT;
}

void CPlayerBullet::Late_Update()
{
}

void CPlayerBullet::Render(HDC hDC)
{
	// laterEdit
	int iOutX = m_tRect.left;
	int iOutY = m_tRect.top;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // 최종적으로 렌더시킬 좌표.

	HDC	hMemDC;
	POINT tmpPoint[3] = { m_tPoint[0], m_tPoint[1], m_tPoint[2]};
	INFO tPlayerInfo = *pPlayer->Get_Info();

	if (m_isStretch)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Player_Bullet_R");
		RotatePoints(tPlayerInfo.fX, tPlayerInfo.fY - tPlayerInfo.fCY / 2, tmpPoint, 30);
	}
	else
	{ 
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Player_Bullet");
		RotatePoints(tPlayerInfo.fX, tPlayerInfo.fY - tPlayerInfo.fCY / 2, tmpPoint, 30);
	}
	HDC	hPlgDC = CBmpMgr::Get_Instance()->Find_Image(L"Player_Bullet_Plg");
	HDC	hResetDC = CBmpMgr::Get_Instance()->Find_Image(L"Player_Bullet_Reset");

	GdiTransparentBlt(
			hDC,					// 대상 DC
			(int)iOutX,				// 출력할 X 좌표
			(int)iOutY,				// 출력할 Y 좌표
			(int)m_tInfo.fCX,		// 출력할 너비
			(int)m_tInfo.fCY,		// 출력할 높이
			hMemDC,					// 원본 DC (hPlgDC의 내용을 복사)
			0, 0,					// 원본 DC에서 복사할 시작 좌표
			(int)m_tInfo.fCX,		// 원본에서 복사할 너비
			(int)m_tInfo.fCY,		// 원본에서 복사할 높이
			RGB(255, 0, 255)		// 투명하게 만들 색상
	);





	// 아래 이건 왜 안되는지 모르겠음.

	//PlgBlt(		hPlgDC,					// 대상 DC
	//			tmpPoint,				// 위치 변환이 적용된 POINT[3]
	//			hMemDC,					// 원본 DC
	//			0, 0,					// 원본 비트맵의 좌표
	//			(int)m_tInfo.fCX,		// 원본에서 복사할 너비
	//			(int)m_tInfo.fCY,		// 원본에서 복사할 높이
	//			NULL, NULL, NULL);		// 마스크들 (선택)
	//GdiTransparentBlt(
	//			hDC,					// 대상 DC
	//			(int)iOutX,				// 출력할 X 좌표
	//			(int)iOutY,				// 출력할 Y 좌표
	//			(int)m_tInfo.fCX,		// 출력할 너비
	//			(int)m_tInfo.fCY,		// 출력할 높이
	//			hPlgDC,					// 원본 DC (hPlgDC의 내용을 복사)
	//			0, 0,					// 원본 DC에서 복사할 시작 좌표
	//			(int)m_tInfo.fCX,		// 원본에서 복사할 너비
	//			(int)m_tInfo.fCY,		// 원본에서 복사할 높이
	//			RGB(255, 0, 255)		// 투명하게 만들 색상
	//);
	//BitBlt(hPlgDC, 0, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hResetDC, 0, 0, SRCCOPY);


}


void CPlayerBullet::Release()
{
}

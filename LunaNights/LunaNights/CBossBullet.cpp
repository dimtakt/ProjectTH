#include "pch.h"
#include "CBossBullet.h"
#include "CBmpMgr.h"
#include "CSpritePropertyMgr.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"
#include "CBoss_HonMeirin.h"

CBossBullet::CBossBullet() : pBoss(nullptr), dwStartFrame(0)
{
	ZeroMemory(&tFrameBullet, sizeof(FRAME));
	ZeroMemory(&tFrameBullet45, sizeof(FRAME));
	ZeroMemory(&tFrameBulletBig, sizeof(FRAME));
}

CBossBullet::~CBossBullet()
{
	Release();
}

void CBossBullet::Initialize()
{
	pBoss = dynamic_cast<CBoss_HonMeirin*>(CObjMgr::Get_Instance()->Get_Boss());
	if (!pBoss)
		return;

	Set_Scale(64, 64);

	// ImageLoad
	
	// ����1 ����ü
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirinBullet/honmeirin_bullet/honmeirin_bullet.bmp", L"Meirin_Bullet");
	FRAME_PROP tMeirin_Bullet = { 64 * 2, 64 * 2, 4, 2, 8 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Bullet, L"Meirin_Bullet");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirinBullet/honmeirin_bullet/honmeirin_bullet_R.bmp", L"Meirin_Bullet_R");
	FRAME_PROP tMeirin_Bullet_R = { 64 * 2, 64 * 2, 4, 2, 8 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Bullet_R, L"Meirin_Bullet_R");

	// ����2 ����ü
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirinBullet/honmeirin_bullet45/honmeirin_bullet45.bmp", L"Meirin_Bullet45");
	FRAME_PROP tMeirin_Bullet45 = { 64 * 2, 64 * 2, 4, 2, 8 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Bullet45, L"Meirin_Bullet45");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirinBullet/honmeirin_bullet45/honmeirin_bullet45_R.bmp", L"Meirin_Bullet45_R");
	FRAME_PROP tMeirin_Bullet45_R = { 64 * 2, 64 * 2, 4, 2, 8 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Bullet45_R, L"Meirin_Bullet45_R");


	// ����4 ����ü
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirinBullet/honmeirin_bullet_big/honmeirin_bullet_big.bmp", L"Meirin_Bullet_Big");
	FRAME_PROP tMeirin_Bullet_Big = { 96 * 2, 96 * 2, 4, 2, 8 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Bullet_Big, L"Meirin_Bullet_Big");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirinBullet/honmeirin_bullet_big/honmeirin_bullet_big_R.bmp", L"Meirin_Bullet_Big_R");
	FRAME_PROP tMeirin_Bullet_Big_R = { 96 * 2, 96 * 2, 4, 2, 8 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Bullet_Big_R, L"Meirin_Bullet_Big_R");
	
	

	m_isStretch = pBoss->Get_Stretch();
	m_eRender = RENDER_GAMEOBJECT;
	m_fSpeed = 8.f;

	m_tFrame.dwSpeed = 40;

}

int CBossBullet::Update()
{
	if (m_isDead)
		return OBJ_DEAD;

	dwStartFrame++;

	m_tInfo.fX += m_fSpeed * cosf(m_fAngle * DEG2RAD);
	m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * DEG2RAD);

	m_tCollideInfo = { m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, 20 };

	// ���� ������Ű ���ڿ��� �ش��ϴ� ������ ������Ƽ�� ��� ������ m_tFrame �� �ݿ�
	FRAME_PROP tProp = CSpritePropertyMgr::Get_Instance()->Find_Property(m_pFrameKey);
	Set_FrameProperty(tProp);


	Move_Frame();
	__super::Update_Rect();

	
	if (m_fSpeed == 8.f &&
		(m_pFrameKey == L"Meirin_Bullet_Big" || m_pFrameKey == L"Meirin_Bullet_Big_R"))
		m_fSpeed = 30.f;

	

	// �Ѿ��� �浹 ���� ũ��� �������� ����

	if (m_pFrameKey == L"Meirin_Bullet" ||
		m_pFrameKey == L"Meirin_Bullet_R" ||
		m_pFrameKey == L"Meirin_Bullet45" ||
		m_pFrameKey == L"Meirin_Bullet45_R")
	{
		m_fAtk = 25;
		m_tCollideInfo = { m_tInfo.fX + tProp.iCX / 4.f, m_tInfo.fY + tProp.iCY / 4.f, tProp.iCX / 2.f, tProp.iCY / 2.f };
	}
	else if (	m_pFrameKey == L"Meirin_Bullet_Big" ||
				m_pFrameKey == L"Meirin_Bullet_Big_R")
	{
		m_fAtk = 50;
		m_tCollideInfo = { m_tInfo.fX + tProp.iCX / 4.f, m_tInfo.fY + tProp.iCY / 4.f, tProp.iCX / 2.f, tProp.iCY / 2.3f };
	}

	return OBJ_NOEVENT;

}

void CBossBullet::Late_Update()
{
	if (dwStartFrame >= 500)
		m_isDead = true;
}

void CBossBullet::Render(HDC hDC)
{
	int iOutX = m_tRect.left;
	int iOutY = m_tRect.top;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // ���������� ������ų ��ǥ.

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);


	GdiTransparentBlt(
						hDC,					// ��� DC
						(int)iOutX,				// ����� X ��ǥ
						(int)iOutY,				// ����� Y ��ǥ
						(int)m_tFrame.iCX,		// ����� �ʺ�
						(int)m_tFrame.iCY,		// ����� ����
						hMemDC,					// ���� DC (hPlgDC�� ������ ����)
						m_tFrame.iCX * ((m_tFrame.iFrameCur) % (m_tFrame.iFrameMaxX)),	// ���� DC���� ������ ���� ��ǥ
						m_tFrame.iCY * ((m_tFrame.iFrameCur) / (m_tFrame.iFrameMaxX)),
						m_tFrame.iCX,			// �������� ������ �ʺ�
						m_tFrame.iCY,			// �������� ������ ����
						RGB(255, 0, 255)		// �����ϰ� ���� ����
	);



	// �׽�Ʈ��

	//int iTmpX = 0;
	//int iTmpY = 0;
	//CCameraMgr::Get_Instance()->Get_RenderPos(iTmpX, iTmpY); // ���������� ������ų ��ǥ.
	//
	//FRAME_PROP tProp = CSpritePropertyMgr::Get_Instance()->Find_Property(m_pFrameKey);
	//Set_FrameProperty(tProp);
	//
	//HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	//HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	//
	//MoveToEx(hDC, m_tCollideInfo.fX - m_tCollideInfo.fCX / 2 + iTmpX, m_tCollideInfo.fY - m_tCollideInfo.fCY / 2 + iTmpY, nullptr);
	//LineTo	(hDC, m_tCollideInfo.fX - m_tCollideInfo.fCX / 2 + iTmpX, m_tCollideInfo.fY + m_tCollideInfo.fCY / 2 + iTmpY);
	//LineTo	(hDC, m_tCollideInfo.fX + m_tCollideInfo.fCX / 2 + iTmpX, m_tCollideInfo.fY + m_tCollideInfo.fCY / 2 + iTmpY);
	//LineTo	(hDC, m_tCollideInfo.fX + m_tCollideInfo.fCX / 2 + iTmpX, m_tCollideInfo.fY - m_tCollideInfo.fCY / 2 + iTmpY);
	//LineTo	(hDC, m_tCollideInfo.fX - m_tCollideInfo.fCX / 2 + iTmpX, m_tCollideInfo.fY - m_tCollideInfo.fCY / 2 + iTmpY);
	//
	//SelectObject(hDC, hOldPen);
	//DeleteObject(hPen);


}

void CBossBullet::Release()
{
}

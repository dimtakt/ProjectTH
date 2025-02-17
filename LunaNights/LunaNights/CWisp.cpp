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

	m_tFrame.dwSpeed = 50;				// ������ ��ȯ �ӵ�
	m_tFrame.dwTime = GetTickCount();	// ����� �ٲ���� �� �帥 �ð��� ����
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

	// �̹��� ���� ũ�⿡ �°� ũ�� ����
	m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(m_pFrameKey);
	Set_FrameProperty(m_tFramePropCur);	// ���� �����ӿ� �� ���¿� �´� ������ �ݿ�
	Set_Scale(m_tFramePropCur.iCX, m_tFramePropCur.iCY);


	// �̹��� ���� ũ�⿡ �°� �浹���� ����
	m_tCollideInfo = { m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX / 2, m_tInfo.fCY / 2 };


	__super::Update_Rect();


	// �÷��̾� ��ġ�� �޾ƿ�, ����� ��������� �÷��̾ ����. (ȭ�� �ȿ� ���� ����)

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
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // ���������� ������ų ��ǥ.

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(	hDC,					// ���������� �׸� DC
						iOutX,					// ������� ��ġ X, Y��ǥ
						iOutY,
						//iOutX - m_tInfo.fCX/2,					// ������� ��ġ X, Y��ǥ
						//iOutY - m_tInfo.fCY,
						m_tInfo.fCX,	// ���� ���� ����, ���� ����.
						m_tInfo.fCY,
						hMemDC,					// ��Ʈ���� ������ �ִ� DC
						m_tInfo.fCX * ((m_tFrame.iFrameCur) % (m_tFrame.iFrameMaxX)),	// ����Ϸ��� ��Ʈ����Ʈ �̹��� �������� ��ǥ
						m_tInfo.fCY * ((m_tFrame.iFrameCur) / (m_tFrame.iFrameMaxX)),
						m_tInfo.fCX,	// ��Ʈ���� ����� ����, ���� ����
						m_tInfo.fCY,
						RGB(255, 0, 255));		// ������ ����

}

void CWisp::Release()
{
}

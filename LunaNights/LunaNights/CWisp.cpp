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

	m_tFrame.dwSpeed = 100;				// ������ ��ȯ �ӵ�
	m_tFrame.dwTime = GetTickCount();	// ����� �ٲ���� �� �帥 �ð��� ����


	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Monsters/will_o_wisp_sprite/will_o_wisp_sprite.bmp", L"Wisp");				// 64_64_X5Y5_21
	FRAME_PROP tWisp = { 32 * 2, 32 * 2, 4, 2, 8 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tWisp, L"Wisp");
	
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

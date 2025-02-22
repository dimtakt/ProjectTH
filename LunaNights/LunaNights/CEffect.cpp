#include "pch.h"
#include "CEffect.h"

#include "CBmpMgr.h"
#include "CSpritePropertyMgr.h"
#include "CSceneMgr.h"
#include "CPlayer.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"


CEffect::CEffect() :
	isLoop(false),
    iLoopTimes(0)
{
	//ZeroMemory(&, sizeof(FRAME));
}

CEffect::~CEffect()
{
	Release();
}

void CEffect::Initialize()
{
	// LoadImages
	// ����Ʈ ��������Ʈ�� ���⿡�� �ε�
	// FRAME_PROP �� ���� ������ �����Ͽ� ����� ��
	
	// �ı� ���� ���� �̺�Ʈ
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Effects/bomb_effect/bomb_effect.bmp", L"Effect_Bomb");
	FRAME_PROP tEffect_Bomb = { 46 * 2, 46 * 2, 4, 3, 12 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tEffect_Bomb, L"Effect_Bomb");
	// �ǰ� ����Ʈ
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Effects/damage_effect/damage_effect.bmp", L"Effect_Damage");
	FRAME_PROP tEffect_Damage = { 180 * 2, 180 * 2, 4, 2, 8 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tEffect_Damage, L"Effect_Damage");
	// MP �Ҹ� ����Ʈ
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Effects/mp_down_sprite/mp_down_sprite.bmp", L"Effect_MPDown");
	FRAME_PROP tEffect_MPDown = { 128 * 2, 17 * 2, 1, 6, 6 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tEffect_MPDown, L"Effect_MPDown");
	// TP �Ҹ� ����Ʈ
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Effects/time_down_sprite/time_down_sprite.bmp", L"Effect_TPDown");
	FRAME_PROP tEffect_TPDown = { 80 * 2, 80 * 2, 6, 1, 6 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tEffect_TPDown, L"Effect_TPDown");

	



	Set_Pos(WINCX / 2, WINCY / 2);

	m_eRender = RENDER_EFFECT;

}

int CEffect::Update()
{
	if (m_isDead)
		return OBJ_DEAD;


	if (m_pFrameKey == L"Effect_TPDown" ||
		m_pFrameKey == L"Effect_MPDown")
		m_eRender = RENDER_UI_EFFECT;



	CSceneMgr::SCENEID tCurScene = CSceneMgr::Get_Instance()->Get_CurScene();

	// �������������� ���̵��� ��
	if (tCurScene == CSceneMgr::SCENEID::SC_LOGO ||
		tCurScene == CSceneMgr::SCENEID::SC_END ||
		tCurScene == CSceneMgr::SCENEID::SC_EDIT)
		return OBJ_NOEVENT;


	m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(m_pFrameKey);
	Set_FrameProperty(m_tFramePropCur);	// ���� �����ӿ� �� ���¿� �´� ������ �ݿ�
	Set_Scale(m_tFrame.iCX, m_tFrame.iCY);
	
	m_tFrame.dwSpeed = 40.f; // ������ ��ȯ �ӵ�








	__super::Update_Rect();


	return OBJ_NOEVENT;
}

void CEffect::Late_Update()
{
	// ����Ʈ ����� ������ �Ǹ� ����

	if (m_tFrame.iFrameCur == m_tFrame.iFrameAmount - 1)
		m_isDead = true;

	Move_Frame();

}

void CEffect::Render(HDC hDC)
{
	CSceneMgr::SCENEID tCurScene = CSceneMgr::Get_Instance()->Get_CurScene();

	// �������������� ���̵��� ��
	if (tCurScene == CSceneMgr::SCENEID::SC_LOGO ||
		tCurScene == CSceneMgr::SCENEID::SC_END ||
		tCurScene == CSceneMgr::SCENEID::SC_EDIT)
		return;


	// TransparentBlt �� �̿��Ͽ� ����Ʈ ���
	// ��ġ�� ���� ��� CObjMgr �� ���� �޾ƿͼ� ����� m_tInfo �� ������� �ؾ��� ��

	if (isWorldFix)
	{
		int iOutX = m_tRect.left;
		int iOutY = m_tRect.top;
		CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // ���������� ������ų ��ǥ.

		HDC	hMemDC;
		FRAME_PROP tCurProp = {};
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

		// m_tFrame ���� Set_FrameProperty �� �������� FRAME_PROP ���� ��

		GdiTransparentBlt(	hDC,					// ���������� �׸� DC
		 					iOutX,					// ������� ��ġ X, Y��ǥ
		 					iOutY,
							m_tFrame.iCX,	// ���� ���� ����, ���� ����.
							m_tFrame.iCY,
		 					hMemDC,					// ��Ʈ���� ������ �ִ� DC
							m_tFrame.iCX * ((m_tFrame.iFrameCur) % (m_tFrame.iFrameMaxX)),	// ����Ϸ��� ��Ʈ����Ʈ �̹��� �������� ��ǥ
							m_tFrame.iCY * ((m_tFrame.iFrameCur) / (m_tFrame.iFrameMaxX)),
							m_tFrame.iCX,	// ��Ʈ���� ����� ����, ���� ����
							m_tFrame.iCY,
		 					RGB(255, 0, 255));		// ������ ����
	}
	else
	{
		HDC	hMemDC;
		FRAME_PROP tCurProp = {};
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

		// m_tFrame ���� Set_FrameProperty �� �������� FRAME_PROP ���� ��

		GdiTransparentBlt(	hDC,					// ���������� �׸� DC
							m_tInfo.fX,					// ������� ��ġ X, Y��ǥ
							m_tInfo.fY,
							m_tFrame.iCX,	// ���� ���� ����, ���� ����.
							m_tFrame.iCY,
							hMemDC,					// ��Ʈ���� ������ �ִ� DC
							m_tFrame.iCX * ((m_tFrame.iFrameCur) % (m_tFrame.iFrameMaxX)),	// ����Ϸ��� ��Ʈ����Ʈ �̹��� �������� ��ǥ
							m_tFrame.iCY * ((m_tFrame.iFrameCur) / (m_tFrame.iFrameMaxX)),
							m_tFrame.iCX,	// ��Ʈ���� ����� ����, ���� ����
							m_tFrame.iCY,
							RGB(255, 0, 255));		// ������ ����
	}


}

void CEffect::Release()
{
}

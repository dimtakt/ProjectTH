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
	fLoopTimes(0),
	iDamageType(0),
    iDamageAmount(0),
	dwStartTime(0)
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
	// ��ų ��� ����Ʈ
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Effects/ring_effect_sprite/ring_effect_sprite.bmp", L"Effect_SkillUse");
	FRAME_PROP tEffect_SkillUse = { 192 * 2, 192 * 2, 4, 4, 16 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tEffect_SkillUse, L"Effect_SkillUse");

	

	// ���� ��Ʈ
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/UI/number_status_font_merged/number_status_font_merged.bmp", L"Effect_Number");
	FRAME_PROP tEffect_Number = { 7 * 2, 8 * 2, 10, 9, 86 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tEffect_Number, L"Effect_Number");
	// �������ͽ�
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/UI/status_vision_sprite/status_vision_sprite.bmp", L"Effect_Status");
	FRAME_PROP tEffect_Status = { 81 * 2, 8 * 2, 1, 14, 14 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tEffect_Status, L"Effect_Status");




	dwStartTime = GetTickCount();

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
	

	if (m_pFrameKey == L"Effect_SkillUse")
		m_tFrame.dwSpeed = 20.f;
	else
		m_tFrame.dwSpeed = 40.f; // ������ ��ȯ �ӵ�








	__super::Update_Rect();


	return OBJ_NOEVENT;
}

void CEffect::Late_Update()
{
	// ����Ʈ ����� ������ �Ǹ� ����

	if (m_pFrameKey == L"Effect_Number" ||
		m_pFrameKey == L"Effect_Status")
	{
		if (dwStartTime + fLoopTimes * 1000 < GetTickCount())
			m_isDead = true;
	}
	else
	{
		if (m_tFrame.iFrameCur == m_tFrame.iFrameAmount - 1)
			m_isDead = true;
		
		Move_Frame();
	}

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

	if (m_pFrameKey == L"Effect_Number")
	{
		// �޾ƿ� ���������� ����Ͽ� �������� ������ ���� ���

		int iOutX = m_tRect.left;
		int iOutY = m_tRect.top;
		CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // ���������� ������ų ��ǥ.

		HDC	hMemDC;
		FRAME_PROP tCurProp = {};
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

		int iDmgCharNum = log10(iDamageAmount) + 1;	// �ڸ���
		int iFirstCharX = 605 + (m_tFrame.iCX / 2) * (3 - iDmgCharNum);	// �ڸ����� ���� ��ġ ����


		for (int i = 0; i < iDmgCharNum; i++)
		{
			int iDmgChar = (iDamageAmount % (int)pow(10, i + 1)) / (pow(10, i));

			GdiTransparentBlt(	hDC,					// ���������� �׸� DC
		 						iOutX + (iDmgCharNum * 0.5 - i + 0.5) * m_tFrame.iCX,					// ������� ��ġ X, Y��ǥ
		 						iOutY,
								m_tFrame.iCX,	// ���� ���� ����, ���� ����.
								m_tFrame.iCY,
		 						hMemDC,					// ��Ʈ���� ������ �ִ� DC
								m_tFrame.iCX * ((iDmgChar) % (m_tFrame.iFrameMaxX)),	// ����Ϸ��� ��Ʈ����Ʈ �̹��� �������� ��ǥ
								m_tFrame.iCY * iDamageType,
								m_tFrame.iCX,	// ��Ʈ���� ����� ����, ���� ����
								m_tFrame.iCY,
		 						RGB(255, 0, 255));		// ������ ����
		}
		return;	// ���� ����Ʈ���ٸ� ����
	}

	else if (m_pFrameKey == L"Effect_Status")
	{
		// �޾ƿ� �Ӽ��� ����Ͽ� �������� �������ͽ� ���� ��ȯ ���

		int iOutX = m_tRect.left;
		int iOutY = m_tRect.top;
		CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // ���������� ������ų ��ǥ.

		HDC	hMemDC;
		FRAME_PROP tCurProp = {};
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

		GdiTransparentBlt(	hDC,					// ���������� �׸� DC
							iOutX,					// ������� ��ġ X, Y��ǥ
							iOutY,
							m_tFrame.iCX,	// ���� ���� ����, ���� ����.
							m_tFrame.iCY,
							hMemDC,					// ��Ʈ���� ������ �ִ� DC
							0,	// ����Ϸ��� ��Ʈ����Ʈ �̹��� �������� ��ǥ
							m_tFrame.iCY * (2 * iStatusType + GetTickCount() % 2),
							m_tFrame.iCX,	// ��Ʈ���� ����� ����, ���� ����
							m_tFrame.iCY,
							RGB(255, 0, 255));		// ������ ����
		return;	// ���� ����Ʈ���ٸ� ����
	}


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

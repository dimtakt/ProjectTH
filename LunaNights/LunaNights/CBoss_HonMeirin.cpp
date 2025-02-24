#include "pch.h"
#include "CBoss_HonMeirin.h"
#include "CBmpMgr.h"
#include "CSpritePropertyMgr.h"
#include "CSceneMgr.h"
#include "CTileCollisionMgr.h"
#include "CCameraMgr.h"
#include "CPlayer.h"
#include "CObjMgr.h"

CBoss_HonMeirin::CBoss_HonMeirin():
	m_ePreState(OBJST_END),
	m_isReady(false),
	m_iPattern(0)
{
	ZeroMemory(&m_tPrePos, sizeof(FPOINT));
}

CBoss_HonMeirin::~CBoss_HonMeirin()
{
	Release();
}

void CBoss_HonMeirin::Initialize()
{
	Set_Scale(128, 128);

	m_eCurState = OBJST_IDLE;
	m_fSpeed = 3.f;
	m_isFlying = false;

	m_iHp = 1000;
	m_iMp = 5;
	m_iGold = 2000;

	m_fAtk = 25.f;


	m_tFrame.iFrameCur = 0;

	m_tFrame.dwSpeed = 100;				// ������ ��ȯ �ӵ�
	m_tFrame.dwTime = GetTickCount();	// ����� �ٲ���� �� �帥 �ð��� ����
	m_eRender = RENDER_GAMEOBJECT;

	LoadImages();


	FRAME_PROP tMeirin_Stand_R = CSpritePropertyMgr::Get_Instance()->Find_Property(L"Meirin_Stand_R");
	Set_FrameProperty(tMeirin_Stand_R);
	m_pFrameKey = L"Meirin_Stand_R";
}

int CBoss_HonMeirin::Update()
{
	if (m_isDead)
		return OBJ_DEAD;

	// �÷��̾��� ��ġ�� �޾ƿ�
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	if (pPlayer == nullptr)
		return OBJ_NOEVENT;
	INFO tPlayerInfo = *pPlayer->Get_Info();


	// ���� ����

	// ���� ���� ����
	// 0. [�غ�] ��ȭ ���� �� �غ� 
	
	
	
	// 1. ���
	// 2. ����1 (�����غ� - ����(����ü 1))
	// 3. ���
	// 4. ����2 (���� - �ڸ���� - ����(45�� ����ü*6) - ���� 
	// 5. ���
	// 6. ����3 (���� - ����(��������))
	// 7. ���
	// 8. ����3
	// 9. ���
	// 10. ����2 (�����̶�� ++, �ƴ϶�� 1��)
	// 11. [����] ����4 (��¡ - ����(���� ����ü))


	__super::Update_Rect_UpStand();

	return 0;
}

void CBoss_HonMeirin::Late_Update()
{
	Jump();

	Move_Frame();
	Motion_Change();
}

void CBoss_HonMeirin::Render(HDC hDC)
{
	int iOutX = m_tRect.left;
	int iOutY = m_tRect.top;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // ���������� ������ų ��ǥ.

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(	hDC,					// ���������� �׸� DC
						iOutX,					// ������� ��ġ X, Y��ǥ
						iOutY,
						m_tInfo.fCX,	// ���� ���� ����, ���� ����.
						m_tInfo.fCY,
						hMemDC,					// ��Ʈ���� ������ �ִ� DC
						m_tInfo.fCX * ((m_tFrame.iFrameCur) % (m_tFrame.iFrameMaxX)),	// ����Ϸ��� ��Ʈ����Ʈ �̹��� �������� ��ǥ
						m_tInfo.fCY * ((m_tFrame.iFrameCur) / (m_tFrame.iFrameMaxX)),
						m_tInfo.fCX,	// ��Ʈ���� ����� ����, ���� ����
						m_tInfo.fCY,
						RGB(255, 0, 255));		// ������ ����

}

void CBoss_HonMeirin::Release()
{
}


void CBoss_HonMeirin::LoadImages()
{
	// ���� ���� �� : ��ȭ �� ���ĵ� ���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_stand/honmeirin_stand.bmp", L"Meirin_Stand");
	FRAME_PROP tMeirin_Stand = { 64 * 2, 64 * 2, 4, 4, 16 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Stand, L"Meirin_Stand");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_stand/honmeirin_stand_R.bmp", L"Meirin_Stand_R");
	FRAME_PROP tMeirin_Stand_R = { 64 * 2, 64 * 2, 4, 4, 16 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Stand_R, L"Meirin_Stand_R");




	// ���� ���� : �ٸ� ���� ���������� ���� ��, ������ ���� ���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_gurd/honmeirin_gurd.bmp", L"Meirin_Guard");
	FRAME_PROP tMeirin_Guard = { 64 * 2, 64 * 2, 4, 1, 1 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Guard, L"Meirin_Guard");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_gurd/honmeirin_gurd_R.bmp", L"Meirin_Guard_R");
	FRAME_PROP tMeirin_Guard_R = { 64 * 2, 64 * 2, 4, 1, 1 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Guard_R, L"Meirin_Guard_R");

	// ���� ���� : �ٸ� ���� ���������� ���� �� �÷��̾�� ���� �Ÿ��� �����ϵ��� �̵� ���� ���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/onmeirin_run/honmeirin_run.bmp", L"Meirin_Run");
	FRAME_PROP tMeirin_Run = { 64 * 2, 80 * 2, 4, 3, 12 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Run, L"Meirin_Run");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/onmeirin_run/honmeirin_run_R.bmp", L"Meirin_Run_R");
	FRAME_PROP tMeirin_Run_R = { 64 * 2, 80 * 2, 4, 3, 12 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Run_R, L"Meirin_Run_R");




	// ���� 1 : ����ü �߻� ���� ���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_attack_tame/honmeirin_attack_tame.bmp", L"Meirin_Attack_Ready");
	FRAME_PROP tMeirin_Attack_Ready = { 128 * 2, 80 * 2, 1, 3, 3 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Attack_Ready, L"Meirin_Attack_Ready");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_attack_tame/honmeirin_attack_tame_R.bmp", L"Meirin_Attack_Ready_R");
	FRAME_PROP tMeirin_Attack_Ready_R = { 128 * 2, 80 * 2, 1, 3, 3 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Attack_Ready_R, L"Meirin_Attack_Ready_R");

	// ���� 1 : ����ü �߻� �� ���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_attack/honmeirin_attack.bmp", L"Meirin_Attack");
	FRAME_PROP tMeirin_Attack = { 128 * 2, 80 * 2, 1, 6, 6 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Attack, L"Meirin_Attack");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_attack/honmeirin_attack_R.bmp", L"Meirin_Attack_R");
	FRAME_PROP tMeirin_Attack_R = { 128 * 2, 80 * 2, 1, 6, 6 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Attack_R, L"Meirin_Attack_R");




	// ���� 2 : ���� �� ���� ���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_jump/honmeirin_jump.bmp", L"Meirin_Jump");
	FRAME_PROP tMeirin_Jump = { 64 * 2, 96 * 2, 1, 2, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Jump, L"Meirin_Jump");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_jump/honmeirin_jump.bmp", L"Meirin_Jump_R");
	FRAME_PROP tMeirin_Jump_R = { 64 * 2, 96 * 2, 1, 2, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Jump_R, L"Meirin_Jump_R");

	// ���� 2 : ���� �� �̵� ���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_dash/honmeirin_dash.bmp", L"Meirin_Dash");
	FRAME_PROP tMeirin_Dash = { 80 * 2, 64 * 2, 1, 2, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Dash, L"Meirin_Dash");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_dash/honmeirin_dash_R.bmp", L"Meirin_Dash_R");
	FRAME_PROP tMeirin_Dash_R = { 80 * 2, 64 * 2, 1, 2, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Dash_R, L"Meirin_Dash_R");

	// ���� 2 : �ϴ� 45���� ����ü�� �߻��ϴ� ���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_tenma/honmeirin_tenma.bmp", L"Meirin_Tenma");
	FRAME_PROP tMeirin_Tenma = { 96 * 2, 96 * 2, 4, 2, 8 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Tenma, L"Meirin_Tenma");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_tenma/honmeirin_tenma_R.bmp", L"Meirin_Tenma_R");
	FRAME_PROP tMeirin_Tenma_R = { 96 * 2, 96 * 2, 4, 2, 8 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Tenma_R, L"Meirin_Tenma_R");

	// ���� 2 : ����ü �߻� ���� ������ �� ������ ���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_falling/honmeirin_falling.bmp", L"Meirin_Falling");
	FRAME_PROP tMeirin_Falling = { 96 * 2, 96 * 2, 2, 1, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Falling, L"Meirin_Falling");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_falling/honmeirin_falling_R.bmp", L"Meirin_Falling_R");
	FRAME_PROP tMeirin_Falling_R = { 96 * 2, 96 * 2, 2, 1, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Falling_R, L"Meirin_Falling_R");




	// ���� 3 : ���� �� ü�� �����ϸ� �������� ���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_rising/honmeirin_rising_first.bmp", L"Meirin_Rising_Ready");
	FRAME_PROP tMeirin_Rising_Ready = { 64 * 2, 64 * 2, 1, 2, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Rising_Ready, L"Meirin_Rising_Ready");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_rising/honmeirin_rising_first_R.bmp", L"Meirin_Rising_Ready_R");
	FRAME_PROP tMeirin_Rising_Ready_R = { 64 * 2, 64 * 2, 1, 2, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Rising_Ready, L"Meirin_Rising_Ready_R");

	// ���� 3 : ���� �� ���߿������� ���� ���������� ���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_rising/honmeirin_rising.bmp", L"Meirin_Rising");
	FRAME_PROP tMeirin_Rising = { 96 * 2, 64 * 2, 1, 6, 6, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Rising, L"Meirin_Rising");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_rising/honmeirin_rising_R.bmp", L"Meirin_Rising_R");
	FRAME_PROP tMeirin_Rising_R = { 96 * 2, 64 * 2, 1, 6, 6, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Rising_R, L"Meirin_Rising_R");

	// ���� 3 : ���� �� ���� ���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_landing/honmeirin_landing.bmp", L"Meirin_Landing");
	FRAME_PROP tMeirin_Landing = { 64 * 2, 64 * 2, 3, 3, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Landing, L"Meirin_Landing");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_landing/honmeirin_landing_R.bmp", L"Meirin_Landing_R");
	FRAME_PROP tMeirin_Landing_R = { 64 * 2, 64 * 2, 3, 3, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Landing_R, L"Meirin_Landing_R");




	// ����ȭ ���� : ������ ����ü �߻� �غ� ���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_hadou_tame/honmeirin_hadou_tame.bmp", L"Meirin_Hadou_Ready");
	FRAME_PROP tMeirin_Hadou_Ready = { 128 * 2, 64 * 2, 1, 6, 6 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Hadou_Ready, L"Meirin_Hadou_Ready");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_hadou_tame/honmeirin_hadou_tame_R.bmp", L"Meirin_Hadou_Ready_R");
	FRAME_PROP tMeirin_Hadou_Ready_R = { 128 * 2, 64 * 2, 1, 6, 6 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Hadou_Ready_R, L"Meirin_Hadou_Ready_R");

	// ����ȭ ���� : ������ ����ü �߻� ���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_hadou/honmeirin_hadou.bmp", L"Meirin_Hadou");
	FRAME_PROP tMeirin_Hadou = { 128 * 2, 64 * 2, 1, 7, 7 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Hadou, L"Meirin_Hadou");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_hadou/honmeirin_hadou_R.bmp", L"Meirin_Hadou_R");
	FRAME_PROP tMeirin_Hadou_R = { 128 * 2, 64 * 2, 1, 7, 7 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Hadou_R, L"Meirin_Hadou_R");




	// Ŭ���� ���� : ���� �Ѿ������� ��� (���� ���� ������ ����)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_des_merged/honmeirin_des_merged_R.bmp", L"Meirin_Des");
	FRAME_PROP tMeirin_Des = { 96 * 2, 96 * 2, 5, 2, 10 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Des, L"Meirin_Des");

	// Ŭ���� ���� : ��� ��� (���� ���� ������ ����)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_dying/honmeirin_dying.bmp", L"Meirin_Dying");
	FRAME_PROP tMeirin_Dying = { 64 * 2, 64 * 2, 4, 1, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Dying, L"Meirin_Dying");




}

void CBoss_HonMeirin::Jump()
{
	if (CSceneMgr::Get_Instance()->Get_CurScene() == CSceneMgr::SC_EDIT)
		return;




	// �÷��̾� Jump �Լ� ����

	float fPlayerPosY = m_tInfo.fY;			// �÷��̾��� Y�� ����
	float fMargin = 20.f;					// �ڿ������� ������ ���� ������

	float fMaxVelocityY = 20;				// �ִ� ���ϼӵ� ����
	float fGravityConst = GRAVITY * 0.12;	// ���� �ڿ������� �߷°��ӵ� �� ���

	// �÷��̾� Y�� �ӵ�. 20 �ȳѰ� ����
	m_fVelocityY = ((m_fVelocityY + fGravityConst) >= fMaxVelocityY) ?
		fMaxVelocityY : m_fVelocityY + fGravityConst;


	
	float fDiffY;							// �� ������, �� ������ ���� Y�� ����

	if (m_tPrePos.x == 0 && m_tPrePos.y == 0)
		fDiffY = 0;
	else
		fDiffY = m_tInfo.fY - m_tPrePos.y;

	m_tPrePos = { m_tInfo.fX, m_tInfo.fY };	// ���� ������ ���� ����


	bool isFoundLine = false;
	if (m_isStartStage)						// �������� ���� 1ȸ�� ���� ����
	{
		m_isStartStage = false;
		isFoundLine = CTileCollisionMgr::Collision_Line(fPlayerPosY, m_tInfo.fX);
		if (!isFoundLine)
		{
			m_isJumping = true;
			m_eCurState = OBJST_JUMP;
		}
	}


	if (!m_isJumping)	// ���� X
	{
		isFoundLine = CTileCollisionMgr::Collision_Line(fPlayerPosY, m_tInfo.fX);

		if (!isFoundLine)
		{
			m_isJumping = true;
			m_eCurState = OBJST_JUMP;
			m_tInfo.fY += m_fVelocityY;
			std::cout << "[INFO][CWolf::Jump] Wolf Pos : (X : " << m_tInfo.fX << ", Y : " << m_tInfo.fY << ")" << std::endl;

		}
		else
		{
			// ã�� ���� �ֱ� �ѵ�, �������� ���ϰ� ���̳� ��.
			// (�� �÷��̾� �Ʒ��� �ٴ��� �ֱ� �ϳ� ���� ���������� ��)
			// �÷��̾ �������� �������ٰ� ����, �������� On.
			if (isFoundLine &&
				(m_tInfo.fY - fMargin > fPlayerPosY || m_tInfo.fY + fMargin / 4 < fPlayerPosY))
			{
				m_isJumping = true;
			}
			else
			{
				m_isJumping = false;
				//m_eCurState = OBJST_IDLE;
				m_fVelocityY = 0;
				m_tInfo.fY = fPlayerPosY;
			}
		}
	}
	else				// ���� O
	{
		m_tInfo.fY += m_fVelocityY;

		if (fDiffY > 0)
		{
			isFoundLine = CTileCollisionMgr::Collision_Line(fPlayerPosY, m_tInfo.fX); // ���⼭ �����̵���. �÷��̾���� ���� ���̰� ������� �ʴ� ��.

			// ���ϴܿ� �ణ�� �������� �־� ���� �ڿ������� Ÿ���� ��
			if (isFoundLine &&
				m_tInfo.fY - fMargin <= fPlayerPosY &&
				m_tInfo.fY + fMargin / 4 >= fPlayerPosY)
			{
				m_isJumping = false;
				m_fVelocityY = 0;
			}
			//m_eCurState = OBJST_IDLE;
		}

	}

}

void CBoss_HonMeirin::Motion_Change()
{
	// ������ �°� ������ ��

	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case OBJ_STATE::OBJST_IDLE:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 100;
			break;

		case OBJ_STATE::OBJST_JUMP:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			break;

		case OBJ_STATE::OBJST_RUN:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 100;
			break;
		}

		m_ePreState = m_eCurState;
		m_tFrame.iFrameCur = 0;
	}
}
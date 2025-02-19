#include "pch.h"
#include "CWolf.h"
#include "CBmpMgr.h"
#include "CSpritePropertyMgr.h"
#include "CPlayer.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"
#include "CTileCollisionMgr.h"
#include "SoundMgr.h"

CWolf::CWolf() :
	m_ePreState(OBJST_END),
	dwAttackReady(0),
	m_isAttackReady(false),
	m_isPreAttackReady(false),
	m_isJumpDirStretch(false)
{
	ZeroMemory(&m_tPrePos, sizeof(FPOINT));
}

CWolf::~CWolf()
{
}

void CWolf::Initialize()
{
	m_eCurState = OBJST_IDLE;


	m_fSpeed = 3.f;
	m_isFlying = false;

	m_iHp = 20;
	m_iMp = 5;
	m_fAtk = 20.f;


	m_tFrame.iFrameCur = 0;

	m_tFrame.dwSpeed = 100;				// ������ ��ȯ �ӵ�
	m_tFrame.dwTime = GetTickCount();	// ����� �ٲ���� �� �帥 �ð��� ����
	m_eRender = RENDER_GAMEOBJECT;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Monsters/wolf_jump_sprite/wolf_jump_sprite.bmp", L"Wolf_Jump");				// 64_64_X5Y5_21
	FRAME_PROP tWolf_Jump = { 96 * 2, 64 * 2, 1, 3, 3, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tWolf_Jump, L"Wolf_Jump");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Monsters/wolf_jump_sprite/wolf_jump_sprite_R.bmp", L"Wolf_Jump_R");				// 64_64_X5Y5_21
	FRAME_PROP tWolf_Jump_R = { 96 * 2, 64 * 2, 1, 3, 3, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tWolf_Jump_R, L"Wolf_Jump_R");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Monsters/wolf_run_sprite/wolf_run_sprite.bmp", L"Wolf_Run");				// 64_64_X5Y5_21
	FRAME_PROP tWolf_Run = { 64 * 2, 64 * 2, 1, 4, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tWolf_Run, L"Wolf_Run");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Monsters/wolf_run_sprite/wolf_run_sprite_R.bmp", L"Wolf_Run_R");				// 64_64_X5Y5_21
	FRAME_PROP tWolf_Run_R = { 64 * 2, 64 * 2, 1, 4, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tWolf_Run_R, L"Wolf_Run_R");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Monsters/wolf_stand_sprite/wolf_stand_sprite.bmp", L"Wolf_Stand");				// 64_64_X5Y5_21
	FRAME_PROP tWolf_Stand = { 64 * 2, 64 * 2, 1, 4, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tWolf_Stand, L"Wolf_Stand");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Monsters/wolf_stand_sprite/wolf_stand_sprite_R.bmp", L"Wolf_Stand_R");				// 64_64_X5Y5_21
	FRAME_PROP tWolf_Stand_R = { 64 * 2, 64 * 2, 1, 4, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tWolf_Stand_R, L"Wolf_Stand_R");
	
	Set_FrameProperty(tWolf_Stand);

	m_pFrameKey = L"Wolf_Stand";
}



// �߷µ� �޾ƾ���

int CWolf::Update()
{
	if (m_isDead)
		return OBJ_DEAD;

	// �̹��� ���� ũ�⿡ �°� ũ�� ����
	m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(m_pFrameKey);
	Set_FrameProperty(m_tFramePropCur);	// ���� �����ӿ� �� ���¿� �´� ������ �ݿ�
	Set_Scale(m_tFramePropCur.iCX, m_tFramePropCur.iCY);


	// �̹��� ���� ũ�⿡ �°� �浹���� ����
	m_tCollideInfo = { m_tInfo.fX, m_tInfo.fY - m_tInfo.fCY * 0.25f, m_tInfo.fCX * 0.7f, m_tInfo.fCY * 0.5f};


	__super::Update_Rect_UpStand();




	// �÷��̾� ��ġ�� �޾ƿ� �� �ൿ ���� ���� (ȭ�� �ȿ� ���� ����)

	int iOutX = 0, iOutY = 0;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY);

	if (!((m_tInfo.fX + m_tInfo.fCX < 0 - iOutX || m_tInfo.fX - iOutY - m_tInfo.fCX > WINCX - iOutX) ||
		(m_tInfo.fY + m_tInfo.fCY < 0 - iOutY || m_tInfo.fY - iOutY - m_tInfo.fCY > WINCY - iOutY)))
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
		if (pPlayer == nullptr)
			return OBJ_NOEVENT;
		INFO tPlayerInfo = *pPlayer->Get_Info();

		// 1. ���� �Ÿ�(iDistToPlayer)���� ���� (���� �غ� ������ false �� ����)
		if (!m_isAttackReady && !m_isJumping)
		{
			int iDistToPlayer = 250;
			float fDistanceX = abs(abs(tPlayerInfo.fX) - abs(m_tInfo.fX));
			m_eCurState = OBJST_RUN;
			if (m_isStretch)	{ m_pFrameKey = L"Wolf_Run"; }
			else				{ m_pFrameKey = L"Wolf_Run_R"; }

			if (iDistToPlayer <= fDistanceX) // �Ÿ��� �ִٸ� ����
			{
				if (tPlayerInfo.fX > m_tInfo.fX)	{ m_tInfo.fX += m_fSpeed; m_isStretch = false; }
				else								{ m_tInfo.fX -= m_fSpeed; m_isStretch = true;  }
			}
			else
			{
				// ���� �غ� ������ true��.
				m_isAttackReady = true;
				m_eCurState = OBJST_IDLE;

			if (m_isStretch)	{ m_pFrameKey = L"Wolf_Stand"; }
			else				{ m_pFrameKey = L"Wolf_Stand_R"; }

			}
		}
		else
		{
			DWORD dwCurrentTime = GetTickCount();

			int iAttackDelay = 1500;
			if (((dwCurrentTime - dwAttackReady) >= iAttackDelay) && dwAttackReady != 0) // unsigned ���ӿ� ����. ������ �� ��Ȳ�� ����� �ȵ�
			{
				// �÷��̾ ���� 30������ ���̷� ������
				// m_eCurState�� �����ؾ���
				// �� �� dw���� �ʱ�ȭ��
				// �������� ������ �ٽ� ���� �غ� ���� �� �� false�� ������
				// laterEdit
				m_eCurState = OBJST_JUMP;
				//m_pFrameKey = L"Wolf_Jump";

				dwAttackReady = 0;
				//m_isJumping = true;

				m_isJumping = true;

				// Y ���� ���� �� �� ó�� �ְ�,
				// X ���� �׳� ���� �� ��� �÷��̾ ���� �̵��ϰ� �ϸ� �Ƿ��� 
				m_fVelocityY = -15.f;
				CSoundMgr::Get_Instance()->StopSound(SOUND_WOLF);
				CSoundMgr::Get_Instance()->PlaySound(L"s1006_cat.wav", SOUND_WOLF, 0.2f);
				
				m_isAttackReady = false;
				m_isPreAttackReady = false;

			}
		}
		
		// ���� �غ� ���°� �� �� 1ȸ�� �����ϴ� �κ� (Ÿ�̸�)
		if (m_isAttackReady && !m_isPreAttackReady)
		{
			m_isPreAttackReady = true;
			dwAttackReady = GetTickCount();

			if (tPlayerInfo.fX > m_tInfo.fX)	m_isJumpDirStretch = false;
			else								m_isJumpDirStretch = true;
		}

		if (m_isJumping)
		{
			// Jump_R �̹����� �������� �����̱⿡ �̹��� ������ �ݴ�� ��.
			if (!m_isJumpDirStretch)			{ m_tInfo.fX += m_fSpeed * 5; m_pFrameKey = L"Wolf_Jump_R"; }
			else								{ m_tInfo.fX -= m_fSpeed * 5; m_pFrameKey = L"Wolf_Jump"; }
		}
		// 2. ���� �غ� ������ true�� ��, Ÿ�̸� ��
		//    ���� ���� �ð� �Ŀ� 30������ ���̷� ����
		//    ���� ����, ���� ������ �� (m_isJumping  �� false�� �� ��)
		//    ���� �غ� ������ falseȭ, �ٽ� 1 �������� ���ư�


		
	}

	return OBJ_NOEVENT;
}

void CWolf::Late_Update()
{
	// ī�޶� �ȿ� �������� ����
	int iOutX = 0, iOutY = 0;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY);

	if (!((m_tInfo.fX + m_tInfo.fCX < 0 - iOutX || m_tInfo.fX - iOutY - m_tInfo.fCX > WINCX - iOutX) ||
		(m_tInfo.fY + m_tInfo.fCY < 0 - iOutY || m_tInfo.fY - iOutY - m_tInfo.fCY > WINCY - iOutY)))
	{

		// ������ �����

		Jump();
		Move_Frame();
		Motion_Change();

	}
}

void CWolf::Render(HDC hDC)
{
	// ī�޶� �ȿ� �������� ����
	int iOutX = 0, iOutY = 0;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY);

	if (!((m_tInfo.fX + m_tInfo.fCX < 0 - iOutX || m_tInfo.fX - iOutY - m_tInfo.fCX > WINCX - iOutX) ||
		(m_tInfo.fY + m_tInfo.fCY < 0 - iOutY || m_tInfo.fY - iOutY - m_tInfo.fCY > WINCY - iOutY)))
	{

		// ������ �����

		int iOutX = m_tRect.left;
		int iOutY = m_tRect.top;
		CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // ���������� ������ų ��ǥ.

		HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

		GdiTransparentBlt(hDC,					// ���������� �׸� DC
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
}

void CWolf::Release()
{
}

void CWolf::Jump()
{
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

void CWolf::Motion_Change()
{
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
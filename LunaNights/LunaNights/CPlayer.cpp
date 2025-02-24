#include "pch.h"
#include "CPlayer.h"


#include "CAbstractFactory.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"
#include "CSpritePropertyMgr.h"
#include "CTileCOllisionMgr.h"
#include "CPlayerBullet.h"
#include "SoundMgr.h"
#include "CEffect.h"

CPlayer::CPlayer() :
	m_ePreState(OBJST_END),
	m_dwTime(GetTickCount()),
	m_dwStateChangeTime(0),
	m_isStartStage(false),
	m_dwGodTime(0),
	m_dwSnailReadyTime(0),
	m_dwSnailTime(0),
	m_isMaxTP(true),
	m_isMaxMP(true),
	m_iMessageWith(0),
	m_iMessageOrder(0)

{
	ZeroMemory(&m_tPrePos, sizeof(FPOINT));
}

CPlayer::~CPlayer()
{
	Release();
}

// ----

void CPlayer::Initialize()
{
	m_eCurState = OBJST_IDLE;

	// ���� �ʱ�ȭ
	m_tInfo = { 100.f, WINCY / 2.f + 100.f, 64.f, 64.f };
	m_fSpeed = 6.f;
	m_fAngle = 0.f;
	m_isJumping = false;
	m_fTime = 0.f;
	m_fJumpSpeed = 20.f;
	m_fPosinLength = 50.f;
	m_fAngle = 0.f;

	// ����-���� �ʱ�ȭ
	m_iHp = 100;
	m_iMaxHp = 100;
	m_iMp = 100;
	m_iMaxMp = 100;
	m_fTp = 85;
	m_iGold = 50;
	m_fAtk = 5.2;
	m_iKnife = 18;
	m_isGetWatch = false;

	m_iTimeMode = 0;	// 0 Idle, 1 Snail, 2 Stop


	// BmpMgr �� ���� �̹��� �ε�, �����ؼ� ���� ����
	LoadImages();


	FRAME_PROP tPlayer_IDLE = { 64*2, 64*2, 5, 5, 21, 5 };
	m_pFrameKey = L"Player_IDLE";
	m_tFramePropCur = tPlayer_IDLE;

	// �������� ����, �� �ε���, ��� ��ȣ ���� ����
	m_tFrame.iFrameCur = 0;
	Set_FrameProperty(tPlayer_IDLE);

	m_tFrame.dwSpeed = 100;				// ������ ��ȯ �ӵ�
	m_tFrame.dwTime = GetTickCount();	// ����� �ٲ���� �� �帥 �ð��� ����

	m_eRender = RENDER_GAMEOBJECT;

	m_dwMpRegenTime = GetTickCount();
	m_dwTpRegenTime = GetTickCount();

	CCameraMgr::Get_Instance()->Set_Target(this);
}

int CPlayer::Update()
{
	if (m_isDead)
		return OBJ_DEAD;

	Key_Input();

	// ȭ�� ������ ������ �� �ǵ���
	if (m_tInfo.fY > WINCY)
		m_tInfo.fY = WINCY / 2;

	// ���� �ڿ�ȸ��
	if (m_dwMpRegenTime + 500 <= GetTickCount() && !(m_iMp >= m_iMaxMp))
	{
		m_iMp++;
		m_dwMpRegenTime = GetTickCount();
	}

	// MP MAX �������
	if (m_iMp == m_iMaxMp && m_isMaxMP == false)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"vo_02fx.wav", SOUND_VO_MPMAX, 0.2f);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT,
			CAbstractFactory<CEffect>::CreateStatusEffect(m_tInfo.fX, m_tInfo.fY - m_tInfo.fCY * 1.2, 0.f, true, CEffect::STT_MPRECOVERY, 1.5f));
		m_isMaxMP = true;
	}
	if (m_iMp < m_iMaxMp)
	{
		m_isMaxMP = false;
	}


	// TP �Ҹ� �� ȸ��
	if (m_dwTpRegenTime + 200 <= GetTickCount() && (m_fTp <= 85))
	{

		if		(m_iTimeMode == 0 && m_fTp < 85)		{ m_fTp += 1.f; }
		else if (m_iTimeMode == 2)						{ m_fTp -= 1.f; }

		m_dwTpRegenTime = GetTickCount();

		if (m_fTp >= 85)	{	m_fTp = 85;		}
	
		// Tp�� ������ ������� ����
		if (m_fTp <= 0)
		{
			m_fTp = 0;
			m_iTimeMode = 0;
			CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT,
				CAbstractFactory<CEffect>::CreateStatusEffect(m_tInfo.fX, m_tInfo.fY - m_tInfo.fCY * 1.2, 0.f, true, CEffect::STT_NOTIME, 1.5f));
			CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CEffect>::Create(m_tInfo.fX, m_tInfo.fY - m_tInfo.fCY / 2, 0.f, L"Effect_SkillUse", true));
			CSoundMgr::Get_Instance()->StopSound(SOUND_TIME_END);
			CSoundMgr::Get_Instance()->PlaySound(L"s21_magicout.wav", SOUND_TIME_END, 0.2f);
			CSoundMgr::Get_Instance()->StopSound(SOUND_TIME_END2);
			CSoundMgr::Get_Instance()->PlaySound(L"time_stop_end.wav", SOUND_TIME_END2, 0.2f);
		}
	}
	if (m_isJumping && m_iTimeMode == 2)
		m_fTp -= 0.05f;

	// TP MAX �������
	if (m_fTp == 85 && m_isMaxTP == false)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"vo_04fx.wav", SOUND_VO_TPMAX, 0.2f);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT,
			CAbstractFactory<CEffect>::CreateStatusEffect(m_tInfo.fX, m_tInfo.fY - m_tInfo.fCY * 1.2, 0.f, true, CEffect::STT_TIMEMAX, 1.5f));
		m_isMaxTP = true;
	}
	if (m_fTp < 85)
	{
		m_isMaxTP = false;
	}




	// ������ ��� �ߵ� �� ó�� 1ȸ �ߵ�
	if (m_iTimeMode == 1 && m_dwSnailTime == 0)
	{
		m_dwSnailTime = GetTickCount();
	}
	// ������ ��� �ߵ��� 3�ʰ� ����
	else if (m_iTimeMode == 1 && m_dwSnailTime != 0)
	{
		DWORD dwSnailMaxTime = 3000;
		if (m_dwSnailTime + dwSnailMaxTime <= GetTickCount())
		{
			m_dwSnailTime = 0;
			m_iTimeMode = 0;

			std::cout << "[INFO][CPlayer::Key_Input] Snail Mode Deactivated!" << std::endl;

			HDC tmpDC = GetDC(g_hWnd);
			Rectangle(tmpDC, 0, 0, WINCX, WINCY);
		}
	}










	// �ǰݵ� �� iGodModeTime �ʰ� ���� ����
	int iGodModeTime = 1500;
	if (m_isPreGod == false && m_isGod == true)	// ������ ���� �÷��̾� ������ ����
	{
		m_dwGodTime = GetTickCount();
		m_isPreGod = true;
	}
	if (m_dwGodTime + iGodModeTime <= GetTickCount())
	{
		m_isPreGod = false;
		m_isGod = false;
	}


	m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(m_pFrameKey);
	Set_FrameProperty(m_tFramePropCur);	// ���� �����ӿ� �� ���¿� �´� ������ �ݿ�
	Set_Scale(m_tFramePropCur.iCX, m_tFramePropCur.iCY);

	m_tCollideInfo = {	m_tInfo.fX, 
						m_tInfo.fY - 64,
						48,
						128 };

	// �÷��̾�� ��ǥ�κ��� ���� ������ �������Ѿ� �� (�������Ƿ�)
	__super::Update_Rect_UpStand();	// 2��ũ�� ���� ���� ��ǥ����

	//std::cout << "[INFO][CPlayer::Update] PlayerPos : " << m_tInfo.fX << ", " << m_tInfo.fY << std::endl;

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update()
{
	Jump();
	Camera_Offset();
	Move_Frame();
	Motion_Change();


}

void CPlayer::Render(HDC hDC)
{
	int iOutX = m_tRect.left;
	int iOutY = m_tRect.top;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // ���������� ������ų ��ǥ.

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);


	// ** ���⼭ ��ǥ�� 1�� �������� ��Ƴ��� ������ 2�� ũ��� �ϴϱ� �ȸ¾���..
	// �׷��� 2��ũ�� ������ Update_Rect ... 2X �Լ����� ���� ����.

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




	// snail gauge
	if (m_iTimeMode != 2 && m_dwSnailReadyTime >= 20)
	{	
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_SNAILSOCKET");
		FRAME_PROP tCurProp = CSpritePropertyMgr::Get_Instance()->Find_Property(L"UI_SNAILSOCKET");


		// snail socket image
		GdiTransparentBlt(	hDC,					// ���������� �׸� DC
							iOutX + m_tInfo.fCX/2 - 23,					// ������� ��ġ X, Y��ǥ
							iOutY - 50,
							tCurProp.iCX,	// ���� ���� ����, ���� ����.
							tCurProp.iCY,
							hMemDC,					// ��Ʈ���� ������ �ִ� DC
							0,	// ����Ϸ��� ��Ʈ����Ʈ �̹��� �������� ��ǥ
							0,
							tCurProp.iCX,	// ��Ʈ���� ����� ����, ���� ����
							tCurProp.iCY,
							RGB(255, 0, 255));		// ������ ����



		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_SNAILGAUGE");
		tCurProp = CSpritePropertyMgr::Get_Instance()->Find_Property(L"UI_SNAILGAUGE");

		
		// snail gauge image
		int iCurFrameSnail = ((m_dwSnailReadyTime - 20) / 80.f * tCurProp.iFrameAmount) - 1;

		if (iCurFrameSnail > tCurProp.iFrameAmount - 1)		{ iCurFrameSnail = tCurProp.iFrameAmount - 1; }
		else if (iCurFrameSnail < 0)						{ iCurFrameSnail = 0; }

		GdiTransparentBlt(	hDC,					// ���������� �׸� DC
							iOutX + m_tInfo.fCX/2 - 23,					// ������� ��ġ X, Y��ǥ
							iOutY - 50,
							tCurProp.iCX,	// ���� ���� ����, ���� ����.
							tCurProp.iCY,
							hMemDC,					// ��Ʈ���� ������ �ִ� DC
							tCurProp.iCX * (iCurFrameSnail % (tCurProp.iFrameMaxX)),	// ����Ϸ��� ��Ʈ����Ʈ �̹��� �������� ��ǥ
							tCurProp.iCY * (iCurFrameSnail / (tCurProp.iFrameMaxX)),
							tCurProp.iCX,	// ��Ʈ���� ����� ����, ���� ����
							tCurProp.iCY,
							RGB(255, 0, 255));		// ������ ����

	}



	// �浹 ���� Ȯ�ο�
	// ���� ���� Rect ����, �ʷϻ��� Info ���� ��ġ.

	//int iTestOutX = 0, iTestOutY = 0;
	//CCameraMgr::Get_Instance()->Get_RenderPos(iTestOutX, iTestOutY);
	//
	//HPEN hGreenPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	//HPEN hOldPen = (HPEN)SelectObject(hDC, hGreenPen);
	//
	//int infoLeft	= (int)(m_tCollideInfo.fX - m_tCollideInfo.fCX * 0.5f) + iTestOutX;
	//int infoTop		= (int)(m_tCollideInfo.fY - m_tCollideInfo.fCY * 0.5f) + iTestOutY;
	//int infoRight	= (int)(m_tCollideInfo.fX + m_tCollideInfo.fCX * 0.5f) + iTestOutX;
	//int infoBottom	= (int)(m_tCollideInfo.fY + m_tCollideInfo.fCY * 0.5f) + iTestOutY;
	//
	//MoveToEx(hDC, infoLeft, infoTop, nullptr);
	//LineTo(hDC, infoRight, infoTop);
	//LineTo(hDC, infoRight, infoBottom);
	//LineTo(hDC, infoLeft, infoBottom);
	//LineTo(hDC, infoLeft, infoTop);
	//
	//HPEN hRedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	//hOldPen = (HPEN)SelectObject(hDC, hRedPen);
	//
	//int rectLeft	= m_tRect.left + iTestOutX;
	//int rectTop		= m_tRect.top + iTestOutY;
	//int rectRight	= m_tRect.right + iTestOutX;
	//int rectBottom	= m_tRect.bottom + iTestOutY;
	//
	//MoveToEx(hDC, rectLeft, rectTop, nullptr);
	//LineTo(hDC, rectRight, rectTop);
	//LineTo(hDC, rectRight, rectBottom);
	//LineTo(hDC, rectLeft, rectBottom);
	//LineTo(hDC, rectLeft, rectTop);
	//
	//SelectObject(hDC, hOldPen);
	//DeleteObject(hGreenPen);
	//DeleteObject(hRedPen);


	if ( true )// GetTickCount() % 2 == 1)
	{
		//std::cout << "Player Rect : \t" << m_tRect.left << "\t" << m_tRect.top << "\t" << m_tRect.right << "\t" << m_tRect.bottom << std::endl;
		//std::cout << "Player Scale: \t" << m_tInfo.fCX << "\t" << m_tInfo.fCY << std::endl;
		//std::cout << "Player Info : \t" << m_tInfo.fX << "\t" << m_tInfo.fY << std::endl;

		//std::cout << "CurrentState: \t" << m_eCurState << std::endl;
		//std::cout << "CurrentFrame: \t" << m_tFrame.iFrameCur + 1 << " / " << m_tFrame.iFrameAmount << std::endl;
		//std::cout << "OriginSpriteIndex: \t" << ((m_tFrame.iFrameCur) % (m_tFrame.iFrameMaxX)) << "\t" << ((m_tFrame.iFrameCur) / (m_tFrame.iFrameMaxX)) << std::endl;

	}


	
}

void CPlayer::Release()
{	
}

// ----

void CPlayer::Key_Input()
{
	// ********************
	// ** �ǰ���
	// ********************
	if (m_eCurState == OBJST_DAMAGED)
	{
		if (m_isStretch) { m_pFrameKey = L"Player_DAMAGE_R";	m_tInfo.fX += 5; }
		else { m_pFrameKey = L"Player_DAMAGE";		m_tInfo.fX -= 5; }

		m_tInfo.fY--;
		m_dwStateChangeTime = GetTickCount();

		// m_tFrame �� Jump ������Ƽ�� ���� ������ �߻��Ͽ���
		if ((m_tFrame.iFrameCur < m_tFrame.iFrameAmount - 1) || (m_tFrame.iFrameAmount != 12))	// 12 �� ������ ���� ���� ������Ƽ�� iFrameAmount�� �ǹ�
			return;
		else
		{
			m_eCurState = OBJST_IDLE;
			if (m_isStretch) { m_pFrameKey = L"Player_IDLE_R"; }
			else { m_pFrameKey = L"Player_IDLE"; }
		}
	}


	// ********************
	// ** ���� Ű (���� ���� �ƴ� ������ ����)
	// ********************
	if (CKeyMgr::Get_Instance()->Key_Down('X') && !m_isJumping)
	{
		// �ö󰡴� ��, �������� ���� ��� ���ǵ� ���� �������� ��
		// �� ������ �߿� õõ�� �������� Ȱ���ϴ� ���ǵ� ���� �������� ��
		// later


		m_eCurState = OBJST_JUMP;
		m_fVelocityY = -20.f;	// �ӽð�. Jump()

		m_isJumping = true;
	}

	if (m_isJumping && m_eCurState != OBJST_JUMP_ATTACK1) // �ƴҼ���
	{
		if (m_isStretch)	m_pFrameKey = L"Player_JUMP_R";
		else				m_pFrameKey = L"Player_JUMP";

		//m_dwStateChangeTime = 0;
		m_dwStateChangeTime = GetTickCount();
	}


	if (CKeyMgr::Get_Instance()->Key_Pressing('X') &&
		m_isJumping &&
		!m_eCurState != OBJST_DAMAGED)
	{
		float fDiffY;							// �� ������, �� ������ ���� Y�� ����

		if (m_tPrePos.x == 0 && m_tPrePos.y == 0)	fDiffY = 0;
		else										fDiffY = m_tInfo.fY - m_tPrePos.y;

		if (fDiffY > 0)								m_dwFallTime++;
	}
	else
	{
		m_dwFallTime = 0;
	}

	if (m_dwFallTime >= 3)
	{
		// Ȱ�� ����
		if (m_eCurState != OBJST_JUMP_ATTACK1)
		{
			if (m_isStretch)	m_pFrameKey = L"Player_GLIDE_R";
			else				m_pFrameKey = L"Player_GLIDE";
			
			m_eCurState = OBJST_GLIDE;
		}


		m_fVelocityY = +3.f;
	}


	// ********************
	// ** ���� Ű
	// ********************
	if (CKeyMgr::Get_Instance()->Key_Down('Z'))
	{
		if (m_iMp >= 3)
		{
			if (m_iTimeMode != 2)
			{
				m_iMp -= 3;
				CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CEffect>::Create(173, 91, 0.f, L"Effect_MPDown", false));
			}
			else
			{
				m_fTp -= 8;
				CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CEffect>::Create(560, 40, 0.f, L"Effect_TPDown", false));
			}

			CSoundMgr::Get_Instance()->StopSound(SOUND_ATTACK);
			CSoundMgr::Get_Instance()->PlaySound(L"s800_kengeki00.wav", SOUND_ATTACK, 0.2f);

			int iAtkEffectSpaceX = 60;
			if (m_isStretch)	iAtkEffectSpaceX *= -1;
			else				iAtkEffectSpaceX *= +1;

			CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CEffect>::Create(m_tInfo.fX + iAtkEffectSpaceX, m_tInfo.fY - m_tInfo.fCY / 2, 0.f, L"Effect_AtkRing", true));


			//�� Į���� ���ư� ��ġ/���� ���ϴ� �κ�

			float iKnifeSpace = 20.f;

			float bulletX = m_tInfo.fX + m_fPosinLength * cos(m_fAngle * DEG2RAD);
			float bulletY = m_tInfo.fY - m_fPosinLength * sin(m_fAngle * DEG2RAD);
			CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYERBULLET, CAbstractFactory<CPlayerBullet>::Create(bulletX, bulletY - m_tInfo.fCY / 2, m_fAngle));
			std::cout << "[INFO][CPlayer::Key_Input] " << "Bullet Created on..  " << bulletX << ", " << bulletY - m_tInfo.fCY / 2 << std::endl;

			if (!m_isStretch)
				bulletX -= iKnifeSpace;
			else
				bulletX += iKnifeSpace;

			bulletY += iKnifeSpace;
			CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYERBULLET, CAbstractFactory<CPlayerBullet>::Create(bulletX, bulletY - m_tInfo.fCY / 2, m_fAngle));
			std::cout << "[INFO][CPlayer::Key_Input] " << "Bullet Created on..  " << bulletX << ", " << bulletY - m_tInfo.fCY / 2 << std::endl;

			if (!m_isStretch)
				bulletX -= iKnifeSpace;
			else
				bulletX += iKnifeSpace;

			bulletY -= iKnifeSpace * 2;
			CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYERBULLET, CAbstractFactory<CPlayerBullet>::Create(bulletX, bulletY - m_tInfo.fCY / 2, m_fAngle));
			std::cout << "[INFO][CPlayer::Key_Input] " << "Bullet Created on..  " << bulletX << ", " << bulletY - m_tInfo.fCY / 2 << std::endl;

			//�� ****************************

			if (!m_isJumping &&
				(m_eCurState != OBJST_RUN &&
					m_eCurState != OBJST_RUN_ATTACK1 &&
					m_eCurState != OBJST_RUN_ATTACK2 &&
					m_eCurState != OBJST_RUN_ATTACK3 &&
					m_eCurState != OBJST_RUN_ATTACK4) ||
				(m_eCurState == OBJST_IDLE ||
					m_eCurState == OBJST_ACTION1 ||
					m_eCurState == OBJST_ACTION2 ||
					m_eCurState == OBJST_ACTION3 ||
					m_eCurState == OBJST_ACTION4 ||
					m_eCurState == OBJST_RUNSTART ||
					m_eCurState == OBJST_RUNEND))
			{
				if (m_eCurState == OBJST_IDLE)
				{
					m_dwStateChangeTime = GetTickCount();
					if (m_isStretch)	m_pFrameKey = L"Player_ACTION_R";
					else				m_pFrameKey = L"Player_ACTION";
					m_eCurState = OBJST_ACTION1;
				}
				else if (m_eCurState == OBJST_ACTION1)
				{
					m_dwStateChangeTime = GetTickCount();
					if (m_isStretch)	m_pFrameKey = L"Player_ACTION2_R";
					else				m_pFrameKey = L"Player_ACTION2";
					m_eCurState = OBJST_ACTION2;
				}
				else if (m_eCurState == OBJST_ACTION2)
				{
					m_dwStateChangeTime = GetTickCount();
					if (m_isStretch)	m_pFrameKey = L"Player_ACTION3_R";
					else				m_pFrameKey = L"Player_ACTION3";
					m_eCurState = OBJST_ACTION3;
				}
				else if (m_eCurState == OBJST_ACTION3)
				{
					m_dwStateChangeTime = GetTickCount();
					if (m_isStretch)	m_pFrameKey = L"Player_ACTION4_R";
					else				m_pFrameKey = L"Player_ACTION4";
					m_eCurState = OBJST_ACTION4;
				}
				else if (m_eCurState == OBJST_ACTION4)
				{
					m_dwStateChangeTime = GetTickCount();
					if (m_isStretch)	m_pFrameKey = L"Player_ACTION_R";
					else				m_pFrameKey = L"Player_ACTION";
					m_eCurState = OBJST_ACTION1;
				}
			}
			else if ((!m_isJumping && m_eCurState == OBJST_RUN) ||
				m_eCurState == OBJST_RUN_ATTACK1 ||
				m_eCurState == OBJST_RUN_ATTACK2 ||
				m_eCurState == OBJST_RUN_ATTACK3 ||
				m_eCurState == OBJST_RUN_ATTACK4)
			{
				if (m_eCurState == OBJST_RUN)
				{
					m_dwStateChangeTime = GetTickCount();
					if (m_isStretch)	m_pFrameKey = L"Player_RUN_ATTACK_R";
					else				m_pFrameKey = L"Player_RUN_ATTACK";
					m_eCurState = OBJST_RUN_ATTACK1;
				}
				else if (m_eCurState == OBJST_RUN_ATTACK1)
				{
					m_dwStateChangeTime = GetTickCount();
					if (m_isStretch)	m_pFrameKey = L"Player_RUN_ATTACK2_R";
					else				m_pFrameKey = L"Player_RUN_ATTACK2";
					m_eCurState = OBJST_RUN_ATTACK2;
				}
				else if (m_eCurState == OBJST_RUN_ATTACK2)
				{
					m_dwStateChangeTime = GetTickCount();
					if (m_isStretch)	m_pFrameKey = L"Player_RUN_ATTACK3_R";
					else				m_pFrameKey = L"Player_RUN_ATTACK3";
					m_eCurState = OBJST_RUN_ATTACK3;
				}
				else if (m_eCurState == OBJST_RUN_ATTACK3)
				{
					m_dwStateChangeTime = GetTickCount();
					if (m_isStretch)	m_pFrameKey = L"Player_RUN_ATTACK4_R";
					else				m_pFrameKey = L"Player_RUN_ATTACK4";
					m_eCurState = OBJST_RUN_ATTACK4;
				}
				else if (m_eCurState == OBJST_RUN_ATTACK4)
				{
					m_dwStateChangeTime = GetTickCount();
					if (m_isStretch)	m_pFrameKey = L"Player_RUN_ATTACK_R";
					else				m_pFrameKey = L"Player_RUN_ATTACK";
					m_eCurState = OBJST_RUN_ATTACK1;
				}
			}
			else if (m_isJumping)
			{
				// ���� ����
				m_dwStateChangeTime = GetTickCount();
				if (m_isStretch)	m_pFrameKey = L"Player_JUMP_ATTACK_R";
				else				m_pFrameKey = L"Player_JUMP_ATTACK";
				m_eCurState = OBJST_JUMP_ATTACK1;
			}


		}

		else
		{
			// nomp ��� laterEdit
			CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT,
				CAbstractFactory<CEffect>::CreateStatusEffect(m_tInfo.fX, m_tInfo.fY - m_tInfo.fCY * 1.2, 0.f, true, CEffect::STT_NOMP, 1.5f));
		}

	}


	if (CKeyMgr::Get_Instance()->Key_Pressing('Z') &&
		m_iTimeMode != 2)
	{
		m_dwSnailReadyTime += 1;
		if (m_iTimeMode == 1 && m_dwSnailReadyTime >= 80)
			m_dwSnailReadyTime = 80;

		if (m_dwSnailReadyTime == 100)
			CSoundMgr::Get_Instance()->PlaySound(L"s811_jumpbonus.wav", SOUND_TIME_SLOWREADY, 0.2f);
			
		std::cout << "[INFO][CPlayer::Key_Input] Current SnailReady Stack : " << m_dwSnailReadyTime << std::endl;
	}
	else if (CKeyMgr::Get_Instance()->Key_Up('Z'))
	{
		DWORD dwSnailReadyTime = 100;
		if (m_dwSnailReadyTime >= dwSnailReadyTime)
		{
			// ������ ��� �ߵ�
			m_iTimeMode = 1;
			CSoundMgr::Get_Instance()->StopSound(SOUND_TIME_SLOW);
			CSoundMgr::Get_Instance()->PlaySound(L"s1017_slow_motion.wav", SOUND_TIME_SLOW, 0.2f);
			CCameraMgr::Get_Instance()->Set_ShakeStrength(20.f);
			CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CEffect>::Create(m_tInfo.fX, m_tInfo.fY - m_tInfo.fCY/2, 0.f, L"Effect_SkillUse", true));


			std::cout << "[INFO][CPlayer::Key_Input] Snail Mode Activated!" << std::endl;

			HDC tmpDC = GetDC(g_hWnd);
			Rectangle(tmpDC, 0, 0, WINCX, WINCY);
		}

		m_dwSnailReadyTime = 0;
	}



	// ********************
	// ** �ð� ���� Ű
	// ********************
	if (CKeyMgr::Get_Instance()->Key_Down('A') && m_isGetWatch)
	{
		if (m_iTimeMode == 0)
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_TIME_SKILL);
			CSoundMgr::Get_Instance()->PlaySound(L"s06_skill.wav", SOUND_TIME_SKILL, 0.2f);
			CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CEffect>::Create(m_tInfo.fX, m_tInfo.fY - m_tInfo.fCY / 2, 0.f, L"Effect_SkillUse", true));
			m_iTimeMode = 2;
		}
		else if (m_iTimeMode == 2)
		{

			CSoundMgr::Get_Instance()->StopSound(SOUND_TIME_END);
			CSoundMgr::Get_Instance()->PlaySound(L"s21_magicout.wav", SOUND_TIME_END, 0.2f);
			CSoundMgr::Get_Instance()->StopSound(SOUND_TIME_END2);
			CSoundMgr::Get_Instance()->PlaySound(L"time_stop_end.wav", SOUND_TIME_END2, 0.2f);
			
			m_iTimeMode = 0;
			CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CEffect>::Create(m_tInfo.fX, m_tInfo.fY - m_tInfo.fCY / 2, 0.f, L"Effect_SkillUse", true));
		}
	}






	// ********************
	// ** ���� ����Ű
	// ********************
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		// ���� ���°� �ݴ븦 �ٶ� ä�� IDLE or RUN or RUNSTART �̾��ٸ�
		if (m_ePreState == OBJST_RUN && !m_isStretch ||
			m_ePreState == OBJST_IDLE && !m_isStretch ||
			m_ePreState == OBJST_RUNSTART && !m_isStretch ||
			m_ePreState == OBJST_RUNCHANGE && !m_isStretch ||
			m_ePreState == OBJST_RUNEND && !m_isStretch )
		{
			m_dwStateChangeTime = GetTickCount();
			m_pFrameKey = L"Player_RUNCHANGE_R"; // RUNCHANGE�� ����
			m_eCurState = OBJST_RUNCHANGE;
		}
		// ���� ���°� IDLE �̾��ٸ�
		else if (m_ePreState == OBJST_IDLE ||
				m_ePreState == OBJST_RUNEND)
		{
			m_dwStateChangeTime = GetTickCount();	// Ÿ�̸� üũ
			m_pFrameKey = L"Player_RUNSTART_R";		// ������Ű�� ���� = "�޸��� ����(��������)"
			m_eCurState = OBJST_RUNSTART;
		}

		// ����� �ٲ�� Ÿ�̸Ӱ� ���ư��� ���̾���, �ش� �������� �������� ������ ����
		if (m_dwStateChangeTime != 0)
		{
			if ((m_tFrame.iFrameCur == (m_tFrame.iFrameAmount - 1)) &&
				(m_dwStateChangeTime - GetTickCount()) >= (m_tFrame.iFrameAmount * m_tFrame.dwSpeed))
			{
				m_pFrameKey = L"Player_RUN_R";
				m_dwStateChangeTime = 0;
				m_eCurState = OBJST_RUN;
			}
		}
		else
		{
			m_pFrameKey = L"Player_RUN_R";			// ������Ű�� ���� = "�޸�����(��������)"
			m_eCurState = OBJST_RUN;
		}

		m_tInfo.fX -= m_fSpeed;
		if (m_iTimeMode == 2) m_fTp -= 0.05f;
		m_isStretch = true;
		m_fAngle = 180.f;
		return;
	}



	// ********************
	// ** �ɱ� Ű (���� ���� �ƴ� ������ ����)
	// ********************
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN) && !m_isJumping)
	{
		m_eCurState = OBJST_DOWN;
		if (m_isStretch)	m_pFrameKey = L"Player_DOWN_R";
		else				m_pFrameKey = L"Player_DOWN";
		//m_dwStateChangeTime = GetTickCount();

		return;
	}



	
	// ********************
	// ** ������ ����Ű
	// ********************
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		// ���� ���°� �ݴ븦 �ٶ� ä�� IDLE or RUN or RUNSTART �̾��ٸ�
		if (m_ePreState == OBJST_RUN && m_isStretch ||
			m_ePreState == OBJST_IDLE && m_isStretch ||
			m_ePreState == OBJST_RUNSTART && m_isStretch ||
			m_ePreState == OBJST_RUNCHANGE && m_isStretch ||
			m_ePreState == OBJST_RUNEND && m_isStretch)
		{
			m_dwStateChangeTime = GetTickCount();
			m_pFrameKey = L"Player_RUNCHANGE"; // RUNCHANGE�� ����
			m_eCurState = OBJST_RUNCHANGE;
		}
		// ���� ���°� IDLE �̾��ٸ�
		else if (m_ePreState == OBJST_IDLE||
				m_ePreState == OBJST_RUNEND)
		{
			m_dwStateChangeTime = GetTickCount();	// Ÿ�̸� üũ
			m_pFrameKey = L"Player_RUNSTART";		// ������Ű�� ���� = "�޸��� ����(��������)"
			m_eCurState = OBJST_RUNSTART;
		}

		// ����� �ٲ�� Ÿ�̸Ӱ� ���ư��� ���̾���, �ش� �������� �������� ������ ����
		if (m_dwStateChangeTime != 0)
		{
			if ((m_tFrame.iFrameCur == (m_tFrame.iFrameAmount - 1)) &&
				(m_dwStateChangeTime - GetTickCount()) >= (m_tFrame.iFrameAmount * m_tFrame.dwSpeed))
			{
				m_pFrameKey = L"Player_RUN";
				m_dwStateChangeTime = 0;
				m_eCurState = OBJST_RUN;
			}
		}
		else
		{
			m_pFrameKey = L"Player_RUN";			// ������Ű�� ���� = "�޸�����(��������)"
			m_eCurState = OBJST_RUN;
		}

		m_tInfo.fX += m_fSpeed;
		if (m_iTimeMode == 2) m_fTp -= 0.05f;
		m_isStretch = false;
		m_fAngle = 0.f;
		return;
	}


	// ********************








	// ********************
	// ** �ƹ� Ű�� ������ ���� IDLE �� ����
	// ** (�ٸ� Ű�� ���� ��쿡�� return���� ����ó����)
	// ********************

	// ���� ���°� RUN or RUNSTART or RUNCHANGE �̾��ٸ�
	if (m_ePreState == OBJST_RUN ||
		m_ePreState == OBJST_RUNSTART ||
		m_ePreState == OBJST_RUNCHANGE)
	{
		m_dwStateChangeTime = GetTickCount();				// Ÿ�̸� üũ
		if (m_isStretch)	m_pFrameKey = L"Player_RUNEND_R";	// ������Ű�� ���� = "�޸��� ��"
		else				m_pFrameKey = L"Player_RUNEND";
		m_eCurState = OBJST_RUNEND;
	}


	// ����� �ٲ�� Ÿ�̸Ӱ� ���ư��� ���̾���, �ش� �������� �������� ������ ����
	if (m_dwStateChangeTime != 0)
	{
		if ((m_tFrame.iFrameCur == (m_tFrame.iFrameAmount - 1)) &&
			(m_dwStateChangeTime - GetTickCount()) >= (m_tFrame.iFrameAmount * m_tFrame.dwSpeed))
		{
			if (m_isStretch)	m_pFrameKey = L"Player_IDLE_R";
			else				m_pFrameKey = L"Player_IDLE";
			m_dwStateChangeTime = 0;
			m_eCurState = OBJST_IDLE;
		}
	}
	else
	{
		if (m_isStretch)	m_pFrameKey = L"Player_IDLE_R";
		else				m_pFrameKey = L"Player_IDLE";
		m_eCurState = OBJST_IDLE;
	}
}

void CPlayer::Jump()
{
	// ���� ����
	//
	// 1. ������ �����ٸ�, m_isJumping �� true�� ��
	// 2. �׸��� ���� ���, ���� Ƣ������� ���⸦ Ư�� ������ ���� (VelocityY = 30.f �̷� ��)
	// 3. �������̶�� �� �����Ӹ��� �߷°��ӵ��� ���� (������ 9.8m/s�̳�, ���ӿ� �°� ���� ����)
	// 4. ��, �ִ� �ӵ��� ������ �ξ� �ٴ� �浹 ������ �����ϰ� ��
	
	// https://www.desmos.com/calculator/ps70idpqw0
	// ���� �̷� �Ŀ��� x ����� �ٲٸ� �¿� ���̰�, y ����� �ٲٸ� ���� ���̰� �ٲ�
	// y ����� x ����� ���� ��� ��쿡 ��������?

	// �������̶��,
	//
	// 1. ������ ���� Ž��
	// 2. ��, ���� Ÿ���� �� �� �κ��� ���� �ǹ�. (�»�, ��� �� ���� �մ� ��)
	// 3. �÷��̾��� ���� �������� m_tInfo.x, m_tInfo.y ��� ����.
	//
	// 4. ���� �÷��̾��� �������� ���� x���̵�, y�ຸ�� ���� ���� �־�� ��.`
	// 5. ���� �����, �� �߿��� ���� ���ƾ� ��
	// 6. �÷��̾��� �������� �������� ������ ���ų� �ణ ���ٸ�, �ش� ���� y������ ���� ����
	// 7. ���� �Ϸ��ߴٸ�, isJumping = false �� ��ȯ
	

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
			std::cout << "[INFO][CPlayer::Jump] Player Pos : (X : " << m_tInfo.fX << ", Y : " << m_tInfo.fY << ")" << std::endl;
		
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
				m_tInfo.fY + fMargin/4 >= fPlayerPosY)
			{
				m_isJumping = false;
				m_fVelocityY = 0;
			}
			//m_eCurState = OBJST_IDLE;
		}
			
	}


	// �⺻���� �¿�� �浹�� �ٸ��� �ؾ� �ϳ�?
}

void CPlayer::Camera_Offset()
{
}

void CPlayer::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case OBJ_STATE::OBJST_IDLE:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 80;
			break;

		case OBJ_STATE::OBJST_RUN:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 40;
			break;

		case OBJ_STATE::OBJST_RUNSTART:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 40;
			break;

		case OBJ_STATE::OBJST_RUNEND:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 40;
			break;

		case OBJ_STATE::OBJST_RUNCHANGE:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 40;
			break;

		case OBJ_STATE::OBJST_JUMP:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 80;
			break;

		case OBJ_STATE::OBJST_DOWN:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 100;
			break;

		case OBJ_STATE::OBJST_ACTION1:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 60;
			break;
		case OBJ_STATE::OBJST_ACTION2:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 60;
			break;
		case OBJ_STATE::OBJST_ACTION3:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 60;
			break;
		case OBJ_STATE::OBJST_ACTION4:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 60;
			break;

		case OBJ_STATE::OBJST_RUN_ATTACK1:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 60;
			break;
		case OBJ_STATE::OBJST_RUN_ATTACK2:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 60;
			break;
		case OBJ_STATE::OBJST_RUN_ATTACK3:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 60;
			break;
		case OBJ_STATE::OBJST_RUN_ATTACK4:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 60;
			break;

		case OBJ_STATE::OBJST_JUMP_ATTACK1:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 30;
			break;

		case OBJ_STATE::OBJST_DAMAGED:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 60;
			break;

		case OBJ_STATE::OBJST_GLIDE:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 60;
			break;

		//case CPlayer::HIT:
		//	m_tFrame.iFrameStart = 0;
		//	m_tFrame.iFrameEnd = 1;
		//	m_tFrame.iMotion = 3;
		//	m_tFrame.dwTime = GetTickCount();
		//	m_tFrame.dwSpeed = 200;
		//	break;

		//case CPlayer::DEAD:
		//	m_tFrame.iFrameStart = 0;
		//	m_tFrame.iFrameEnd = 3;
		//	m_tFrame.iMotion = 4;
		//	m_tFrame.dwTime = GetTickCount();
		//	m_tFrame.dwSpeed = 200;
		//	break;
		}

		m_ePreState = m_eCurState;
		m_tFrame.iFrameCur = 0;
	}
}

void CPlayer::LoadImages()
{
	// �̹��� �ҷ�����

	// ..Stretch�� ���� �ӽ� �̹���
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/_Temp_Image/Stretch.bmp", L"Player_Stretch");
	//FRAME_PROP tPlayer_Stretch = { 128, 96 };
	//CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_Stretch, L"Player_Stretch");

	// ..�÷��̾� �¿� �̵�
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_stop/player_stop.bmp", L"Player_IDLE");				// 64_64_X5Y5_21
	FRAME_PROP tPlayer_IDLE = { 64*2, 64*2, 5, 5, 21, 5 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_IDLE, L"Player_IDLE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_stop/player_stop_R.bmp", L"Player_IDLE_R");				// 64_64_X5Y5_21
	FRAME_PROP tPlayer_IDLE_R = { 64*2, 64*2, 5, 5, 21, 5 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_IDLE, L"Player_IDLE_R");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_run_start/player_run_start.bmp", L"Player_RUNSTART");	// 96_64_X3Y3_9
	FRAME_PROP tPlayer_RUNSTART = { 96*2, 64*2, 3, 3, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_RUNSTART, L"Player_RUNSTART");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_run_start/player_run_start_R.bmp", L"Player_RUNSTART_R");	// 96_64_X3Y3_9
	FRAME_PROP tPlayer_RUNSTART_R = { 96*2, 64*2, 3, 3, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_RUNSTART_R, L"Player_RUNSTART_R");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_run/player_run.bmp", L"Player_RUN");					// 96_64_X4Y4_16
	FRAME_PROP tPlayer_RUN = { 96*2, 64*2, 4, 4, 16 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_RUN, L"Player_RUN");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_run/player_run_R.bmp", L"Player_RUN_R");					// 96_64_X4Y4_16
	FRAME_PROP tPlayer_RUN_R = { 96*2, 64*2, 4, 4, 16 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_RUN_R, L"Player_RUN_R");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_run_stop/player_run_stop.bmp", L"Player_RUNEND");		// 64_64_X5
	FRAME_PROP tPlayer_RUNEND = { 64*2, 64*2, 5, 1, 5 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_RUNEND, L"Player_RUNEND");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_run_stop/player_run_stop_R.bmp", L"Player_RUNEND_R");		// 64_64_X5
	FRAME_PROP tPlayer_RUNEND_R = { 64*2, 64*2, 5, 1, 5 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_RUNEND_R, L"Player_RUNEND_R");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_run_back/player_run_back.bmp", L"Player_RUNCHANGE");	// 96_64_X3Y3_9
	FRAME_PROP tPlayer_RUNCHANGE = { 96*2, 64*2, 3, 3, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_RUNCHANGE, L"Player_RUNCHANGE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_run_back/player_run_back_R.bmp", L"Player_RUNCHANGE_R");	// 96_64_X3Y3_9
	FRAME_PROP tPlayer_RUNCHANGE_R = { 96*2, 64*2, 3, 3, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_RUNCHANGE_R, L"Player_RUNCHANGE_R");

	// ..�÷��̾� ����
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_jump/player_jump.bmp", L"Player_JUMP");
	FRAME_PROP tPlayer_JUMP = { 64 * 2, 64 * 2, 2, 1, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_JUMP, L"Player_JUMP");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_jump/player_jump_R.bmp", L"Player_JUMP_R");
	FRAME_PROP tPlayer_JUMP_R = { 64 * 2, 64 * 2, 2, 1, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_JUMP_R, L"Player_JUMP_R");

	// ..�÷��̾� Ȱ��
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_gliding/player_gliding.bmp", L"Player_GLIDE");
	FRAME_PROP tPlayer_GLIDE = { 96 * 2, 64 * 2, 1, 6, 6, 3 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_GLIDE, L"Player_GLIDE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_gliding/player_gliding_R.bmp", L"Player_GLIDE_R");
	FRAME_PROP tPlayer_GLIDE_R = { 96 * 2, 64 * 2, 1, 6, 6, 3 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_GLIDE_R, L"Player_GLIDE_R");

	// ..�÷��̾� �ɱ�
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_down/player_down.bmp", L"Player_DOWN");				// 64_64_X4
	FRAME_PROP tPlayer_DOWN = { 64 * 2, 64 * 2, 4, 1, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_DOWN, L"Player_DOWN");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_down/player_down_R.bmp", L"Player_DOWN_R");				// 64_64_X4
	FRAME_PROP tPlayer_DOWN_R = { 64 * 2, 64 * 2, 4, 1, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_DOWN_R, L"Player_DOWN_R");

	// ..�÷��̾� ����
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_action/player_action.bmp", L"Player_ACTION");
	FRAME_PROP tPlayer_ACTION = { 128 * 2, 64 * 2, 3, 3, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_ACTION, L"Player_ACTION");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_action2/player_action2.bmp", L"Player_ACTION2");
	FRAME_PROP tPlayer_ACTION2 = { 128 * 2, 64 * 2, 1, 9, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_ACTION2, L"Player_ACTION2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_action3/player_action3.bmp", L"Player_ACTION3");
	FRAME_PROP tPlayer_ACTION3 = { 128 * 2, 64 * 2, 1, 9, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_ACTION3, L"Player_ACTION3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_action4/player_action4.bmp", L"Player_ACTION4");
	FRAME_PROP tPlayer_ACTION4 = { 128 * 2, 64 * 2, 1, 9, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_ACTION4, L"Player_ACTION4");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_action/player_action_R.bmp", L"Player_ACTION_R");
	FRAME_PROP tPlayer_ACTION_R = { 128 * 2, 64 * 2, 3, 3, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_ACTION_R, L"Player_ACTION_R");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_action2/player_action2_R.bmp", L"Player_ACTION2_R");
	FRAME_PROP tPlayer_ACTION2_R = { 128 * 2, 64 * 2, 1, 9, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_ACTION2_R, L"Player_ACTION2_R");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_action3/player_action3_R.bmp", L"Player_ACTION3_R");
	FRAME_PROP tPlayer_ACTION3_R = { 128 * 2, 64 * 2, 1, 9, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_ACTION3_R, L"Player_ACTION3_R");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_action4/player_action4_R.bmp", L"Player_ACTION4_R");
	FRAME_PROP tPlayer_ACTION4_R = { 128 * 2, 64 * 2, 1, 9, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_ACTION4_R, L"Player_ACTION4_R");

	// ..�÷��̾� ���� (�޸��� ��)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_run_attack/player_run_attack.bmp", L"Player_RUN_ATTACK");
	FRAME_PROP tPlayer_RUN_ATTACK = { 160 * 2, 64 * 2, 2, 5, 10 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_RUN_ATTACK, L"Player_RUN_ATTACK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_run_attack2/player_run_attack2.bmp", L"Player_RUN_ATTACK2");
	FRAME_PROP tPlayer_RUN_ATTACK2 = { 128 * 2, 64 * 2, 1, 9, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_RUN_ATTACK2, L"Player_RUN_ATTACK2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_run_attack3/player_run_attack3.bmp", L"Player_RUN_ATTACK3");
	FRAME_PROP tPlayer_RUN_ATTACK3 = { 160 * 2, 64 * 2, 1, 9, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_RUN_ATTACK3, L"Player_RUN_ATTACK3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_run_attack4/player_run_attack4.bmp", L"Player_RUN_ATTACK4");
	FRAME_PROP tPlayer_RUN_ATTACK4 = { 128 * 2, 64 * 2, 1, 9, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_RUN_ATTACK4, L"Player_RUN_ATTACK4");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_run_attack/player_run_attack_R.bmp", L"Player_RUN_ATTACK_R");
	FRAME_PROP tPlayer_RUN_ATTACK_R = { 160 * 2, 64 * 2, 2, 5, 10 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_RUN_ATTACK_R, L"Player_RUN_ATTACK_R");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_run_attack2/player_run_attack2_R.bmp", L"Player_RUN_ATTACK2_R");
	FRAME_PROP tPlayer_RUN_ATTACK2_R = { 128 * 2, 64 * 2, 1, 9, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_RUN_ATTACK2_R, L"Player_RUN_ATTACK2_R");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_run_attack3/player_run_attack3_R.bmp", L"Player_RUN_ATTACK3_R");
	FRAME_PROP tPlayer_RUN_ATTACK3_R = { 160 * 2, 64 * 2, 1, 9, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_RUN_ATTACK3_R, L"Player_RUN_ATTACK3_R");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_run_attack4/player_run_attack4_R.bmp", L"Player_RUN_ATTACK4_R");
	FRAME_PROP tPlayer_RUN_ATTACK4_R = { 128 * 2, 64 * 2, 1, 9, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_RUN_ATTACK4_R, L"Player_RUN_ATTACK4_R");

	// �÷��̾� ���� (���� ��)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_jump_attack/player_jump_attack.bmp", L"Player_JUMP_ATTACK");
	FRAME_PROP tPlayer_JUMP_ATTACK = { 128 * 2, 64 * 2, 1, 6, 6 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_JUMP_ATTACK, L"Player_JUMP_ATTACK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_jump_attack/player_jump_attack_R.bmp", L"Player_JUMP_ATTACK_R");
	FRAME_PROP tPlayer_JUMP_ATTACK_R = { 128 * 2, 64 * 2, 1, 6, 6 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_JUMP_ATTACK_R, L"Player_JUMP_ATTACK_R");
	

	// �÷��̾� �ǰ�
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_damage/player_damage.bmp", L"Player_DAMAGE");
	FRAME_PROP tPlayer_DAMAGE = { 96 * 2, 64 * 2, 1, 12, 12 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_DAMAGE, L"Player_DAMAGE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_damage/player_damage_R.bmp", L"Player_DAMAGE_R");
	FRAME_PROP tPlayer_DAMAGE_R = { 96 * 2, 64 * 2, 1, 12, 12 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_DAMAGE_R, L"Player_DAMAGE_R");

	// �÷��̾� ������ ������ �� ����
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/UI/snail_gauge_soket/snail_gauge_soket.bmp", L"UI_SNAILSOCKET");
	FRAME_PROP tUI_SNAILSOCKET = { 32 * 2, 32 * 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tUI_SNAILSOCKET, L"UI_SNAILSOCKET");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/UI/snail_gauge/snail_gauge.bmp", L"UI_SNAILGAUGE");
	FRAME_PROP tUI_SNAILGAUGE = { 32 * 2, 32 * 2, 14, 4, 56 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tUI_SNAILGAUGE, L"UI_SNAILGAUGE");
}

int CPlayer::Get_Stat(PLAYERSTAT _statType)
{
	switch (_statType)
	{
	case CPlayer::HP:				return m_iHp;
	case CPlayer::MAXHP:			return m_iMaxHp;
	case CPlayer::MP:				return m_iMp;
	case CPlayer::MAXMP:			return m_iMaxMp;
	case CPlayer::TP:				return m_fTp;
	case CPlayer::GOLD:				return m_iGold;
	case CPlayer::ATK:				return (int)m_fAtk;
	case CPlayer::KNIFE:			return m_iKnife;
	case CPlayer::ISGETWATCH:		return (int)m_isGetWatch;
	case CPlayer::TIMEMODE:			return m_iTimeMode;
	}
}

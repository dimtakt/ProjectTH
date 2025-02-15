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

CPlayer::CPlayer() :
	m_eCurState(OBJST_IDLE),
	m_ePreState(OBJST_END),
	m_dwTime(GetTickCount()),
	m_dwStateChangeTime(0),
	m_isStartStage(false)
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
	m_iTp = 85;
	m_iGold = 0;
	m_fAtk = 5.2;
	m_iKnife = 18;
	m_isGetWatch = 0;

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


	CCameraMgr::Get_Instance()->Set_Target(this);
}

int CPlayer::Update()
{
	if (m_isDead)
		return OBJ_DEAD;

	Key_Input();

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
	// ** ���� Ű (���� ���� �ƴ� ������ ����)
	// ********************
	if (CKeyMgr::Get_Instance()->Key_Down('X') && !m_isJumping)
	{
		// �ö󰡴� ��, �������� ���� ��� ���ǵ� ���� �������� ��
		// �� ������ �߿� õõ�� �������� Ȱ���ϴ� ���ǵ� ���� �������� ��
		// later
		if (m_isStretch)	m_pFrameKey = L"Player_Jump_R";
		else				m_pFrameKey = L"Player_Jump";

		m_dwStateChangeTime = 0;
		m_eCurState = OBJST_JUMP;
		m_fVelocityY = -20.f;	// �ӽð�. Jump()

		m_isJumping = true;
	}

	// ********************
	// ** ���� Ű
	// ********************
	if (	CKeyMgr::Get_Instance()->Key_Down('Z'))
	{
		float iKnifeSpace = 20.f;

		// �Ѿ��� �ʱ� ��ġ ���
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

		bulletY -= iKnifeSpace*2;
		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYERBULLET, CAbstractFactory<CPlayerBullet>::Create(bulletX, bulletY - m_tInfo.fCY / 2, m_fAngle));
		std::cout << "[INFO][CPlayer::Key_Input] " << "Bullet Created on..  " << bulletX << ", " << bulletY - m_tInfo.fCY / 2 << std::endl;


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
		m_isStretch = true;
		m_fAngle = 180.f;
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
			if (m_isStretch)	m_pFrameKey = L"Player_IDLE_R";	// ������Ű�� ���� = "�޸��� ��"
			else				m_pFrameKey = L"Player_IDLE";
			m_dwStateChangeTime = 0;
			m_eCurState = OBJST_IDLE;
		}
	}
	else
	{
		if (m_isStretch)	m_pFrameKey = L"Player_IDLE_R";	// ������Ű�� ���� = "�޸��� ��"
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
	



	// y���� ��� �����ϴ� �� �ƴ϶�, ������ ������ �����ϰ�, ���ǿ� ���� �ش� ���� ���� ���� �����ϴ� ������ �ٲ�� ��
	// 444��° �� Ȯ���غ��� ������ �ִ� ������ �����ؾ� ��
	float fPlayerPosY = m_tInfo.fY;			// �÷��̾��� Y�� ����
	float fMargin = 20.f;					// �ڿ������� ������ ���� ������

	float fMaxVelocityY = 20;				// �ִ� ���ϼӵ� ����
	float fGravityConst = GRAVITY * 0.12;	// ���� �ڿ������� �߷°��ӵ� �� ���

	// �÷��̾� Y�� �ӵ�. 30 �ȳѰ� ����
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

		//case CPlayer::ATTACK:
		//	m_tFrame.iFrameStart = 0;
		//	m_tFrame.iFrameEnd = 5;
		//	m_tFrame.iMotion = 2;
		//	m_tFrame.dwTime = GetTickCount();
		//	m_tFrame.dwSpeed = 200;
		//	break;

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

	// ..�÷��̾� �ɱ�
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_down/player_down.bmp", L"Player_DOWN");				// 64_64_X4
	FRAME_PROP tPlayer_DOWN = { 64*2, 64*2, 4, 1, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_DOWN, L"Player_DOWN");

	// ..�÷��̾� ����
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_action/player_action.bmp", L"Player_ACTION");				// 64_64_X4
	FRAME_PROP tPlayer_DOWN = { 128 * 2, 64 * 2, 1, 6, 1 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_DOWN, L"Player_ACTION");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_action/player_action.bmp", L"Player_ACTION2");				// 64_64_X4
	FRAME_PROP tPlayer_DOWN = { 160 * 2, 64 * 2, 1, 6, 1 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_DOWN, L"Player_ACTION2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_action/player_action.bmp", L"Player_ACTION3");				// 64_64_X4
	FRAME_PROP tPlayer_DOWN = { 160 * 2, 64 * 2, 1, 6, 1 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_DOWN, L"Player_ACTION3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_action/player_action.bmp", L"Player_ACTION4");				// 64_64_X4
	FRAME_PROP tPlayer_DOWN = { 128 * 2, 64 * 2, 1, 6, 1 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_DOWN, L"Player_ACTION4");

}

int CPlayer::Get_Stat(PLAYERSTAT _statType)
{
	switch (_statType)
	{
	case CPlayer::HP:				return m_iHp;
	case CPlayer::MAXHP:			return m_iMaxHp;
	case CPlayer::MP:				return m_iMp;
	case CPlayer::MAXMP:			return m_iMaxMp;
	case CPlayer::TP:				return m_iTp;
	case CPlayer::GOLD:				return m_iGold;
	case CPlayer::ATK:				return (int)m_fAtk;
	case CPlayer::KNIFE:			return m_iKnife;
	case CPlayer::ISGETWATCH:		return m_isGetWatch;
	case CPlayer::TIMEMODE:			return m_iTimeMode;
	}
}

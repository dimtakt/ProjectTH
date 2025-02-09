#include "pch.h"
#include "CPlayer.h"
#include "CAbstractFactory.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"
#include "CBmpMgr.h"
#include "CSpritePropertyMgr.h"

CPlayer::CPlayer() :
	m_eCurState(OBJST_IDLE),
	m_ePreState(OBJST_END),
	m_dwTime(GetTickCount())
{
	//ZeroMemory(&m_tPosin, sizeof(POINT));
	ZeroMemory(&m_tInfo, sizeof(INFO));
}

CPlayer::~CPlayer()
{
	Release();
}

// ----

void CPlayer::Initialize()
{

	// ���� �ʱ�ȭ
	m_tInfo = { 100.f, WINCY / 2.f, 200.f, 200.f };	// 100, 360, 200, 200
	m_fSpeed = 8.f;
	m_fAngle = 0.f;
	m_isJump = false;
	m_fTime = 0.f;
	m_fJumpSpeed = 20.f;


	// �̹��� �ҷ�����

	// ..Stretch�� ���� �ӽ� �̹���
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/_Temp_Image/Stretch.bmp", L"Player_Stretch");
	FRAME_PROP tPlayer_Stretch = { 128, 96 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_Stretch, L"Player_Stretch");

	// ..�÷��̾� �¿� �̵�
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_stop/player_stop.bmp", L"Player_IDLE");				// 64_64_X5Y5_21
	FRAME_PROP tPlayer_IDLE = { 64, 64, 5, 5, 21, 5 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_IDLE, L"Player_IDLE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_stop/player_stop_R.bmp", L"Player_IDLE_R");				// 64_64_X5Y5_21
	FRAME_PROP tPlayer_IDLE_R = { 64, 64, 5, 5, 21, 5 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_IDLE, L"Player_IDLE_R");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_run_start/player_run_start.bmp", L"Player_RUNSTART");	// 96_64_X3Y3_9
	FRAME_PROP tPlayer_RUNSTART = { 96, 64, 3, 3, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_RUNSTART, L"Player_RUNSTART");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_run/player_run.bmp", L"Player_RUN");					// 96_64_X4Y4_16
	FRAME_PROP tPlayer_RUN = { 96, 64, 4, 4, 16 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_RUN, L"Player_RUN");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_run/player_run_R.bmp", L"Player_RUN_R");					// 96_64_X4Y4_16
	FRAME_PROP tPlayer_RUN_R = { 96, 64, 4, 4, 16 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_RUN_R, L"Player_RUN_R");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_run_stop/player_run_stop.bmp", L"Player_RUNEND");		// 64_64_X5
	FRAME_PROP tPlayer_RUNEND = { 64, 64, 5, 1, 5 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_RUNEND, L"Player_RUNEND");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_run_back/player_run_back.bmp", L"Player_RUNCHANGE");	// 96_64_X3Y3_8
	FRAME_PROP tPlayer_RUNCHANGE = { 96, 64, 3, 3, 8 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_RUNCHANGE, L"Player_RUNCHANGE");

	// ..�÷��̾� �ɱ�
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_down/player_down.bmp", L"Player_DOWN");				// 64_64_X4
	FRAME_PROP tPlayer_DOWN = { 64, 64, 4, 1, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_DOWN, L"Player_DOWN");

	// ..�÷��̾� ����
	//..


	m_pFrameKey = L"Player_IDLE";
	m_tFramePropCur = tPlayer_IDLE;

	// �������� ����, �� �ε���, ��� ��ȣ ���� ����
	m_tFrame.iFrameCur = 0;
	Set_FrameProperty(tPlayer_IDLE);

	m_tFrame.dwSpeed = 100;				// ������ ��ȯ �ӵ�
	m_tFrame.dwTime = GetTickCount();	// ����� �ٲ���� �� �帥 �ð��� ����

	m_eRender = RENDER_GAMEOBJECT;
}

int CPlayer::Update()
{
	if (m_isDead)
		return OBJ_DEAD;

	Key_Input();

	m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(m_pFrameKey);
	Set_FrameProperty(m_tFramePropCur);	// ���� �����ӿ� �� ���¿� �´� ������ �ݿ�
	Set_Scale(m_tFramePropCur.iCX, m_tFramePropCur.iCY);

	// �÷��̾�� ��ǥ�κ��� ���� ������ �������Ѿ� �� (�������Ƿ�)
	__super::Update_Rect_UpStand2X();	// 2��ũ�� ���� ���� ��ǥ����




	std::cout << "Player Rect : \t" << m_tRect.left << "\t" << m_tRect.top << "\t" << m_tRect.right << "\t" << m_tRect.bottom << std::endl;
	std::cout << "Player Scale: \t" << m_tInfo.fCX << "\t" << m_tInfo.fCY << std::endl;
	std::cout << "Player Info : \t" << m_tInfo.fX << "\t" << m_tInfo.fY << std::endl;



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
		 				m_tInfo.fCX * 2,	// ���� ���� ����, ���� ����.
		 				m_tInfo.fCY * 2,
		 				hMemDC,					// ��Ʈ���� ������ �ִ� DC
						m_tInfo.fCX * ((m_tFrame.iFrameCur) % (m_tFrame.iFrameMaxX)),	// ����Ϸ��� ��Ʈ����Ʈ �̹��� �������� ��ǥ
						m_tInfo.fCY * ((m_tFrame.iFrameCur) / (m_tFrame.iFrameMaxY)),
						m_tInfo.fCX,	// ��Ʈ���� ����� ����, ���� ����
		 				m_tInfo.fCY,
		 				RGB(255, 0, 255));		// ������ ����


}

void CPlayer::Release()
{	
}

// ----

void CPlayer::Key_Input()
{
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		m_tInfo.fX -= m_fSpeed;
		m_pFrameKey = L"Player_RUN_R";
		m_eCurState = OBJST_RUN;
		m_isStretch = true;
	}

	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		m_tInfo.fX += m_fSpeed;
		m_pFrameKey = L"Player_RUN";
		m_eCurState = OBJST_RUN;
		m_isStretch = false;
	}
	else
	{
		if (m_isStretch)
			m_pFrameKey = L"Player_IDLE_R";
		else
			m_pFrameKey = L"Player_IDLE";

		m_eCurState = OBJST_IDLE;
	}
}

void CPlayer::Jump()
{
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
			m_tFrame.dwSpeed = 100;
			break;

		case OBJ_STATE::OBJST_RUN:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 50;
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

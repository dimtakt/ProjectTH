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

	// 변수 초기화
	m_tInfo = { 100.f, WINCY / 2.f, 200.f, 200.f };	// 100, 360, 200, 200
	m_fSpeed = 8.f;
	m_fAngle = 0.f;
	m_isJump = false;
	m_fTime = 0.f;
	m_fJumpSpeed = 20.f;


	// 이미지 불러오기

	// ..Stretch를 위한 임시 이미지
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/_Temp_Image/Stretch.bmp", L"Player_Stretch");
	FRAME_PROP tPlayer_Stretch = { 128, 96 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_Stretch, L"Player_Stretch");

	// ..플레이어 좌우 이동
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

	// ..플레이어 앉기
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_down/player_down.bmp", L"Player_DOWN");				// 64_64_X4
	FRAME_PROP tPlayer_DOWN = { 64, 64, 4, 1, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_DOWN, L"Player_DOWN");

	// ..플레이어 공격
	//..


	m_pFrameKey = L"Player_IDLE";
	m_tFramePropCur = tPlayer_IDLE;

	// 프레임의 시작, 끝 인덱스, 모션 번호 등의 저장
	m_tFrame.iFrameCur = 0;
	Set_FrameProperty(tPlayer_IDLE);

	m_tFrame.dwSpeed = 100;				// 프레임 전환 속도
	m_tFrame.dwTime = GetTickCount();	// 모션이 바뀌었을 때 흐른 시간을 저장

	m_eRender = RENDER_GAMEOBJECT;
}

int CPlayer::Update()
{
	if (m_isDead)
		return OBJ_DEAD;

	Key_Input();

	m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(m_pFrameKey);
	Set_FrameProperty(m_tFramePropCur);	// 현재 프레임에 현 상태에 맞는 정보를 반영
	Set_Scale(m_tFramePropCur.iCX, m_tFramePropCur.iCY);

	// 플레이어는 좌표로부터 위쪽 범위에 렌더시켜야 함 (서있으므로)
	__super::Update_Rect_UpStand2X();	// 2배크기 렌더 기준 좌표보정




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
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // 최종적으로 렌더시킬 좌표.

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	

	// ** 여기서 좌표는 1배 기준으로 잡아놓고 렌더는 2배 크기로 하니까 안맞았지..
	// 그래서 2배크기 렌더용 Update_Rect ... 2X 함수들을 새로 만듦.

	GdiTransparentBlt(	hDC,					// 최종적으로 그릴 DC
		 				iOutX,					// 복사받을 위치 X, Y좌표
		 				iOutY,
						//iOutX - m_tInfo.fCX/2,					// 복사받을 위치 X, Y좌표
						//iOutY - m_tInfo.fCY,
		 				m_tInfo.fCX * 2,	// 복사 받을 가로, 세로 길이.
		 				m_tInfo.fCY * 2,
		 				hMemDC,					// 비트맵을 가지고 있는 DC
						m_tInfo.fCX * ((m_tFrame.iFrameCur) % (m_tFrame.iFrameMaxX)),	// 출력하려는 스트라이트 이미지 내에서의 좌표
						m_tInfo.fCY * ((m_tFrame.iFrameCur) / (m_tFrame.iFrameMaxY)),
						m_tInfo.fCX,	// 비트맵을 출력할 가로, 세로 길이
		 				m_tInfo.fCY,
		 				RGB(255, 0, 255));		// 제거할 색상


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

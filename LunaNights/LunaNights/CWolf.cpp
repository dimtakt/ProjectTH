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

	m_tFrame.dwSpeed = 100;				// 프레임 전환 속도
	m_tFrame.dwTime = GetTickCount();	// 모션이 바뀌었을 때 흐른 시간을 저장
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



// 중력도 받아야함

int CWolf::Update()
{
	if (m_isDead)
		return OBJ_DEAD;

	// 이미지 파일 크기에 맞게 크기 구성
	m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(m_pFrameKey);
	Set_FrameProperty(m_tFramePropCur);	// 현재 프레임에 현 상태에 맞는 정보를 반영
	Set_Scale(m_tFramePropCur.iCX, m_tFramePropCur.iCY);


	// 이미지 파일 크기에 맞게 충돌판정 구성
	m_tCollideInfo = { m_tInfo.fX, m_tInfo.fY - m_tInfo.fCY * 0.25f, m_tInfo.fCX * 0.7f, m_tInfo.fCY * 0.5f};


	__super::Update_Rect_UpStand();




	// 플레이어 위치를 받아온 뒤 행동 패턴 진행 (화면 안에 있을 때만)

	int iOutX = 0, iOutY = 0;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY);

	if (!((m_tInfo.fX + m_tInfo.fCX < 0 - iOutX || m_tInfo.fX - iOutY - m_tInfo.fCX > WINCX - iOutX) ||
		(m_tInfo.fY + m_tInfo.fCY < 0 - iOutY || m_tInfo.fY - iOutY - m_tInfo.fCY > WINCY - iOutY)))
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
		if (pPlayer == nullptr)
			return OBJ_NOEVENT;
		INFO tPlayerInfo = *pPlayer->Get_Info();

		// 1. 일정 거리(iDistToPlayer)까지 접근 (도약 준비 변수가 false 일 때만)
		if (!m_isAttackReady && !m_isJumping)
		{
			int iDistToPlayer = 250;
			float fDistanceX = abs(abs(tPlayerInfo.fX) - abs(m_tInfo.fX));
			m_eCurState = OBJST_RUN;
			if (m_isStretch)	{ m_pFrameKey = L"Wolf_Run"; }
			else				{ m_pFrameKey = L"Wolf_Run_R"; }

			if (iDistToPlayer <= fDistanceX) // 거리가 멀다면 붙음
			{
				if (tPlayerInfo.fX > m_tInfo.fX)	{ m_tInfo.fX += m_fSpeed; m_isStretch = false; }
				else								{ m_tInfo.fX -= m_fSpeed; m_isStretch = true;  }
			}
			else
			{
				// 도약 준비 변수를 true로.
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
			if (((dwCurrentTime - dwAttackReady) >= iAttackDelay) && dwAttackReady != 0) // unsigned 값임에 유의. 음수가 될 상황이 생기면 안됨
			{
				// 플레이어를 향해 30도가량 높이로 점프함
				// m_eCurState도 변경해야함
				// 그 뒤 dw변수 초기화함
				// 착지까지 했으면 다시 도약 준비 변수 두 개 false로 해제함
				// laterEdit
				m_eCurState = OBJST_JUMP;
				//m_pFrameKey = L"Wolf_Jump";

				dwAttackReady = 0;
				//m_isJumping = true;

				m_isJumping = true;

				// Y 값은 점프 할 때 처럼 주고,
				// X 값은 그냥 점프 중 계속 플레이어를 향해 이동하게 하면 되려나 
				m_fVelocityY = -15.f;
				CSoundMgr::Get_Instance()->StopSound(SOUND_WOLF);
				CSoundMgr::Get_Instance()->PlaySound(L"s1006_cat.wav", SOUND_WOLF, 0.2f);
				
				m_isAttackReady = false;
				m_isPreAttackReady = false;

			}
		}
		
		// 도약 준비 상태가 될 때 1회만 실행하는 부분 (타이머)
		if (m_isAttackReady && !m_isPreAttackReady)
		{
			m_isPreAttackReady = true;
			dwAttackReady = GetTickCount();

			if (tPlayerInfo.fX > m_tInfo.fX)	m_isJumpDirStretch = false;
			else								m_isJumpDirStretch = true;
		}

		if (m_isJumping)
		{
			// Jump_R 이미지는 정방향이 왼쪽이기에 이미지 조건을 반대로 줌.
			if (!m_isJumpDirStretch)			{ m_tInfo.fX += m_fSpeed * 5; m_pFrameKey = L"Wolf_Jump_R"; }
			else								{ m_tInfo.fX -= m_fSpeed * 5; m_pFrameKey = L"Wolf_Jump"; }
		}
		// 2. 도약 준비 변수가 true일 때, 타이머 온
		//    이후 일정 시간 후에 30도가량 높이로 도약
		//    도약 이후, 땅에 착지할 때 (m_isJumping  이 false가 될 때)
		//    도약 준비 변수의 false화, 다시 1 패턴으로 돌아감


		
	}

	return OBJ_NOEVENT;
}

void CWolf::Late_Update()
{
	// 카메라 안에 있을때만 갱신
	int iOutX = 0, iOutY = 0;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY);

	if (!((m_tInfo.fX + m_tInfo.fCX < 0 - iOutX || m_tInfo.fX - iOutY - m_tInfo.fCX > WINCX - iOutX) ||
		(m_tInfo.fY + m_tInfo.fCY < 0 - iOutY || m_tInfo.fY - iOutY - m_tInfo.fCY > WINCY - iOutY)))
	{

		// 실질적 실행부

		Jump();
		Move_Frame();
		Motion_Change();

	}
}

void CWolf::Render(HDC hDC)
{
	// 카메라 안에 있을때만 갱신
	int iOutX = 0, iOutY = 0;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY);

	if (!((m_tInfo.fX + m_tInfo.fCX < 0 - iOutX || m_tInfo.fX - iOutY - m_tInfo.fCX > WINCX - iOutX) ||
		(m_tInfo.fY + m_tInfo.fCY < 0 - iOutY || m_tInfo.fY - iOutY - m_tInfo.fCY > WINCY - iOutY)))
	{

		// 실질적 실행부

		int iOutX = m_tRect.left;
		int iOutY = m_tRect.top;
		CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // 최종적으로 렌더시킬 좌표.

		HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

		GdiTransparentBlt(hDC,					// 최종적으로 그릴 DC
			iOutX,					// 복사받을 위치 X, Y좌표
			iOutY,
			//iOutX - m_tInfo.fCX/2,					// 복사받을 위치 X, Y좌표
			//iOutY - m_tInfo.fCY,
			m_tInfo.fCX,	// 복사 받을 가로, 세로 길이.
			m_tInfo.fCY,
			hMemDC,					// 비트맵을 가지고 있는 DC
			m_tInfo.fCX * ((m_tFrame.iFrameCur) % (m_tFrame.iFrameMaxX)),	// 출력하려는 스트라이트 이미지 내에서의 좌표
			m_tInfo.fCY * ((m_tFrame.iFrameCur) / (m_tFrame.iFrameMaxX)),
			m_tInfo.fCX,	// 비트맵을 출력할 가로, 세로 길이
			m_tInfo.fCY,
			RGB(255, 0, 255));		// 제거할 색상


	}
}

void CWolf::Release()
{
}

void CWolf::Jump()
{
	// 플레이어 Jump 함수 재사용

	float fPlayerPosY = m_tInfo.fY;			// 플레이어의 Y축 높이
	float fMargin = 20.f;					// 자연스러운 착지를 위한 마진값

	float fMaxVelocityY = 20;				// 최대 낙하속도 제한
	float fGravityConst = GRAVITY * 0.12;	// 대충 자연스러운 중력가속도 및 계수

	// 플레이어 Y축 속도. 20 안넘게 조절
	m_fVelocityY = ((m_fVelocityY + fGravityConst) >= fMaxVelocityY) ?
		fMaxVelocityY : m_fVelocityY + fGravityConst;



	float fDiffY;							// 전 프레임, 현 프레임 간의 Y축 차이

	if (m_tPrePos.x == 0 && m_tPrePos.y == 0)
		fDiffY = 0;
	else
		fDiffY = m_tInfo.fY - m_tPrePos.y;

	m_tPrePos = { m_tInfo.fX, m_tInfo.fY };	// 이전 프레임 정보 저장


	bool isFoundLine = false;
	if (m_isStartStage)						// 스테이지 시작 1회에 한해 실행
	{
		m_isStartStage = false;
		isFoundLine = CTileCollisionMgr::Collision_Line(fPlayerPosY, m_tInfo.fX);
		if (!isFoundLine)
		{
			m_isJumping = true;
			m_eCurState = OBJST_JUMP;
		}
	}


	if (!m_isJumping)	// 점프 X
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
			// 찾은 선이 있기 한데, 범위에서 심하게 차이날 때.
			// (즉 플레이어 아래에 바닥이 있긴 하나 발이 떨어져있을 때)
			// 플레이어가 지형에서 떨어졌다고 판정, 점프상태 On.
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
	else				// 점프 O
	{
		m_tInfo.fY += m_fVelocityY;

		if (fDiffY > 0)
		{
			isFoundLine = CTileCollisionMgr::Collision_Line(fPlayerPosY, m_tInfo.fX); // 여기서 순간이동함. 플레이어와의 높이 차이가 고려되지 않는 듯.

			// 상하단에 약간의 여윳값을 주어 선을 자연스럽게 타도록 함
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
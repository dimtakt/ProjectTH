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

	// 변수 초기화
	m_tInfo = { 100.f, WINCY / 2.f + 100.f, 64.f, 64.f };
	m_fSpeed = 6.f;
	m_fAngle = 0.f;
	m_isJumping = false;
	m_fTime = 0.f;
	m_fJumpSpeed = 20.f;
	m_fPosinLength = 50.f;
	m_fAngle = 0.f;

	// 변수-스탯 초기화
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


	// BmpMgr 을 통한 이미지 로드, 난잡해서 따로 뺐음
	LoadImages();


	FRAME_PROP tPlayer_IDLE = { 64*2, 64*2, 5, 5, 21, 5 };
	m_pFrameKey = L"Player_IDLE";
	m_tFramePropCur = tPlayer_IDLE;

	// 프레임의 시작, 끝 인덱스, 모션 번호 등의 저장
	m_tFrame.iFrameCur = 0;
	Set_FrameProperty(tPlayer_IDLE);

	m_tFrame.dwSpeed = 100;				// 프레임 전환 속도
	m_tFrame.dwTime = GetTickCount();	// 모션이 바뀌었을 때 흐른 시간을 저장

	m_eRender = RENDER_GAMEOBJECT;


	CCameraMgr::Get_Instance()->Set_Target(this);
}

int CPlayer::Update()
{
	if (m_isDead)
		return OBJ_DEAD;

	Key_Input();

	m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(m_pFrameKey);
	Set_FrameProperty(m_tFramePropCur);	// 현재 프레임에 현 상태에 맞는 정보를 반영
	Set_Scale(m_tFramePropCur.iCX, m_tFramePropCur.iCY);

	m_tCollideInfo = {	m_tInfo.fX, 
						m_tInfo.fY - 64,
						48,
						128 };

	// 플레이어는 좌표로부터 위쪽 범위에 렌더시켜야 함 (서있으므로)
	__super::Update_Rect_UpStand();	// 2배크기 렌더 기준 좌표보정

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
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // 최종적으로 렌더시킬 좌표.

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	

	// ** 여기서 좌표는 1배 기준으로 잡아놓고 렌더는 2배 크기로 하니까 안맞았지..
	// 그래서 2배크기 렌더용 Update_Rect ... 2X 함수들을 새로 만듦.

	GdiTransparentBlt(	hDC,					// 최종적으로 그릴 DC
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


	// 충돌 기준 확인용
	// 빨간 색은 Rect 기준, 초록색은 Info 기준 위치.

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
	// ** 점프 키 (점프 중이 아닐 때에만 가능)
	// ********************
	if (CKeyMgr::Get_Instance()->Key_Down('X') && !m_isJumping)
	{
		// 올라가는 중, 내려가는 중의 모션 조건도 따로 만들어줘야 함
		// 꾹 누르는 중에 천천히 내려가며 활공하는 조건도 따로 만들어줘야 함
		// later
		if (m_isStretch)	m_pFrameKey = L"Player_Jump_R";
		else				m_pFrameKey = L"Player_Jump";

		m_dwStateChangeTime = 0;
		m_eCurState = OBJST_JUMP;
		m_fVelocityY = -20.f;	// 임시값. Jump()

		m_isJumping = true;
	}

	// ********************
	// ** 공격 키
	// ********************
	if (	CKeyMgr::Get_Instance()->Key_Down('Z'))
	{
		float iKnifeSpace = 20.f;

		// 총알의 초기 위치 계산
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
	// ** 왼쪽 방향키
	// ********************
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		// 이전 상태가 반대를 바라본 채로 IDLE or RUN or RUNSTART 이었다면
		if (m_ePreState == OBJST_RUN && !m_isStretch ||
			m_ePreState == OBJST_IDLE && !m_isStretch ||
			m_ePreState == OBJST_RUNSTART && !m_isStretch ||
			m_ePreState == OBJST_RUNCHANGE && !m_isStretch ||
			m_ePreState == OBJST_RUNEND && !m_isStretch )
		{
			m_dwStateChangeTime = GetTickCount();
			m_pFrameKey = L"Player_RUNCHANGE_R"; // RUNCHANGE로 변경
			m_eCurState = OBJST_RUNCHANGE;
		}
		// 이전 상태가 IDLE 이었다면
		else if (m_ePreState == OBJST_IDLE ||
				m_ePreState == OBJST_RUNEND)
		{
			m_dwStateChangeTime = GetTickCount();	// 타이머 체크
			m_pFrameKey = L"Player_RUNSTART_R";		// 프레임키와 상태 = "달리기 시작(왼쪽으로)"
			m_eCurState = OBJST_RUNSTART;
		}

		// 모션이 바뀌는 타이머가 돌아가는 중이었고, 해당 프레임이 마지막을 찍으면 원복
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
			m_pFrameKey = L"Player_RUN_R";			// 프레임키와 상태 = "달리기중(왼쪽으로)"
			m_eCurState = OBJST_RUN;
		}

		m_tInfo.fX -= m_fSpeed;
		m_isStretch = true;
		m_fAngle = 180.f;
		return;
	}

	
	// ********************
	// ** 오른쪽 방향키
	// ********************
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		// 이전 상태가 반대를 바라본 채로 IDLE or RUN or RUNSTART 이었다면
		if (m_ePreState == OBJST_RUN && m_isStretch ||
			m_ePreState == OBJST_IDLE && m_isStretch ||
			m_ePreState == OBJST_RUNSTART && m_isStretch ||
			m_ePreState == OBJST_RUNCHANGE && m_isStretch ||
			m_ePreState == OBJST_RUNEND && m_isStretch)
		{
			m_dwStateChangeTime = GetTickCount();
			m_pFrameKey = L"Player_RUNCHANGE"; // RUNCHANGE로 변경
			m_eCurState = OBJST_RUNCHANGE;
		}
		// 이전 상태가 IDLE 이었다면
		else if (m_ePreState == OBJST_IDLE||
				m_ePreState == OBJST_RUNEND)
		{
			m_dwStateChangeTime = GetTickCount();	// 타이머 체크
			m_pFrameKey = L"Player_RUNSTART";		// 프레임키와 상태 = "달리기 시작(왼쪽으로)"
			m_eCurState = OBJST_RUNSTART;
		}

		// 모션이 바뀌는 타이머가 돌아가는 중이었고, 해당 프레임이 마지막을 찍으면 원복
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
			m_pFrameKey = L"Player_RUN";			// 프레임키와 상태 = "달리기중(왼쪽으로)"
			m_eCurState = OBJST_RUN;
		}

		m_tInfo.fX += m_fSpeed;
		m_isStretch = false;
		m_fAngle = 0.f;
		return;
	}


	// ********************








	// ********************
	// ** 아무 키도 누르지 않은 IDLE 한 상태
	// ** (다른 키를 누른 경우에는 return으로 예외처리함)
	// ********************

	// 이전 상태가 RUN or RUNSTART or RUNCHANGE 이었다면
	if (m_ePreState == OBJST_RUN ||
		m_ePreState == OBJST_RUNSTART ||
		m_ePreState == OBJST_RUNCHANGE)
	{
		m_dwStateChangeTime = GetTickCount();				// 타이머 체크
		if (m_isStretch)	m_pFrameKey = L"Player_RUNEND_R";	// 프레임키와 상태 = "달리기 끝"
		else				m_pFrameKey = L"Player_RUNEND";
		m_eCurState = OBJST_RUNEND;
	}


	// 모션이 바뀌는 타이머가 돌아가는 중이었고, 해당 프레임이 마지막을 찍으면 원복
	if (m_dwStateChangeTime != 0)
	{
		if ((m_tFrame.iFrameCur == (m_tFrame.iFrameAmount - 1)) &&
			(m_dwStateChangeTime - GetTickCount()) >= (m_tFrame.iFrameAmount * m_tFrame.dwSpeed))
		{
			if (m_isStretch)	m_pFrameKey = L"Player_IDLE_R";	// 프레임키와 상태 = "달리기 끝"
			else				m_pFrameKey = L"Player_IDLE";
			m_dwStateChangeTime = 0;
			m_eCurState = OBJST_IDLE;
		}
	}
	else
	{
		if (m_isStretch)	m_pFrameKey = L"Player_IDLE_R";	// 프레임키와 상태 = "달리기 끝"
		else				m_pFrameKey = L"Player_IDLE";
		m_eCurState = OBJST_IDLE;
	}
}

void CPlayer::Jump()
{
	// 점프 구현
	//
	// 1. 점프를 눌렀다면, m_isJumping 이 true가 됨
	// 2. 그리고 점프 즉시, 위로 튀어오르는 세기를 특정 값으로 변경 (VelocityY = 30.f 이런 식)
	// 3. 점프중이라면 매 프레임마다 중력가속도를 받음 (현실은 9.8m/s이나, 게임에 맞게 배율 조정)
	// 4. 단, 최대 속도는 제한을 두어 바닥 충돌 관리에 용이하게 함
	
	// https://www.desmos.com/calculator/ps70idpqw0
	// 대충 이런 식에서 x 계수를 바꾸면 좌우 길이가, y 계수를 바꾸면 상하 길이가 바뀜
	// y 계수와 x 계수는 각각 어느 경우에 대응되지?

	// 점프중이라면,
	//
	// 1. 착지할 선을 탐색
	// 2. 단, 선은 타일의 맨 윗 부분의 선을 의미. (좌상, 우상 두 점을 잇는 선)
	// 3. 플레이어의 착지 기준점은 m_tInfo.x, m_tInfo.y 라고 가정.
	//
	// 4. 선은 플레이어의 기준점과 같은 x축이되, y축보다 낮은 곳에 있어야 함.`
	// 5. 여러 개라면, 그 중에선 가장 높아야 함
	// 6. 플레이어의 기준점이 착지예정 선보다 같거나 약간 낮다면, 해당 선의 y축으로 높이 변경
	// 7. 착지 완료했다면, isJumping = false 로 전환
	



	// y축을 즉시 대입하는 게 아니라, 복사한 변수를 대입하고, 조건에 따라 해당 값을 원본 값에 대입하는 식으로 바꿔야 함
	// 444번째 줄 확인해보고 조건을 주는 식으로 수정해야 함
	float fPlayerPosY = m_tInfo.fY;			// 플레이어의 Y축 높이
	float fMargin = 20.f;					// 자연스러운 착지를 위한 마진값

	float fMaxVelocityY = 20;				// 최대 낙하속도 제한
	float fGravityConst = GRAVITY * 0.12;	// 대충 자연스러운 중력가속도 및 계수

	// 플레이어 Y축 속도. 30 안넘게 조절
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
			std::cout << "[INFO][CPlayer::Jump] Player Pos : (X : " << m_tInfo.fX << ", Y : " << m_tInfo.fY << ")" << std::endl;
		
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
				m_tInfo.fY + fMargin/4 >= fPlayerPosY)
			{
				m_isJumping = false;
				m_fVelocityY = 0;
			}
			//m_eCurState = OBJST_IDLE;
		}
			
	}


	// 기본적인 좌우상 충돌은 다르게 해야 하나?
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
	// 이미지 불러오기

	// ..Stretch를 위한 임시 이미지
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/_Temp_Image/Stretch.bmp", L"Player_Stretch");
	//FRAME_PROP tPlayer_Stretch = { 128, 96 };
	//CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_Stretch, L"Player_Stretch");

	// ..플레이어 좌우 이동
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

	// ..플레이어 앉기
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Player/player_down/player_down.bmp", L"Player_DOWN");				// 64_64_X4
	FRAME_PROP tPlayer_DOWN = { 64*2, 64*2, 4, 1, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tPlayer_DOWN, L"Player_DOWN");

	// ..플레이어 공격
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

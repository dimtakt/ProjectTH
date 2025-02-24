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

	m_tFrame.dwSpeed = 100;				// 프레임 전환 속도
	m_tFrame.dwTime = GetTickCount();	// 모션이 바뀌었을 때 흐른 시간을 저장
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

	// 플레이어의 위치를 받아옴
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	if (pPlayer == nullptr)
		return OBJ_NOEVENT;
	INFO tPlayerInfo = *pPlayer->Get_Info();


	// 패턴 진행

	// 패턴 진행 순서
	// 0. [준비] 대화 진행 중 준비 
	
	
	
	// 1. 대기
	// 2. 패턴1 (공격준비 - 공격(투사체 1))
	// 3. 대기
	// 4. 패턴2 (점프 - 자리잡기 - 공격(45도 투사체*6) - 착지 
	// 5. 대기
	// 6. 패턴3 (점프 - 공격(빠른착지))
	// 7. 대기
	// 8. 패턴3
	// 9. 대기
	// 10. 패턴2 (광폭이라면 ++, 아니라면 1로)
	// 11. [광폭] 패턴4 (차징 - 공격(강한 투사체))


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
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // 최종적으로 렌더시킬 좌표.

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(	hDC,					// 최종적으로 그릴 DC
						iOutX,					// 복사받을 위치 X, Y좌표
						iOutY,
						m_tInfo.fCX,	// 복사 받을 가로, 세로 길이.
						m_tInfo.fCY,
						hMemDC,					// 비트맵을 가지고 있는 DC
						m_tInfo.fCX * ((m_tFrame.iFrameCur) % (m_tFrame.iFrameMaxX)),	// 출력하려는 스트라이트 이미지 내에서의 좌표
						m_tInfo.fCY * ((m_tFrame.iFrameCur) / (m_tFrame.iFrameMaxX)),
						m_tInfo.fCX,	// 비트맵을 출력할 가로, 세로 길이
						m_tInfo.fCY,
						RGB(255, 0, 255));		// 제거할 색상

}

void CBoss_HonMeirin::Release()
{
}


void CBoss_HonMeirin::LoadImages()
{
	// 패턴 시작 전 : 대화 중 스탠드 모션
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_stand/honmeirin_stand.bmp", L"Meirin_Stand");
	FRAME_PROP tMeirin_Stand = { 64 * 2, 64 * 2, 4, 4, 16 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Stand, L"Meirin_Stand");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_stand/honmeirin_stand_R.bmp", L"Meirin_Stand_R");
	FRAME_PROP tMeirin_Stand_R = { 64 * 2, 64 * 2, 4, 4, 16 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Stand_R, L"Meirin_Stand_R");




	// 패턴 공통 : 다른 패턴 진행중이지 않을 시, 공격을 막는 모션
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_gurd/honmeirin_gurd.bmp", L"Meirin_Guard");
	FRAME_PROP tMeirin_Guard = { 64 * 2, 64 * 2, 4, 1, 1 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Guard, L"Meirin_Guard");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_gurd/honmeirin_gurd_R.bmp", L"Meirin_Guard_R");
	FRAME_PROP tMeirin_Guard_R = { 64 * 2, 64 * 2, 4, 1, 1 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Guard_R, L"Meirin_Guard_R");

	// 패턴 공통 : 다른 패턴 진행중이지 않을 시 플레이어와 일정 거리를 유지하도록 이동 중의 모션
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/onmeirin_run/honmeirin_run.bmp", L"Meirin_Run");
	FRAME_PROP tMeirin_Run = { 64 * 2, 80 * 2, 4, 3, 12 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Run, L"Meirin_Run");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/onmeirin_run/honmeirin_run_R.bmp", L"Meirin_Run_R");
	FRAME_PROP tMeirin_Run_R = { 64 * 2, 80 * 2, 4, 3, 12 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Run_R, L"Meirin_Run_R");




	// 패턴 1 : 투사체 발사 직전 모션
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_attack_tame/honmeirin_attack_tame.bmp", L"Meirin_Attack_Ready");
	FRAME_PROP tMeirin_Attack_Ready = { 128 * 2, 80 * 2, 1, 3, 3 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Attack_Ready, L"Meirin_Attack_Ready");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_attack_tame/honmeirin_attack_tame_R.bmp", L"Meirin_Attack_Ready_R");
	FRAME_PROP tMeirin_Attack_Ready_R = { 128 * 2, 80 * 2, 1, 3, 3 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Attack_Ready_R, L"Meirin_Attack_Ready_R");

	// 패턴 1 : 투사체 발사 시 모션
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_attack/honmeirin_attack.bmp", L"Meirin_Attack");
	FRAME_PROP tMeirin_Attack = { 128 * 2, 80 * 2, 1, 6, 6 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Attack, L"Meirin_Attack");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_attack/honmeirin_attack_R.bmp", L"Meirin_Attack_R");
	FRAME_PROP tMeirin_Attack_R = { 128 * 2, 80 * 2, 1, 6, 6 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Attack_R, L"Meirin_Attack_R");




	// 패턴 2 : 공격 전 점프 모션
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_jump/honmeirin_jump.bmp", L"Meirin_Jump");
	FRAME_PROP tMeirin_Jump = { 64 * 2, 96 * 2, 1, 2, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Jump, L"Meirin_Jump");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_jump/honmeirin_jump.bmp", L"Meirin_Jump_R");
	FRAME_PROP tMeirin_Jump_R = { 64 * 2, 96 * 2, 1, 2, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Jump_R, L"Meirin_Jump_R");

	// 패턴 2 : 공격 전 이동 모션
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_dash/honmeirin_dash.bmp", L"Meirin_Dash");
	FRAME_PROP tMeirin_Dash = { 80 * 2, 64 * 2, 1, 2, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Dash, L"Meirin_Dash");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_dash/honmeirin_dash_R.bmp", L"Meirin_Dash_R");
	FRAME_PROP tMeirin_Dash_R = { 80 * 2, 64 * 2, 1, 2, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Dash_R, L"Meirin_Dash_R");

	// 패턴 2 : 하단 45도로 투사체를 발사하는 모션
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_tenma/honmeirin_tenma.bmp", L"Meirin_Tenma");
	FRAME_PROP tMeirin_Tenma = { 96 * 2, 96 * 2, 4, 2, 8 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Tenma, L"Meirin_Tenma");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_tenma/honmeirin_tenma_R.bmp", L"Meirin_Tenma_R");
	FRAME_PROP tMeirin_Tenma_R = { 96 * 2, 96 * 2, 4, 2, 8 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Tenma_R, L"Meirin_Tenma_R");

	// 패턴 2 : 투사체 발사 이후 떨어질 때 까지의 모션
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_falling/honmeirin_falling.bmp", L"Meirin_Falling");
	FRAME_PROP tMeirin_Falling = { 96 * 2, 96 * 2, 2, 1, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Falling, L"Meirin_Falling");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_falling/honmeirin_falling_R.bmp", L"Meirin_Falling_R");
	FRAME_PROP tMeirin_Falling_R = { 96 * 2, 96 * 2, 2, 1, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Falling_R, L"Meirin_Falling_R");




	// 패턴 3 : 공격 시 체공 진입하며 떠오르는 모션
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_rising/honmeirin_rising_first.bmp", L"Meirin_Rising_Ready");
	FRAME_PROP tMeirin_Rising_Ready = { 64 * 2, 64 * 2, 1, 2, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Rising_Ready, L"Meirin_Rising_Ready");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_rising/honmeirin_rising_first_R.bmp", L"Meirin_Rising_Ready_R");
	FRAME_PROP tMeirin_Rising_Ready_R = { 64 * 2, 64 * 2, 1, 2, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Rising_Ready, L"Meirin_Rising_Ready_R");

	// 패턴 3 : 공격 시 공중에서부터 착지 직전까지의 모션
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_rising/honmeirin_rising.bmp", L"Meirin_Rising");
	FRAME_PROP tMeirin_Rising = { 96 * 2, 64 * 2, 1, 6, 6, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Rising, L"Meirin_Rising");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_rising/honmeirin_rising_R.bmp", L"Meirin_Rising_R");
	FRAME_PROP tMeirin_Rising_R = { 96 * 2, 64 * 2, 1, 6, 6, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Rising_R, L"Meirin_Rising_R");

	// 패턴 3 : 공격 시 착지 모션
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_landing/honmeirin_landing.bmp", L"Meirin_Landing");
	FRAME_PROP tMeirin_Landing = { 64 * 2, 64 * 2, 3, 3, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Landing, L"Meirin_Landing");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_landing/honmeirin_landing_R.bmp", L"Meirin_Landing_R");
	FRAME_PROP tMeirin_Landing_R = { 64 * 2, 64 * 2, 3, 3, 9 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Landing_R, L"Meirin_Landing_R");




	// 광폭화 패턴 : 강력한 투사체 발사 준비 모션
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_hadou_tame/honmeirin_hadou_tame.bmp", L"Meirin_Hadou_Ready");
	FRAME_PROP tMeirin_Hadou_Ready = { 128 * 2, 64 * 2, 1, 6, 6 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Hadou_Ready, L"Meirin_Hadou_Ready");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_hadou_tame/honmeirin_hadou_tame_R.bmp", L"Meirin_Hadou_Ready_R");
	FRAME_PROP tMeirin_Hadou_Ready_R = { 128 * 2, 64 * 2, 1, 6, 6 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Hadou_Ready_R, L"Meirin_Hadou_Ready_R");

	// 광폭화 패턴 : 강력한 투사체 발사 모션
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_hadou/honmeirin_hadou.bmp", L"Meirin_Hadou");
	FRAME_PROP tMeirin_Hadou = { 128 * 2, 64 * 2, 1, 7, 7 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Hadou, L"Meirin_Hadou");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_hadou/honmeirin_hadou_R.bmp", L"Meirin_Hadou_R");
	FRAME_PROP tMeirin_Hadou_R = { 128 * 2, 64 * 2, 1, 7, 7 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Hadou_R, L"Meirin_Hadou_R");




	// 클리어 이후 : 대기로 넘어가기까지의 모션 (방향 왼쪽 고정임 주의)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_des_merged/honmeirin_des_merged_R.bmp", L"Meirin_Des");
	FRAME_PROP tMeirin_Des = { 96 * 2, 96 * 2, 5, 2, 10 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Des, L"Meirin_Des");

	// 클리어 이후 : 대기 모션 (방향 왼쪽 고정임 주의)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_dying/honmeirin_dying.bmp", L"Meirin_Dying");
	FRAME_PROP tMeirin_Dying = { 64 * 2, 64 * 2, 4, 1, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Dying, L"Meirin_Dying");




}

void CBoss_HonMeirin::Jump()
{
	if (CSceneMgr::Get_Instance()->Get_CurScene() == CSceneMgr::SC_EDIT)
		return;




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

void CBoss_HonMeirin::Motion_Change()
{
	// 보스에 맞게 수정할 것

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
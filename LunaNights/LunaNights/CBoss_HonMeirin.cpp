#include "pch.h"
#include "CBoss_HonMeirin.h"
#include "CBmpMgr.h"
#include "CSpritePropertyMgr.h"
#include "CSceneMgr.h"
#include "CTileCollisionMgr.h"
#include "CCameraMgr.h"
#include "CPlayer.h"
#include "CObjMgr.h"
#include "SoundMgr.h"
#include "CAbstractFactory.h"
#include "CBossBullet.h"

CBoss_HonMeirin::CBoss_HonMeirin() :
	m_ePreState(OBJST_END),
	m_isReady(false),
	m_isMoveDirStretch(false),
	m_isPatternPropCalced(false),
	m_iPattern(0),
	m_dwPatternElapsedFrame(0),
	m_dwPatternNeedFrame(0),
	m_isChangeFrame(true),
	m_iEndPattern(0),
	m_isShown(false),
	m_isShownTrigger(false)
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

	m_fAtk = 12.f;
	m_isStretch = true;

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

	// 플레이어의 위치를 받아옴, 없으면 진행X
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	if (pPlayer == nullptr)
		return OBJ_NOEVENT;
	INFO tPlayerInfo = *pPlayer->Get_Info();



	// 플레이어가 가까이 오면 첫 등장. 1480, 440 로 이동.
	if (m_iPattern == 0 && !m_isShown)
	{
		if (tPlayerInfo.fX >= 1200)
			m_isShownTrigger = true;
		if (m_isShownTrigger && !m_isShown)
		{
			if (m_dwPatternElapsedFrame < 30)
			{
				float fDistX = (1480 - m_tInfo.fX) / (30 - m_dwPatternElapsedFrame);
				float fDistY = (544 - m_tInfo.fY) / (30 - m_dwPatternElapsedFrame);

				m_tInfo.fX += fDistX;
				m_tInfo.fY += fDistY;

				m_pFrameKey = L"Meirin_Landing_R";
				std::cout << "메이링 위치  : X " << m_tInfo.fX << ", " << m_tInfo.fY << std::endl;
				CSoundMgr::Get_Instance()->SetChannelVolume(SOUND_BGM, 0.1f - ((m_dwPatternElapsedFrame + 1) * 0.002f));
			}

			else if (m_dwPatternElapsedFrame < 50)
			{
				m_pFrameKey = L"Meirin_Landing_R";

				if (m_dwPatternElapsedFrame == 30)
				{
					CCameraMgr::Get_Instance()->Set_ShakeStrength(20.f);
					CSoundMgr::Get_Instance()->StopSound(SOUND_DESTROY);
					CSoundMgr::Get_Instance()->PlaySound(L"s15_destroy.wav", SOUND_DESTROY, 0.1f);
				}
				CSoundMgr::Get_Instance()->SetChannelVolume(SOUND_BGM, 0.1f - ((m_dwPatternElapsedFrame + 1) * 0.002f));
			}

			else
			{
				m_isShown = true;
				m_dwPatternElapsedFrame = 0;
				
			}

	
			m_dwPatternElapsedFrame++;
		}

		//m_pFrameKey = L"Meirin_Stand_R";
		__super::Update_Rect_UpStand();
		m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(m_pFrameKey);
		Set_FrameProperty(m_tFramePropCur);
		Set_Scale(m_tFramePropCur.iCX, m_tFramePropCur.iCY);

		return OBJ_NOEVENT;
	}


	// 보스전 종료
	if (m_iHp == 0)
	{
		pPlayer->Set_isBossStart(false);

		
		switch (m_iEndPattern)
		{
		case 0:
			m_iEndPattern++;
			break;


		case 1:
			m_eCurState = OBJST_DAMAGED;
			m_pFrameKey = L"Meirin_DesStart";
			
			// 매 40프레임마다 폭발 사운드 재생 및 이펙트 출력
			if (m_dwPatternElapsedFrame % 15 == 0)
			{
				srand(m_dwPatternElapsedFrame);
				for (int i = 0; i < 2; i++)
				{
					int iTmpX = (rand() % 128 - 128);
					int iTmpY = (rand() % 128 - 128);
					CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT,
						CAbstractFactory<CEffect>::Create(m_tInfo.fX + m_tInfo.fCX / 4 + iTmpX, m_tInfo.fY  + iTmpY, 0.f, L"Effect_Bomb"));
				}

				CCameraMgr::Get_Instance()->Set_ShakeStrength(10.f);
				CSoundMgr::Get_Instance()->StopSound(SOUND_DESTROY);
				CSoundMgr::Get_Instance()->PlaySound(L"s15_destroy.wav", SOUND_DESTROY, 0.2f);
			}

			m_tInfo.fY -= 0.2f;
			m_fVelocityY = 0;

			m_dwPatternNeedFrame = 300;	// 패턴 지속시간

			break;

		case 2:
			m_iEndPattern++;
			m_fVelocityY = -10.f;
			break;

		case 3:
			m_pFrameKey = L"Meirin_Des";

			m_dwPatternNeedFrame = 50;
			break;

		case 4:
			m_pFrameKey = L"Meirin_Dying";

			if (pPlayer->Get_isClearedBoss(1))
			{
				m_iEndPattern++;
				m_dwPatternElapsedFrame = 0;
			}

			CSoundMgr::Get_Instance()->SetChannelVolume(SOUND_BGM, 0.1f - (m_dwPatternElapsedFrame * 0.0005f));
			

			m_dwPatternNeedFrame = 999999999;
			break;

		// 수동으로 넘기면
		case 5:
			std::cout << "메이린 도망가는 이벤트" << std::endl;

			if (m_dwPatternElapsedFrame == 1)
			{
				CSoundMgr::Get_Instance()->StopSound(SOUND_DESTROY);
				CSoundMgr::Get_Instance()->PlaySound(L"s15_destroy.wav", SOUND_DESTROY, 0.2f); // 발소리로 바꾸는 게 나을듯
				CCameraMgr::Get_Instance()->Set_ShakeStrength(20.f);
			}

			m_pFrameKey = L"Meirin_Rising_Ready_R";
			m_tInfo.fX += 20.f;
			m_tInfo.fY -= 20.f;
						
			if (m_dwPatternElapsedFrame >= 200)
			{
				Set_Dead();
			}

			break;
			

		default:
			break;
		}
		



		if (m_dwPatternElapsedFrame >= m_dwPatternNeedFrame)
		{
			m_iEndPattern++;
			m_dwPatternElapsedFrame = 0;

			std::cout << "[INFO][CBoss_HonMeirin::Update] m_iEndPattern changed to [" << m_iEndPattern << "]" << std::endl;
		}

		m_dwPatternElapsedFrame++;


		m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(m_pFrameKey);
		Set_FrameProperty(m_tFramePropCur);
		Set_Scale(m_tFramePropCur.iCX, m_tFramePropCur.iCY);
		__super::Update_Rect_UpStand();

		return OBJ_NOEVENT;
	}


	// 패턴 진행

	// 패턴 진행 순서
	// 0. [준비] 대화 진행 중 준비 
	if (m_iPattern == 0)
	{
		m_pFrameKey = L"Meirin_Stand_R";
		m_eCurState = OBJST_IDLE;
		
		__super::Update_Rect_UpStand();
		m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(m_pFrameKey);
		Set_FrameProperty(m_tFramePropCur);
		Set_Scale(m_tFramePropCur.iCX, m_tFramePropCur.iCY);

		return OBJ_NOEVENT;
	}
	
	// (이후 플레이어의 m_isBossStart 변수가 true가 된다면 아래 패턴 진행)
	switch (m_iPattern)
	{
	// 대기
	case 1:
	case 3:
	case 5:
	case 7:
	case 9:
	case 11:
		
		// 적정 거리를 유지하도록 2초간 이동 
		// 단, case 5 또는 7 일때 (착지공격 패턴 전), 11 일때 (궁) 는 무조건 거리를 벌림

		m_eCurState = OBJST_RUN; 
		if (m_isStretch)	m_pFrameKey = L"Meirin_Run_R";
		else				m_pFrameKey = L"Meirin_Run";

		if (!m_isPatternPropCalced)
		{
			if ((tPlayerInfo.fX < m_tInfo.fX && abs(tPlayerInfo.fX - m_tInfo.fX) > 400) ||
				(tPlayerInfo.fX > m_tInfo.fX && abs(tPlayerInfo.fX - m_tInfo.fX) < 300))
				m_isMoveDirStretch = true;
			else
				m_isMoveDirStretch = false;

			// case 5 또는 7, 11일때는 무조건 거리를 벌림
			if (m_iPattern == 5 ||
				m_iPattern == 7 ||
				m_iPattern == 11 )
				m_isMoveDirStretch = (tPlayerInfo.fX < m_tInfo.fX) ? false : true;

			m_isPatternPropCalced = true;	// 공격할 방향 확정.
		}

		if (m_isMoveDirStretch)		m_tInfo.fX -= m_fSpeed;
		else						m_tInfo.fX += m_fSpeed;


		// 단, 도중에 칼날이 앞에 감지되면 가드 자세 취함
		// 이건 일단 나중에 구현... 밑에꺼 다 만들고 건드리기
		//if ()




		m_dwPatternNeedFrame = 100;	// 패턴 지속프레임

		break;

	// 패턴1 (공격준비 - 공격(투사체 1))
	case 2:
		
		m_eCurState = OBJST_ACTION1;	// m_tFrame.dwSpeed 조절용, 이건 

		if (m_dwPatternElapsedFrame < 50)
		{
			if (m_isStretch)	m_pFrameKey = L"Meirin_Attack_Ready_R";
			else				m_pFrameKey = L"Meirin_Attack_Ready";
		}
		else if (m_dwPatternElapsedFrame < 100)
		{
			if (m_dwPatternElapsedFrame == 50)
			{
				// 총알 1 생성
				if (m_isStretch)
					CObjMgr::Get_Instance()->Add_Object(OBJ_BOSSBULLET, CAbstractFactory<CBossBullet>::Create(m_tInfo.fX, m_tInfo.fY - m_tInfo.fCY / 2, 180, L"Meirin_Bullet_R"));
				else
					CObjMgr::Get_Instance()->Add_Object(OBJ_BOSSBULLET, CAbstractFactory<CBossBullet>::Create(m_tInfo.fX, m_tInfo.fY - m_tInfo.fCY / 2, 0, L"Meirin_Bullet"));

				//if (m_isStretch)
				//	CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CEffect>::CreateEffect(m_tInfo.fX + 80, m_tInfo.fY - 64, 0, L"Effect_DustSmoke"));
				//else
				//	CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CEffect>::CreateEffect(m_tInfo.fX - 80, m_tInfo.fY - 64, 0, L"Effect_DustSmoke_R"));


				CCameraMgr::Get_Instance()->Set_ShakeStrength(20.f);
			}

			if (m_isStretch)	m_pFrameKey = L"Meirin_Attack_R";
			else				m_pFrameKey = L"Meirin_Attack";
		}

		m_dwPatternNeedFrame = 150;	// 패턴 지속프레임

		break;

	// 패턴2 (점프 - 자리잡기 - 공격(45도 투사체*6) - 착지 
	case 4:
	case 10:

		m_eCurState = OBJST_ACTION2;

		// 점프
		if (m_dwPatternElapsedFrame < 50)
		{
			if (m_isStretch)	m_pFrameKey = L"Meirin_Jump_R";
			else				m_pFrameKey = L"Meirin_Jump";
			
			m_fVelocityY = -GRAVITY * 0.1;
			m_tInfo.fY -= m_fSpeed * 2;
		}
		// 자리잡기
		else if (m_dwPatternElapsedFrame < 100)
		{
			// 플레이어가 더 왼쪽에 있으면, 왼쪽으로 방향 고정. 아니면 반대
			if (m_dwPatternElapsedFrame == 50)
			{
				if (tPlayerInfo.fX < m_tInfo.fX)	m_isMoveDirStretch = true;
				else								m_isMoveDirStretch = false;
			}
			//std::cout << "m_isMoveDirStretch : " << m_isMoveDirStretch << std::endl;

			if (m_isStretch)	m_pFrameKey = L"Meirin_Dash_R";
			else				m_pFrameKey = L"Meirin_Dash";

			// 이따만큼 이동할거임
			float fMoveDistance;
			
			if (m_isMoveDirStretch)	// 왼쪽으로
				fMoveDistance = (tPlayerInfo.fX < m_tInfo.fX) ? m_tInfo.fX - tPlayerInfo.fX + 400 : 400 - (tPlayerInfo.fX - m_tInfo.fX);
			else					// 오른쪽으로
				fMoveDistance = (tPlayerInfo.fX > m_tInfo.fX) ? tPlayerInfo.fX - m_tInfo.fX + 400 : 400 - (m_tInfo.fX - tPlayerInfo.fX);

			
			float fMoveDistancePerFrame = fMoveDistance / (100 - m_dwPatternElapsedFrame);

			if (m_isMoveDirStretch)		m_tInfo.fX -= fMoveDistancePerFrame;
			else						m_tInfo.fX += fMoveDistancePerFrame;

			m_fVelocityY = -GRAVITY * 0.1;
		}
		else if (m_dwPatternElapsedFrame <= 200)
		{
			// 공격 시에는 방향을 반대로 틀으므로 조건도 반대
			if (!m_isStretch)	m_pFrameKey = L"Meirin_Tenma_R";
			else				m_pFrameKey = L"Meirin_Tenma";

			if ((m_dwPatternElapsedFrame - 100) % 20 == 0)
			{
				int iSpace = 10;

				// 총알 2 생성
				// 생성 위치 조건은.. 이렇게 결과값이 짝이나 홀이냐에 따라 윗공격 아랫공격 나누면 될 듯
				if (((m_dwPatternElapsedFrame - 100) / 20) % 2 == 1)
				{
					if (!m_isStretch)
						CObjMgr::Get_Instance()->Add_Object(OBJ_BOSSBULLET, CAbstractFactory<CBossBullet>::Create(m_tInfo.fX + iSpace, m_tInfo.fY - m_tInfo.fCY / 1.5 + iSpace, 220, L"Meirin_Bullet45_R"));
					else
						CObjMgr::Get_Instance()->Add_Object(OBJ_BOSSBULLET, CAbstractFactory<CBossBullet>::Create(m_tInfo.fX - iSpace, m_tInfo.fY - m_tInfo.fCY / 1.5 + iSpace, 310, L"Meirin_Bullet45"));
				}
				else
				{
					if (!m_isStretch)
						CObjMgr::Get_Instance()->Add_Object(OBJ_BOSSBULLET, CAbstractFactory<CBossBullet>::Create(m_tInfo.fX - iSpace, m_tInfo.fY - m_tInfo.fCY / 1.5 - iSpace, 230, L"Meirin_Bullet45_R"));
					else
						CObjMgr::Get_Instance()->Add_Object(OBJ_BOSSBULLET, CAbstractFactory<CBossBullet>::Create(m_tInfo.fX + iSpace, m_tInfo.fY - m_tInfo.fCY / 1.5 - iSpace, 320, L"Meirin_Bullet45"));

				}

				CCameraMgr::Get_Instance()->Set_ShakeStrength(20.f);
				std::cout << "[INFO][CBoss_HonMeirin::Update] Boss Bullet Generated on..  case :" << m_iPattern << std::endl;
			}
			
			m_fVelocityY = 0;
		}
		else
		{
			if (m_dwPatternElapsedFrame == 201)
				m_fVelocityY = -20;

			if (!m_isStretch)	{ m_pFrameKey = L"Meirin_Falling_R";	m_tFrame.iFrameCur = 0; }
			else				{ m_pFrameKey = L"Meirin_Falling";		m_tFrame.iFrameCur = 0; }
			
			if (m_isJumping)
			{
				m_tFrame.iFrameCur = 0;
				m_isChangeFrame = false;
				if (!m_isStretch)	m_tInfo.fX += m_fSpeed;
				else				m_tInfo.fX -= m_fSpeed;
			}
			else
			{
				m_tFrame.iFrameCur = 1;
				m_isChangeFrame = false;
			}

			//std::cout << "테스트 :: Y축 높이는 " << m_tInfo.fY << ", 현재 진행 프레임은 " << m_dwPatternElapsedFrame << "." << std::endl;
			
			if (m_dwPatternElapsedFrame == 274)
			{
				CCameraMgr::Get_Instance()->Set_ShakeStrength(20.f);
				
				CSoundMgr::Get_Instance()->StopSound(SOUND_LAND2);
				CSoundMgr::Get_Instance()->PlaySound(L"s13_absorber.wav", SOUND_LAND2, 0.2f);
			}

			if (m_dwPatternElapsedFrame >= 274)
			{
				if (!m_isStretch)	{ m_pFrameKey = L"Meirin_Landing_R"; }
				else				{ m_pFrameKey = L"Meirin_Landing";	 }
			}
		}

		m_dwPatternNeedFrame = 300; // 패턴 지속프레임

		break;

	// 패턴3(점프 - 공격(빠른착지))
	case 6:
	case 8:

		m_eCurState = OBJST_ACTION3;
		
		// 점프
		if (m_dwPatternElapsedFrame < 40)
		{
			if (m_dwPatternElapsedFrame == 1)
			{
				if (tPlayerInfo.fX < m_tInfo.fX)	m_isMoveDirStretch = true;
				else								m_isMoveDirStretch = false;
			}

			if (!m_isStretch)	m_pFrameKey = L"Meirin_Rising_Ready_R";
			else				m_pFrameKey = L"Meirin_Rising_Ready";

			// 이따만큼 이동할거임
			float fMoveDistance;

			if (m_isMoveDirStretch)	// 왼쪽으로
				fMoveDistance = (tPlayerInfo.fX < m_tInfo.fX) ? m_tInfo.fX - tPlayerInfo.fX + 600 : 600 - (tPlayerInfo.fX - m_tInfo.fX);
			else					// 오른쪽으로
				fMoveDistance = (tPlayerInfo.fX > m_tInfo.fX) ? tPlayerInfo.fX - m_tInfo.fX + 600 : 600 - (m_tInfo.fX - tPlayerInfo.fX);


			float fMoveDistancePerFrame = fMoveDistance / (40 - m_dwPatternElapsedFrame);

			if (m_isMoveDirStretch)		{ m_tInfo.fX -= fMoveDistancePerFrame; m_tInfo.fY -= m_fSpeed * 2.0; }
			else						{ m_tInfo.fX += fMoveDistancePerFrame; m_tInfo.fY -= m_fSpeed * 2.0; }

			m_fVelocityY = -GRAVITY * 0.1;
		}
		else if (m_dwPatternElapsedFrame < 50)
		{
			m_tInfo.fY -= m_fSpeed;
		}
		else if (m_dwPatternElapsedFrame < 70)
		{
			if (m_dwPatternElapsedFrame == 50)
			{
				if (tPlayerInfo.fX < m_tInfo.fX)	m_isMoveDirStretch = true;
				else								m_isMoveDirStretch = false;
			}

			if (!m_isStretch)	m_pFrameKey = L"Meirin_Rising_R";
			else				m_pFrameKey = L"Meirin_Rising";

			// 이따만큼 이동할거임
			float fMoveDistance;

			if (m_isMoveDirStretch)	// 왼쪽으로
				fMoveDistance = (tPlayerInfo.fX < m_tInfo.fX) ? m_tInfo.fX - tPlayerInfo.fX + 250 : 250 - (tPlayerInfo.fX - m_tInfo.fX);
			else					// 오른쪽으로
				fMoveDistance = (tPlayerInfo.fX > m_tInfo.fX) ? tPlayerInfo.fX - m_tInfo.fX + 250 : 250 - (m_tInfo.fX - tPlayerInfo.fX);


			float fMoveDistancePerFrame = fMoveDistance / (20 - (m_dwPatternElapsedFrame - 50));

			
			if (m_isMoveDirStretch)		m_tInfo.fX -= fMoveDistancePerFrame;
			else						m_tInfo.fX += fMoveDistancePerFrame;

			m_fVelocityY = 30;
		}
		else
		{
			if (m_dwPatternElapsedFrame == 70)
			{
				CCameraMgr::Get_Instance()->Set_ShakeStrength(20.f);
				CSoundMgr::Get_Instance()->StopSound(SOUND_DESTROY);
				CSoundMgr::Get_Instance()->PlaySound(L"s15_destroy.wav", SOUND_DESTROY, 0.1f);
			}
			if (m_dwPatternElapsedFrame >= 70)
			{
				if (!m_isStretch)	{ m_pFrameKey = L"Meirin_Landing_R"; }
				else				{ m_pFrameKey = L"Meirin_Landing"; }
			}
		}

		std::cout << "테스트 :: Y축 높이는 " << m_tInfo.fY << ", 현재 진행 프레임은 " << m_dwPatternElapsedFrame << "." << std::endl;


		m_dwPatternNeedFrame = 100; // 패턴 지속프레임



		break;


	// [광폭] 패턴4 (차징 - 공격(강한 투사체))
	case 12:
		m_eCurState = OBJST_ACTION4;

		// 차지
		if (m_dwPatternElapsedFrame < 200)
		{
			CCameraMgr::Get_Instance()->Set_ShakeStrength(10.f);
			std::cout << "차지 준비" << std::endl;

			if (m_dwPatternElapsedFrame == 1)
			{
				CSoundMgr::Get_Instance()->StopSound(SOUND_BOSS_SKILL);
				CSoundMgr::Get_Instance()->PlaySound(L"s06_skill.wav", SOUND_BOSS_SKILL, 0.2f);
			}

			if (m_isStretch)	m_pFrameKey = L"Meirin_Hadou_Ready_R";
			else				m_pFrameKey = L"Meirin_Hadou_Ready";
		}
		else
		{
			if (m_dwPatternElapsedFrame == 200)
			{
				CCameraMgr::Get_Instance()->Set_ShakeStrength(30.f);
				// 총알 3 생성

				if (m_isStretch)
					CObjMgr::Get_Instance()->Add_Object(OBJ_BOSSBULLET, CAbstractFactory<CBossBullet>::Create(m_tInfo.fX, m_tInfo.fY - m_tInfo.fCY / 1.1, -180, L"Meirin_Bullet_Big_R"));
				else
					CObjMgr::Get_Instance()->Add_Object(OBJ_BOSSBULLET, CAbstractFactory<CBossBullet>::Create(m_tInfo.fX, m_tInfo.fY - m_tInfo.fCY / 1.1, 0, L"Meirin_Bullet_Big"));
			
			}

			if (m_isStretch)	m_pFrameKey = L"Meirin_Hadou_R";
			else				m_pFrameKey = L"Meirin_Hadou";
		}

		m_dwPatternNeedFrame = 300; // 패턴 지속프레임

		break;

	default:
		
		std::cout << "[WARN][CBoss_HonMeirin::Update] 예외 발생 - 보스 패턴 변수가 1~12 가 아님" << std::endl;

		break;
	}


	// 다음 패턴으로 넘어갈 조건(시간경과)이 됐다면.
	// 다음 패턴으로 넘어감 및 바라보는 방향 전환
	// m_dwPatternElapsedFrame 은 switch문 각각의 부분에서 지정
	if (m_dwPatternElapsedFrame >= m_dwPatternNeedFrame)
	{
		// 바라보는 방향 전환
		if (tPlayerInfo.fX > m_tInfo.fX)		m_isStretch = false;
		else									m_isStretch = true;

		// 다음 패턴으로 전환
		if ((m_iPattern == 10 && m_iHp >= 500) ||
			(m_iPattern == 12))
			m_iPattern = 1;
		else if ((m_iPattern >= 10 && m_iHp < 500) ||
			(m_iPattern < 10))
			m_iPattern++;

		m_dwPatternElapsedFrame = 0;
		m_isPatternPropCalced = false;

		std::cout << "[INFO][CBoss_HonMeirin::Update] m_iPattern changed to [" << m_iPattern << "]" << std::endl;
	}

	m_dwPatternElapsedFrame++;

	// 1. 대기
	// 2. 패턴1 (공격준비 - 공격(투사체 1))
	// 3. 대기
	// 4. 패턴2 (점프 - 자리잡기 - 공격(45도 투사체*6) - 착지 
	// 5. 대기
	// 6. 패턴3 (점프 - 공격(빠른착지))
	// 7. 대기
	// 8. 패턴3
	// 9. 대기
	// 10. 패턴2
	
	// (HP 500 미만이라면 ++, 아니라면 1로)
	
	// 11. [광폭] 대기
	// 12. [광폭] 패턴4 (차징 - 공격(강한 투사체))

	m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(m_pFrameKey);
	Set_FrameProperty(m_tFramePropCur);
	Set_Scale(m_tFramePropCur.iCX, m_tFramePropCur.iCY);

	m_tCollideInfo = {	m_tInfo.fX, 
						m_tInfo.fY - 48,
						48,
						96 };

	// 원작과 같게, 3번 패턴중에는 앉거나 이동으로 피할 수 있게 하기 위해 콜라이더 사이즈를 해당 패턴에 한해 줄임
	if (m_eCurState == OBJST_ACTION3 &&
		m_dwPatternElapsedFrame >= 50 &&
		m_dwPatternElapsedFrame < 70)
		m_tCollideInfo = { m_tInfo.fX,
					m_tInfo.fY - 64,
					48,
					32 };

	__super::Update_Rect_UpStand();
	
	return 0;
}

void CBoss_HonMeirin::Late_Update()
{
	if (!(	m_iEndPattern == 1 ||
			m_iEndPattern == 5 || 
			(m_iPattern == 0 && !m_isShown)))
		Jump();

	if (m_isChangeFrame)
		Move_Frame();
	m_isChangeFrame = true;
	
	Motion_Change();
}

void CBoss_HonMeirin::Render(HDC hDC)
{
	int iOutX = m_tRect.left;
	int iOutY = m_tRect.top;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // 최종적으로 렌더시킬 좌표.

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);


	if (m_iEndPattern == 1)
	{
		if (GetTickCount() % 2)
		{
			GdiTransparentBlt(hDC,					// 최종적으로 그릴 DC
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

	}
	else
	{
		GdiTransparentBlt(hDC,					// 최종적으로 그릴 DC
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
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_run/honmeirin_run.bmp", L"Meirin_Run");
	FRAME_PROP tMeirin_Run = { 64 * 2, 80 * 2, 4, 3, 12 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Run, L"Meirin_Run");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_run/honmeirin_run_R.bmp", L"Meirin_Run_R");
	FRAME_PROP tMeirin_Run_R = { 64 * 2, 80 * 2, 4, 3, 12 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Run_R, L"Meirin_Run_R");




	// 패턴 1 : 투사체 발사 직전 모션
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_attack_tame/honmeirin_attack_tame.bmp", L"Meirin_Attack_Ready");
	FRAME_PROP tMeirin_Attack_Ready = { 128 * 2, 80 * 2, 1, 3, 3, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Attack_Ready, L"Meirin_Attack_Ready");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_attack_tame/honmeirin_attack_tame_R.bmp", L"Meirin_Attack_Ready_R");
	FRAME_PROP tMeirin_Attack_Ready_R = { 128 * 2, 80 * 2, 1, 3, 3, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Attack_Ready_R, L"Meirin_Attack_Ready_R");

	// 패턴 1 : 투사체 발사 시 모션
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_attack/honmeirin_attack.bmp", L"Meirin_Attack");
	FRAME_PROP tMeirin_Attack = { 128 * 2, 80 * 2, 1, 6, 6, 5 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Attack, L"Meirin_Attack");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_attack/honmeirin_attack_R.bmp", L"Meirin_Attack_R");
	FRAME_PROP tMeirin_Attack_R = { 128 * 2, 80 * 2, 1, 6, 6, 5 };
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
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_rising_first/honmeirin_rising_first.bmp", L"Meirin_Rising_Ready");
	FRAME_PROP tMeirin_Rising_Ready = { 64 * 2, 64 * 2, 1, 2, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Rising_Ready, L"Meirin_Rising_Ready");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_rising_first/honmeirin_rising_first_R.bmp", L"Meirin_Rising_Ready_R");
	FRAME_PROP tMeirin_Rising_Ready_R = { 64 * 2, 64 * 2, 1, 2, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Rising_Ready, L"Meirin_Rising_Ready_R");

	// 패턴 3 : 공격 시 공중에서부터 착지 직전까지의 모션
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_rising/honmeirin_rising.bmp", L"Meirin_Rising");
	FRAME_PROP tMeirin_Rising = { 96 * 2, 64 * 2, 1, 6, 6, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Rising, L"Meirin_Rising");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_rising/honmeirin_rising_R.bmp", L"Meirin_Rising_R");
	FRAME_PROP tMeirin_Rising_R = { 96 * 2, 64 * 2, 1, 6, 6, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Rising_R, L"Meirin_Rising_R");

	// 패턴 2, 3 : 공격 시 착지 모션
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_landing/honmeirin_landing.bmp", L"Meirin_Landing");
	FRAME_PROP tMeirin_Landing = { 64 * 2, 64 * 2, 3, 3, 9, 8 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Landing, L"Meirin_Landing");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_landing/honmeirin_landing_R.bmp", L"Meirin_Landing_R");
	FRAME_PROP tMeirin_Landing_R = { 64 * 2, 64 * 2, 3, 3, 9, 8 };
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
	FRAME_PROP tMeirin_Hadou = { 128 * 2, 64 * 2, 1, 7, 7, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Hadou, L"Meirin_Hadou");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_hadou/honmeirin_hadou_R.bmp", L"Meirin_Hadou_R");
	FRAME_PROP tMeirin_Hadou_R = { 128 * 2, 64 * 2, 1, 7, 7, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_Hadou_R, L"Meirin_Hadou_R");



	// 클리어 이후 : 폭발 (방향 왼쪽 고정임 주의)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_des_merged/honmeirin_des.bmp", L"Meirin_DesStart");
	FRAME_PROP tMeirin_DesStart = { 96 * 2, 96 * 2, 1, 1, 1 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tMeirin_DesStart, L"Meirin_DesStart");


	// 클리어 이후 : 대기로 넘어가기까지의 모션 (방향 왼쪽 고정임 주의)
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirin/honmeirin_des_merged/honmeirin_des_merged_R.bmp", L"Meirin_Des");
	FRAME_PROP tMeirin_Des = { 96 * 2, 96 * 2, 5, 2, 10, 8 };
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

	float fMaxVelocityY = 12;				// 최대 낙하속도 제한
	float fGravityConst = GRAVITY * 0.08;	// 대충 자연스러운 중력가속도 및 계수

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
			m_tFrame.dwSpeed = 80;
			break;

		case OBJ_STATE::OBJST_JUMP:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 80;
			break;

		case OBJ_STATE::OBJST_RUN:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 80;
			break;
		
		case OBJ_STATE::OBJST_ACTION2:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 70;
			break;
			
		case OBJ_STATE::OBJST_ACTION4:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 40;
			break;

		case OBJ_STATE::OBJST_DAMAGED:
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 100;
			break;

		default:
			m_tFrame.dwSpeed = 100;
			break;
		}

		m_ePreState = m_eCurState;
		m_tFrame.iFrameCur = 0;
	}
}
#include "pch.h"
#include "CUI.h"


#include "CBmpMgr.h"
#include "CSpritePropertyMgr.h"
#include "CSceneMgr.h"
#include "CPlayer.h"
#include "CObjMgr.h"
#include "SoundMgr.h"
#include "CBoss_HonMeirin.h"

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!	UI 하나 추가할 때 마다 해당 FRAME 변수 및
// !!!	변수에 대한 Zeromemory 추가할 것!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


CUI::CUI()
{
	ZeroMemory(&tFrameHPMP, sizeof(FRAME));
	ZeroMemory(&tFrameGauge_Stop, sizeof(FRAME));
	ZeroMemory(&tFrameTime_Number, sizeof(FRAME));
	ZeroMemory(&tFrameNumber_Sprite, sizeof(FRAME));
}

CUI::~CUI()
{
	Release();
}

void CUI::Initialize()
{
	// LoadImages

	//.. 플레이 중 맨 위 중앙의 UI
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/UI/gauge_sprite/guage_sprite.bmp", L"UI_Gauge_Sprite");
	FRAME_PROP tUI_Guage_Sprite = { 576 * 2, 96 * 2, 1, 2, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tUI_Guage_Sprite, L"UI_Gauge_Sprite");

	// CUI 하나에서 모든 고정형 UI를 관리할 것이기 때문에,
	// 스프라이트의 변화 존재하는 UI는 Insert_Property를 사용하지 않고
	// 로컬 변수에 저장해뒀다가(현재 프레임 정보때문) 꺼내씀
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/UI/hpvar_sprite/hpvar_sprite.bmp", L"UI_HP");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/UI/mpvar_sprite/mpvar_sprite.bmp", L"UI_MP");
	FRAME_PROP tUI_HPMP = { 100 * 2, 5 * 2, 1, 20, 20 };
	Set_FrameProperty(tUI_HPMP, tFrameHPMP);
	tFrameHPMP.dwSpeed = 14;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/UI/ui_number_sprite/ui_number_sprite.bmp", L"UI_Number_Sprite");
	FRAME_PROP tUI_Number_Sprite = { 4 * 2, 5 * 2, 10, 1, 10 };
	Set_FrameProperty(tUI_Number_Sprite, tFrameNumber_Sprite);


	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/UI/gauge_stop/gauge_stop.bmp", L"UI_Gauge_Stop");
	FRAME_PROP tUI_Gauge_Stop = { 32 * 2, 7 * 2, 1, 24, 24 };
	Set_FrameProperty(tUI_Gauge_Stop, tFrameGauge_Stop);
	tFrameGauge_Stop.dwSpeed = 30;
	tFrameGauge_Stop.iFrameCur = 16;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/UI/time_number_sprite/time_number_sprite.bmp", L"UI_Time_Number");
	FRAME_PROP tUI_Time_Number = { 12 * 2, 18 * 2, 10, 1, 10 };
	Set_FrameProperty(tUI_Time_Number, tFrameTime_Number);


	// 대화창
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/_non_grouped_things/message_window_sprite/message_window_sprite.bmp", L"UI_Message");
	FRAME_PROP tUI_Message = { 1200, 150, 1, 5, 5 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tUI_Message, L"UI_Message");



	Set_Pos(WINCX / 2, WINCY / 2);

	m_eRender = RENDER_UI;


}

int CUI::Update()
{
	CSceneMgr::SCENEID tCurScene = CSceneMgr::Get_Instance()->Get_CurScene();
	
	// 스테이지에서만 보이도록 함
	if (tCurScene == CSceneMgr::SCENEID::SC_LOGO ||
		tCurScene == CSceneMgr::SCENEID::SC_END ||
		tCurScene == CSceneMgr::SCENEID::SC_EDIT)
		return OBJ_NOEVENT;




	// MoveFrame으로 스프라이트 갱신	
	Move_Frame(tFrameHPMP);
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	if (!pPlayer)
		return OBJ_NOEVENT;

	if ( !(	(pPlayer->Get_Stat(CPlayer::TIMEMODE) == 2) && (tFrameGauge_Stop.iFrameCur == 0) ||
			(pPlayer->Get_Stat(CPlayer::TIMEMODE) == 1) && (tFrameGauge_Stop.iFrameCur == 8) ||
			(pPlayer->Get_Stat(CPlayer::TIMEMODE) == 0) && (tFrameGauge_Stop.iFrameCur == 16)))
		Move_Frame(tFrameGauge_Stop);

	if (pPlayer->Get_Stat(CPlayer::ISGETWATCH) == TRUE)
		Move_Frame(tFrameTime_Number);


	return 0;
}

void CUI::Late_Update()
{
}

void CUI::Render(HDC hDC)
{
	CSceneMgr::SCENEID tCurScene = CSceneMgr::Get_Instance()->Get_CurScene();

	// 스테이지에서만 보이도록 함
	if (tCurScene == CSceneMgr::SCENEID::SC_LOGO ||
		tCurScene == CSceneMgr::SCENEID::SC_END ||
		tCurScene == CSceneMgr::SCENEID::SC_EDIT)
		return;



	HDC	hMemDC;
	FRAME_PROP tCurProp = {};
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	if (!pPlayer)
		return;





	// 남은 HP (체력)
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_HP");

	StretchBlt(			hDC,											// 최종적으로 그릴 DC
		 				212,												// 복사받을 위치 X, Y좌표
		 				80,
		 				tFrameHPMP.iCX * ((float)pPlayer->Get_Stat(CPlayer::HP) / (float)pPlayer->Get_Stat(CPlayer::MAXHP)),									// 복사 받을 가로, 세로 길이.
		 				tFrameHPMP.iCY,
		 				hMemDC,											// 비트맵을 가지고 있는 DC
						tFrameHPMP.iCX * ((tFrameHPMP.iFrameCur) % (tFrameHPMP.iFrameMaxX)),	// 출력하려는 스트라이트 이미지 내에서의 좌표
						tFrameHPMP.iCY * ((tFrameHPMP.iFrameCur) / (tFrameHPMP.iFrameMaxX)),	// 출력하려는 스트라이트 이미지 내에서의 좌표
						tFrameHPMP.iCX,									// 비트맵을 출력할 가로, 세로 길이
		 				tFrameHPMP.iCY,
		 				SRCCOPY);										// 복사 모드


	// 남은 MP (마나)
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_MP");

	StretchBlt(			hDC,											// 최종적으로 그릴 DC
		 				212,												// 복사받을 위치 X, Y좌표
		 				104,
		 				tFrameHPMP.iCX * ((float)pPlayer->Get_Stat(CPlayer::MP) / (float)pPlayer->Get_Stat(CPlayer::MAXMP)),									// 복사 받을 가로, 세로 길이.
		 				tFrameHPMP.iCY,
		 				hMemDC,											// 비트맵을 가지고 있는 DC
						tFrameHPMP.iCX * ((tFrameHPMP.iFrameCur) % (tFrameHPMP.iFrameMaxX)),	// 출력하려는 스트라이트 이미지 내에서의 좌표
						tFrameHPMP.iCY * ((tFrameHPMP.iFrameCur) / (tFrameHPMP.iFrameMaxX)),	// 출력하려는 스트라이트 이미지 내에서의 좌표
						tFrameHPMP.iCX,									// 비트맵을 출력할 가로, 세로 길이
		 				tFrameHPMP.iCY,
		 				SRCCOPY);										// 복사 모드
	



	// 상단에 보일 UI 틀
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_Gauge_Sprite");
	tCurProp = CSpritePropertyMgr::Get_Instance()->Find_Property(L"UI_Gauge_Sprite");
	
	GdiTransparentBlt(	hDC,											// 최종적으로 그릴 DC
		 				WINCX / 2 - tCurProp.iCX / 2,					// 복사받을 위치 X, Y좌표
		 				0,
		 				tCurProp.iCX,									// 복사 받을 가로, 세로 길이.
		 				tCurProp.iCY,
		 				hMemDC,											// 비트맵을 가지고 있는 DC
						0,												// 출력하려는 스트라이트 이미지 내에서의 좌표
						pPlayer->Get_Stat(CPlayer::ISGETWATCH) * tCurProp.iCY,
						tCurProp.iCX,									// 비트맵을 출력할 가로, 세로 길이
		 				tCurProp.iCY,
		 				RGB(255, 0, 255));								// 제거할 색상


	// 현재 상태 (Gold 아래 STOP, SNAIL, MOVE 상태창)
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_Gauge_Stop");

	GdiTransparentBlt(	hDC,											// 최종적으로 그릴 DC
		 				469,					// 복사받을 위치 X, Y좌표
		 				102,
		 				tFrameGauge_Stop.iCX,									// 복사 받을 가로, 세로 길이.
		 				tFrameGauge_Stop.iCY,
		 				hMemDC,											// 비트맵을 가지고 있는 DC
						tFrameGauge_Stop.iCX * ((tFrameGauge_Stop.iFrameCur) % (tFrameGauge_Stop.iFrameMaxX)),												// 출력하려는 스트라이트 이미지 내에서의 좌표
						tFrameGauge_Stop.iCY * ((tFrameGauge_Stop.iFrameCur) / (tFrameGauge_Stop.iFrameMaxX)),
						tFrameGauge_Stop.iCX,									// 비트맵을 출력할 가로, 세로 길이
		 				tFrameGauge_Stop.iCY,
		 				RGB(255, 0, 255));								// 제거할 색상


	// 남은 TP (시간)
	if (pPlayer->Get_Stat(CPlayer::ISGETWATCH) == TRUE)
	{
		int iTp = pPlayer->Get_Stat(CPlayer::TP);
		int iTpCharNum = log10(iTp) + 1;	// 자릿수
		if (iTpCharNum <= 0) iTpCharNum = 1;

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_Time_Number");

		int iFirstCharX = 605 + (tFrameTime_Number.iCX / 2) * (3 - iTpCharNum);	// 자릿수에 따른 위치 보정

		for (int i = 0; i < iTpCharNum; i++) // 자릿수만큼만 남은시간 표시
		{
			int iTpChar = (iTp % (int)pow(10, i + 1)) / (pow(10, i));							// 출력하려는 수
			if (iTp <= 0) iTpChar = 0;

			GdiTransparentBlt(	hDC,											// 최종적으로 그릴 DC
								605 + (iTpCharNum * 0.5 - i + 0.5) * tFrameTime_Number.iCX,	// 복사받을 위치 X, Y좌표
		 						103,
		 						tFrameTime_Number.iCX,									// 복사 받을 가로, 세로 길이.
		 						tFrameTime_Number.iCY,
		 						hMemDC,											// 비트맵을 가지고 있는 DC
								tFrameTime_Number.iCX * ((iTpChar) % (tFrameTime_Number.iFrameMaxX)),												// 출력하려는 스트라이트 이미지 내에서의 좌표
								tFrameTime_Number.iCY * ((iTpChar) / (tFrameTime_Number.iFrameMaxX)),
								tFrameTime_Number.iCX,									// 비트맵을 출력할 가로, 세로 길이
		 						tFrameTime_Number.iCY,
		 						RGB(255, 0, 255));								// 제거할 색상
		}

	}



	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_Number_Sprite");

	// 남은 HP

	int iHp = pPlayer->Get_Stat(CPlayer::HP);

	for (int i = 0; i < 3; i++)
	{
		int iHpChar = (iHp % (int)pow(10, 3 - i)) / (pow(10, 2 - i));							// 출력하려는 수

		GdiTransparentBlt(	hDC,											// 최종적으로 그릴 DC
							184 + i * tFrameNumber_Sprite.iCX,				// 복사받을 위치 X, Y좌표
							80,
							tFrameNumber_Sprite.iCX,						// 복사 받을 가로, 세로 길이.
							tFrameNumber_Sprite.iCY,
							hMemDC,											// 비트맵을 가지고 있는 DC
							tFrameNumber_Sprite.iCX * ((iHpChar) % 10),		// 출력하려는 스트라이트 이미지 내에서의 좌표
							tFrameNumber_Sprite.iCY * ((iHpChar) / 10),
							tFrameNumber_Sprite.iCX,						// 비트맵을 출력할 가로, 세로 길이
							tFrameNumber_Sprite.iCY,
							RGB(255, 0, 255));								// 제거할 색상
	}

	// 남은 MP

	int iMp = pPlayer->Get_Stat(CPlayer::MP);

	for (int i = 0; i < 3; i++)
	{
		int iMpChar = (iMp % (int)pow(10, 3 - i)) / (pow(10, 2 - i));							// 출력하려는 수

		GdiTransparentBlt(	hDC,											// 최종적으로 그릴 DC
							184 + i * tFrameNumber_Sprite.iCX,				// 복사받을 위치 X, Y좌표
							104,
							tFrameNumber_Sprite.iCX,						// 복사 받을 가로, 세로 길이.
							tFrameNumber_Sprite.iCY,
							hMemDC,											// 비트맵을 가지고 있는 DC
							tFrameNumber_Sprite.iCX * ((iMpChar) % 10),		// 출력하려는 스트라이트 이미지 내에서의 좌표
							tFrameNumber_Sprite.iCY * ((iMpChar) / 10),
							tFrameNumber_Sprite.iCX,						// 비트맵을 출력할 가로, 세로 길이
							tFrameNumber_Sprite.iCY,
							RGB(255, 0, 255));								// 제거할 색상
	}

	// 남은 GOLD

	int iGold = pPlayer->Get_Stat(CPlayer::GOLD);
	

	for (int i = 0; i < 8; i++)
	{
		int iGoldChar = (iGold % (int)pow(10, 8 - i)) / (pow(10, 7 - i));							// 출력하려는 수

		GdiTransparentBlt(	hDC,											// 최종적으로 그릴 DC
							492 + i * tFrameNumber_Sprite.iCX,				// 복사받을 위치 X, Y좌표
							80,
							tFrameNumber_Sprite.iCX,						// 복사 받을 가로, 세로 길이.
							tFrameNumber_Sprite.iCY,
							hMemDC,											// 비트맵을 가지고 있는 DC
							tFrameNumber_Sprite.iCX * ((iGoldChar) % 10),		// 출력하려는 스트라이트 이미지 내에서의 좌표
							tFrameNumber_Sprite.iCY * ((iGoldChar) / 10),
							tFrameNumber_Sprite.iCX,						// 비트맵을 출력할 가로, 세로 길이
							tFrameNumber_Sprite.iCY,
							RGB(255, 0, 255));								// 제거할 색상
	}

	// 남은 Boss의 HP

	if (CObjMgr::Get_Instance()->Get_Boss() != nullptr && pPlayer->Get_isBossStart()) // 보스가 있을때만 띄움
	{

		int iBossHp = dynamic_cast<CBoss_HonMeirin*>(CObjMgr::Get_Instance()->Get_Boss())->Get_HP();

		for (int i = 0; i < 5; i++)
		{
			int iBossHpChar = (iBossHp % (int)pow(10, 5 - i)) / (pow(10, 4 - i));							// 출력하려는 수

			GdiTransparentBlt(hDC,											// 최종적으로 그릴 DC
				716 + i * tFrameNumber_Sprite.iCX,				// 복사받을 위치 X, Y좌표
				80,
				tFrameNumber_Sprite.iCX,						// 복사 받을 가로, 세로 길이.
				tFrameNumber_Sprite.iCY,
				hMemDC,											// 비트맵을 가지고 있는 DC
				tFrameNumber_Sprite.iCX * ((iBossHpChar) % 10),		// 출력하려는 스트라이트 이미지 내에서의 좌표
				tFrameNumber_Sprite.iCY * ((iBossHpChar) / 10),
				tFrameNumber_Sprite.iCX,						// 비트맵을 출력할 가로, 세로 길이
				tFrameNumber_Sprite.iCY,
				RGB(255, 0, 255));								// 제거할 색상
		}

	}




	if (pPlayer->Get_MessageWith() != 0)
	{
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_Message");
	tCurProp = CSpritePropertyMgr::Get_Instance()->Find_Property(L"UI_Message");
	
	GdiTransparentBlt(	hDC,											// 최종적으로 그릴 DC
		 				40,					// 복사받을 위치 X, Y좌표
		 				540,
		 				tCurProp.iCX,									// 복사 받을 가로, 세로 길이.
		 				tCurProp.iCY,
		 				hMemDC,											// 비트맵을 가지고 있는 DC
						0,												// 출력하려는 스트라이트 이미지 내에서의 좌표
						(pPlayer->Get_MessagePic() - 1) * tCurProp.iCY,
						tCurProp.iCX,									// 비트맵을 출력할 가로, 세로 길이
		 				tCurProp.iCY,
		 				RGB(255, 0, 255));								// 제거할 색상

		CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
		CBoss_HonMeirin* pBoss = dynamic_cast<CBoss_HonMeirin*>(CObjMgr::Get_Instance()->Get_Boss());

		if (pPlayer->Get_MessageWith() != 0)
		{
			//  250, 570 쯤에 글씨 출력.,>
			RECT rect = { 250, 570, WINCX - 250, 800 };

			// 배경 투명화, 글자 색 흰색으로
			SetBkMode(hDC, TRANSPARENT);
			SetTextColor(hDC, RGB(255, 255, 255));
			
			switch (pPlayer->Get_MessageWith())
			{
			case 3:	// 아큐
				if (pPlayer->Get_MessageOrder() == 0)
					DrawText(hDC, TEXT("대화 테스트 (아큐)"), -1, &rect, DT_LEFT | DT_WORDBREAK);
				else
				{
					pPlayer->Set_MessageWith(0);
					pPlayer->Set_MessagePic(0);
					pPlayer->Set_MessageOrder(0);
				}
				break;

			case 4: // 니토리
				if (pPlayer->Get_MessageOrder() == 0)
					DrawText(hDC, TEXT("손님, 지금 자기 능력 제대로 못 쓰지? 강한 마력이 담긴 시계가 있는데, 관심 있어?"), -1, &rect, DT_LEFT | DT_WORDBREAK);
				else if (pPlayer->Get_MessageOrder() == 1)
					DrawText(hDC, TEXT("500 골드만 모아와. 그럼 그 가격에 주도록 할 게."), -1, &rect, DT_LEFT | DT_WORDBREAK);

				else
				{
					pPlayer->Set_MessageWith(0);
					pPlayer->Set_MessagePic(0);
					pPlayer->Set_MessageOrder(0);
				}

				break;

			case 5: // 메이린

				if (pPlayer->Get_MessageOrder() == 0)
				{
					CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
					CSoundMgr::Get_Instance()->PlayBGM(L"ivent02.ogg", 0.1f);
					pPlayer->Set_MessageOrder(pPlayer->Get_MessageOrder() + 1);
				}
				else if (pPlayer->Get_MessageOrder() == 1)
				{
					DrawText(hDC, TEXT("거기까지입니다!"), -1, &rect, DT_LEFT | DT_WORDBREAK);
				}
				else if (pPlayer->Get_MessageOrder() == 2)
					DrawText(hDC, TEXT("이 이상은 한 발짝도 못 들어오십니다!"), -1, &rect, DT_LEFT | DT_WORDBREAK);
				else if (pPlayer->Get_MessageOrder() == 3)
				{
					pPlayer->Set_MessagePic(2);
					DrawText(hDC, TEXT("......뭘 하고 있는 거지, 메이링?"), -1, &rect, DT_LEFT | DT_WORDBREAK);
				}
				else if (pPlayer->Get_MessageOrder() == 4)
				{
					pPlayer->Set_MessagePic(5);
					DrawText(hDC, TEXT("죄송합니다, 아가씨께서 내리신 명령이라......"), -1, &rect, DT_LEFT | DT_WORDBREAK);
				}
				else if (pPlayer->Get_MessageOrder() == 5)
				{
					pPlayer->Set_MessagePic(2);
					DrawText(hDC, TEXT("아하 그렇군, 당신은 진짜인 모양이네."), -1, &rect, DT_LEFT | DT_WORDBREAK);
				}
				else if (pPlayer->Get_MessageOrder() == 6)
				{
					pPlayer->Set_MessagePic(5);
					DrawText(hDC, TEXT("꼭 지나가야겠다면 저를 쓰러뜨리고 가세요!"), -1, &rect, DT_LEFT | DT_WORDBREAK);
				}
				else if (pPlayer->Get_MessageOrder() == 7)
				{
					pPlayer->Set_MessagePic(2);
					DrawText(hDC, TEXT("평상시라면 당신한테 질 일은 없겠지만......"), -1, &rect, DT_LEFT | DT_WORDBREAK);
				}
				else if (pPlayer->Get_MessageOrder() == 8)
				{
					pPlayer->Set_MessagePic(2);
					DrawText(hDC, TEXT("지금 상태라면 조금 힘겨운 싸움이 될 것 같네."), -1, &rect, DT_LEFT | DT_WORDBREAK);
				}
				else if (pPlayer->Get_MessageOrder() == 9)
				{
					pPlayer->Set_MessagePic(5);
					DrawText(hDC, TEXT("아가씨께서 봐줄 필요 없다고 하셨으니, 각오하십시오!"), -1, &rect, DT_LEFT | DT_WORDBREAK);
				}
				else if (pPlayer->Get_MessageOrder() == 10)
				{
					pPlayer->Set_MessagePic(2);
					DrawText(hDC, TEXT("봐줄 것 없어. 자, 덤벼."), -1, &rect, DT_LEFT | DT_WORDBREAK);
				}
				else // 대화 종료, 보스전 시작
				{
					pPlayer->Set_MessageWith(0);
					pPlayer->Set_MessagePic(0);
					pPlayer->Set_MessageOrder(0);
					pPlayer->Set_isBossStart(true);
					pBoss->Set_Pattern(1);
					CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
					CSoundMgr::Get_Instance()->PlayBGM(L"boss00.ogg", 0.1f);

					// 여기에 시작 스프라이트 넣으면 될듯
				}

				break;

			default:
				break;
			}
			

			
		}


	}


}

void CUI::Release()
{
}

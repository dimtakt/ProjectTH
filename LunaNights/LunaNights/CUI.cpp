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
// !!!	UI �ϳ� �߰��� �� ���� �ش� FRAME ���� ��
// !!!	������ ���� Zeromemory �߰��� ��!!
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

	//.. �÷��� �� �� �� �߾��� UI
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/UI/gauge_sprite/guage_sprite.bmp", L"UI_Gauge_Sprite");
	FRAME_PROP tUI_Guage_Sprite = { 576 * 2, 96 * 2, 1, 2, 2 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tUI_Guage_Sprite, L"UI_Gauge_Sprite");

	// CUI �ϳ����� ��� ������ UI�� ������ ���̱� ������,
	// ��������Ʈ�� ��ȭ �����ϴ� UI�� Insert_Property�� ������� �ʰ�
	// ���� ������ �����ص״ٰ�(���� ������ ��������) ������
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


	// ��ȭâ
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/_non_grouped_things/message_window_sprite/message_window_sprite.bmp", L"UI_Message");
	FRAME_PROP tUI_Message = { 1200, 150, 1, 5, 5 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tUI_Message, L"UI_Message");



	Set_Pos(WINCX / 2, WINCY / 2);

	m_eRender = RENDER_UI;


}

int CUI::Update()
{
	CSceneMgr::SCENEID tCurScene = CSceneMgr::Get_Instance()->Get_CurScene();
	
	// �������������� ���̵��� ��
	if (tCurScene == CSceneMgr::SCENEID::SC_LOGO ||
		tCurScene == CSceneMgr::SCENEID::SC_END ||
		tCurScene == CSceneMgr::SCENEID::SC_EDIT)
		return OBJ_NOEVENT;




	// MoveFrame���� ��������Ʈ ����	
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

	// �������������� ���̵��� ��
	if (tCurScene == CSceneMgr::SCENEID::SC_LOGO ||
		tCurScene == CSceneMgr::SCENEID::SC_END ||
		tCurScene == CSceneMgr::SCENEID::SC_EDIT)
		return;



	HDC	hMemDC;
	FRAME_PROP tCurProp = {};
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	if (!pPlayer)
		return;





	// ���� HP (ü��)
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_HP");

	StretchBlt(			hDC,											// ���������� �׸� DC
		 				212,												// ������� ��ġ X, Y��ǥ
		 				80,
		 				tFrameHPMP.iCX * ((float)pPlayer->Get_Stat(CPlayer::HP) / (float)pPlayer->Get_Stat(CPlayer::MAXHP)),									// ���� ���� ����, ���� ����.
		 				tFrameHPMP.iCY,
		 				hMemDC,											// ��Ʈ���� ������ �ִ� DC
						tFrameHPMP.iCX * ((tFrameHPMP.iFrameCur) % (tFrameHPMP.iFrameMaxX)),	// ����Ϸ��� ��Ʈ����Ʈ �̹��� �������� ��ǥ
						tFrameHPMP.iCY * ((tFrameHPMP.iFrameCur) / (tFrameHPMP.iFrameMaxX)),	// ����Ϸ��� ��Ʈ����Ʈ �̹��� �������� ��ǥ
						tFrameHPMP.iCX,									// ��Ʈ���� ����� ����, ���� ����
		 				tFrameHPMP.iCY,
		 				SRCCOPY);										// ���� ���


	// ���� MP (����)
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_MP");

	StretchBlt(			hDC,											// ���������� �׸� DC
		 				212,												// ������� ��ġ X, Y��ǥ
		 				104,
		 				tFrameHPMP.iCX * ((float)pPlayer->Get_Stat(CPlayer::MP) / (float)pPlayer->Get_Stat(CPlayer::MAXMP)),									// ���� ���� ����, ���� ����.
		 				tFrameHPMP.iCY,
		 				hMemDC,											// ��Ʈ���� ������ �ִ� DC
						tFrameHPMP.iCX * ((tFrameHPMP.iFrameCur) % (tFrameHPMP.iFrameMaxX)),	// ����Ϸ��� ��Ʈ����Ʈ �̹��� �������� ��ǥ
						tFrameHPMP.iCY * ((tFrameHPMP.iFrameCur) / (tFrameHPMP.iFrameMaxX)),	// ����Ϸ��� ��Ʈ����Ʈ �̹��� �������� ��ǥ
						tFrameHPMP.iCX,									// ��Ʈ���� ����� ����, ���� ����
		 				tFrameHPMP.iCY,
		 				SRCCOPY);										// ���� ���
	



	// ��ܿ� ���� UI Ʋ
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_Gauge_Sprite");
	tCurProp = CSpritePropertyMgr::Get_Instance()->Find_Property(L"UI_Gauge_Sprite");
	
	GdiTransparentBlt(	hDC,											// ���������� �׸� DC
		 				WINCX / 2 - tCurProp.iCX / 2,					// ������� ��ġ X, Y��ǥ
		 				0,
		 				tCurProp.iCX,									// ���� ���� ����, ���� ����.
		 				tCurProp.iCY,
		 				hMemDC,											// ��Ʈ���� ������ �ִ� DC
						0,												// ����Ϸ��� ��Ʈ����Ʈ �̹��� �������� ��ǥ
						pPlayer->Get_Stat(CPlayer::ISGETWATCH) * tCurProp.iCY,
						tCurProp.iCX,									// ��Ʈ���� ����� ����, ���� ����
		 				tCurProp.iCY,
		 				RGB(255, 0, 255));								// ������ ����


	// ���� ���� (Gold �Ʒ� STOP, SNAIL, MOVE ����â)
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_Gauge_Stop");

	GdiTransparentBlt(	hDC,											// ���������� �׸� DC
		 				469,					// ������� ��ġ X, Y��ǥ
		 				102,
		 				tFrameGauge_Stop.iCX,									// ���� ���� ����, ���� ����.
		 				tFrameGauge_Stop.iCY,
		 				hMemDC,											// ��Ʈ���� ������ �ִ� DC
						tFrameGauge_Stop.iCX * ((tFrameGauge_Stop.iFrameCur) % (tFrameGauge_Stop.iFrameMaxX)),												// ����Ϸ��� ��Ʈ����Ʈ �̹��� �������� ��ǥ
						tFrameGauge_Stop.iCY * ((tFrameGauge_Stop.iFrameCur) / (tFrameGauge_Stop.iFrameMaxX)),
						tFrameGauge_Stop.iCX,									// ��Ʈ���� ����� ����, ���� ����
		 				tFrameGauge_Stop.iCY,
		 				RGB(255, 0, 255));								// ������ ����


	// ���� TP (�ð�)
	if (pPlayer->Get_Stat(CPlayer::ISGETWATCH) == TRUE)
	{
		int iTp = pPlayer->Get_Stat(CPlayer::TP);
		int iTpCharNum = log10(iTp) + 1;	// �ڸ���
		if (iTpCharNum <= 0) iTpCharNum = 1;

		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_Time_Number");

		int iFirstCharX = 605 + (tFrameTime_Number.iCX / 2) * (3 - iTpCharNum);	// �ڸ����� ���� ��ġ ����

		for (int i = 0; i < iTpCharNum; i++) // �ڸ�����ŭ�� �����ð� ǥ��
		{
			int iTpChar = (iTp % (int)pow(10, i + 1)) / (pow(10, i));							// ����Ϸ��� ��
			if (iTp <= 0) iTpChar = 0;

			GdiTransparentBlt(	hDC,											// ���������� �׸� DC
								605 + (iTpCharNum * 0.5 - i + 0.5) * tFrameTime_Number.iCX,	// ������� ��ġ X, Y��ǥ
		 						103,
		 						tFrameTime_Number.iCX,									// ���� ���� ����, ���� ����.
		 						tFrameTime_Number.iCY,
		 						hMemDC,											// ��Ʈ���� ������ �ִ� DC
								tFrameTime_Number.iCX * ((iTpChar) % (tFrameTime_Number.iFrameMaxX)),												// ����Ϸ��� ��Ʈ����Ʈ �̹��� �������� ��ǥ
								tFrameTime_Number.iCY * ((iTpChar) / (tFrameTime_Number.iFrameMaxX)),
								tFrameTime_Number.iCX,									// ��Ʈ���� ����� ����, ���� ����
		 						tFrameTime_Number.iCY,
		 						RGB(255, 0, 255));								// ������ ����
		}

	}



	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_Number_Sprite");

	// ���� HP

	int iHp = pPlayer->Get_Stat(CPlayer::HP);

	for (int i = 0; i < 3; i++)
	{
		int iHpChar = (iHp % (int)pow(10, 3 - i)) / (pow(10, 2 - i));							// ����Ϸ��� ��

		GdiTransparentBlt(	hDC,											// ���������� �׸� DC
							184 + i * tFrameNumber_Sprite.iCX,				// ������� ��ġ X, Y��ǥ
							80,
							tFrameNumber_Sprite.iCX,						// ���� ���� ����, ���� ����.
							tFrameNumber_Sprite.iCY,
							hMemDC,											// ��Ʈ���� ������ �ִ� DC
							tFrameNumber_Sprite.iCX * ((iHpChar) % 10),		// ����Ϸ��� ��Ʈ����Ʈ �̹��� �������� ��ǥ
							tFrameNumber_Sprite.iCY * ((iHpChar) / 10),
							tFrameNumber_Sprite.iCX,						// ��Ʈ���� ����� ����, ���� ����
							tFrameNumber_Sprite.iCY,
							RGB(255, 0, 255));								// ������ ����
	}

	// ���� MP

	int iMp = pPlayer->Get_Stat(CPlayer::MP);

	for (int i = 0; i < 3; i++)
	{
		int iMpChar = (iMp % (int)pow(10, 3 - i)) / (pow(10, 2 - i));							// ����Ϸ��� ��

		GdiTransparentBlt(	hDC,											// ���������� �׸� DC
							184 + i * tFrameNumber_Sprite.iCX,				// ������� ��ġ X, Y��ǥ
							104,
							tFrameNumber_Sprite.iCX,						// ���� ���� ����, ���� ����.
							tFrameNumber_Sprite.iCY,
							hMemDC,											// ��Ʈ���� ������ �ִ� DC
							tFrameNumber_Sprite.iCX * ((iMpChar) % 10),		// ����Ϸ��� ��Ʈ����Ʈ �̹��� �������� ��ǥ
							tFrameNumber_Sprite.iCY * ((iMpChar) / 10),
							tFrameNumber_Sprite.iCX,						// ��Ʈ���� ����� ����, ���� ����
							tFrameNumber_Sprite.iCY,
							RGB(255, 0, 255));								// ������ ����
	}

	// ���� GOLD

	int iGold = pPlayer->Get_Stat(CPlayer::GOLD);
	

	for (int i = 0; i < 8; i++)
	{
		int iGoldChar = (iGold % (int)pow(10, 8 - i)) / (pow(10, 7 - i));							// ����Ϸ��� ��

		GdiTransparentBlt(	hDC,											// ���������� �׸� DC
							492 + i * tFrameNumber_Sprite.iCX,				// ������� ��ġ X, Y��ǥ
							80,
							tFrameNumber_Sprite.iCX,						// ���� ���� ����, ���� ����.
							tFrameNumber_Sprite.iCY,
							hMemDC,											// ��Ʈ���� ������ �ִ� DC
							tFrameNumber_Sprite.iCX * ((iGoldChar) % 10),		// ����Ϸ��� ��Ʈ����Ʈ �̹��� �������� ��ǥ
							tFrameNumber_Sprite.iCY * ((iGoldChar) / 10),
							tFrameNumber_Sprite.iCX,						// ��Ʈ���� ����� ����, ���� ����
							tFrameNumber_Sprite.iCY,
							RGB(255, 0, 255));								// ������ ����
	}

	// ���� Boss�� HP

	if (CObjMgr::Get_Instance()->Get_Boss() != nullptr && pPlayer->Get_isBossStart()) // ������ �������� ���
	{

		int iBossHp = dynamic_cast<CBoss_HonMeirin*>(CObjMgr::Get_Instance()->Get_Boss())->Get_HP();

		for (int i = 0; i < 5; i++)
		{
			int iBossHpChar = (iBossHp % (int)pow(10, 5 - i)) / (pow(10, 4 - i));							// ����Ϸ��� ��

			GdiTransparentBlt(hDC,											// ���������� �׸� DC
				716 + i * tFrameNumber_Sprite.iCX,				// ������� ��ġ X, Y��ǥ
				80,
				tFrameNumber_Sprite.iCX,						// ���� ���� ����, ���� ����.
				tFrameNumber_Sprite.iCY,
				hMemDC,											// ��Ʈ���� ������ �ִ� DC
				tFrameNumber_Sprite.iCX * ((iBossHpChar) % 10),		// ����Ϸ��� ��Ʈ����Ʈ �̹��� �������� ��ǥ
				tFrameNumber_Sprite.iCY * ((iBossHpChar) / 10),
				tFrameNumber_Sprite.iCX,						// ��Ʈ���� ����� ����, ���� ����
				tFrameNumber_Sprite.iCY,
				RGB(255, 0, 255));								// ������ ����
		}

	}




	if (pPlayer->Get_MessageWith() != 0)
	{
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_Message");
	tCurProp = CSpritePropertyMgr::Get_Instance()->Find_Property(L"UI_Message");
	
	GdiTransparentBlt(	hDC,											// ���������� �׸� DC
		 				40,					// ������� ��ġ X, Y��ǥ
		 				540,
		 				tCurProp.iCX,									// ���� ���� ����, ���� ����.
		 				tCurProp.iCY,
		 				hMemDC,											// ��Ʈ���� ������ �ִ� DC
						0,												// ����Ϸ��� ��Ʈ����Ʈ �̹��� �������� ��ǥ
						(pPlayer->Get_MessagePic() - 1) * tCurProp.iCY,
						tCurProp.iCX,									// ��Ʈ���� ����� ����, ���� ����
		 				tCurProp.iCY,
		 				RGB(255, 0, 255));								// ������ ����

		CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
		CBoss_HonMeirin* pBoss = dynamic_cast<CBoss_HonMeirin*>(CObjMgr::Get_Instance()->Get_Boss());

		if (pPlayer->Get_MessageWith() != 0)
		{
			//  250, 570 �뿡 �۾� ���.,>
			RECT rect = { 250, 570, WINCX - 250, 800 };

			// ��� ����ȭ, ���� �� �������
			SetBkMode(hDC, TRANSPARENT);
			SetTextColor(hDC, RGB(255, 255, 255));
			
			switch (pPlayer->Get_MessageWith())
			{
			case 3:	// ��ť
				if (pPlayer->Get_MessageOrder() == 0)
					DrawText(hDC, TEXT("��ȭ �׽�Ʈ (��ť)"), -1, &rect, DT_LEFT | DT_WORDBREAK);
				else
				{
					pPlayer->Set_MessageWith(0);
					pPlayer->Set_MessagePic(0);
					pPlayer->Set_MessageOrder(0);
				}
				break;

			case 4: // ���丮
				if (pPlayer->Get_MessageOrder() == 0)
					DrawText(hDC, TEXT("�մ�, ���� �ڱ� �ɷ� ����� �� ����? ���� ������ ��� �ð谡 �ִµ�, ���� �־�?"), -1, &rect, DT_LEFT | DT_WORDBREAK);
				else if (pPlayer->Get_MessageOrder() == 1)
					DrawText(hDC, TEXT("500 ��常 ��ƿ�. �׷� �� ���ݿ� �ֵ��� �� ��."), -1, &rect, DT_LEFT | DT_WORDBREAK);

				else
				{
					pPlayer->Set_MessageWith(0);
					pPlayer->Set_MessagePic(0);
					pPlayer->Set_MessageOrder(0);
				}

				break;

			case 5: // ���̸�

				if (pPlayer->Get_MessageOrder() == 0)
				{
					CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
					CSoundMgr::Get_Instance()->PlayBGM(L"ivent02.ogg", 0.1f);
					pPlayer->Set_MessageOrder(pPlayer->Get_MessageOrder() + 1);
				}
				else if (pPlayer->Get_MessageOrder() == 1)
				{
					DrawText(hDC, TEXT("�ű�����Դϴ�!"), -1, &rect, DT_LEFT | DT_WORDBREAK);
				}
				else if (pPlayer->Get_MessageOrder() == 2)
					DrawText(hDC, TEXT("�� �̻��� �� ��¦�� �� �����ʴϴ�!"), -1, &rect, DT_LEFT | DT_WORDBREAK);
				else if (pPlayer->Get_MessageOrder() == 3)
				{
					pPlayer->Set_MessagePic(2);
					DrawText(hDC, TEXT("......�� �ϰ� �ִ� ����, ���̸�?"), -1, &rect, DT_LEFT | DT_WORDBREAK);
				}
				else if (pPlayer->Get_MessageOrder() == 4)
				{
					pPlayer->Set_MessagePic(5);
					DrawText(hDC, TEXT("�˼��մϴ�, �ư������� ������ ����̶�......"), -1, &rect, DT_LEFT | DT_WORDBREAK);
				}
				else if (pPlayer->Get_MessageOrder() == 5)
				{
					pPlayer->Set_MessagePic(2);
					DrawText(hDC, TEXT("���� �׷���, ����� ��¥�� ����̳�."), -1, &rect, DT_LEFT | DT_WORDBREAK);
				}
				else if (pPlayer->Get_MessageOrder() == 6)
				{
					pPlayer->Set_MessagePic(5);
					DrawText(hDC, TEXT("�� �������߰ڴٸ� ���� �����߸��� ������!"), -1, &rect, DT_LEFT | DT_WORDBREAK);
				}
				else if (pPlayer->Get_MessageOrder() == 7)
				{
					pPlayer->Set_MessagePic(2);
					DrawText(hDC, TEXT("���ö�� ������� �� ���� ��������......"), -1, &rect, DT_LEFT | DT_WORDBREAK);
				}
				else if (pPlayer->Get_MessageOrder() == 8)
				{
					pPlayer->Set_MessagePic(2);
					DrawText(hDC, TEXT("���� ���¶�� ���� ���ܿ� �ο��� �� �� ����."), -1, &rect, DT_LEFT | DT_WORDBREAK);
				}
				else if (pPlayer->Get_MessageOrder() == 9)
				{
					pPlayer->Set_MessagePic(5);
					DrawText(hDC, TEXT("�ư������� ���� �ʿ� ���ٰ� �ϼ�����, �����Ͻʽÿ�!"), -1, &rect, DT_LEFT | DT_WORDBREAK);
				}
				else if (pPlayer->Get_MessageOrder() == 10)
				{
					pPlayer->Set_MessagePic(2);
					DrawText(hDC, TEXT("���� �� ����. ��, ����."), -1, &rect, DT_LEFT | DT_WORDBREAK);
				}
				else // ��ȭ ����, ������ ����
				{
					pPlayer->Set_MessageWith(0);
					pPlayer->Set_MessagePic(0);
					pPlayer->Set_MessageOrder(0);
					pPlayer->Set_isBossStart(true);
					pBoss->Set_Pattern(1);
					CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
					CSoundMgr::Get_Instance()->PlayBGM(L"boss00.ogg", 0.1f);

					// ���⿡ ���� ��������Ʈ ������ �ɵ�
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

#include "pch.h"
#include "CEffect.h"

#include "CBmpMgr.h"
#include "CSpritePropertyMgr.h"
#include "CSceneMgr.h"
#include "CPlayer.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"


CEffect::CEffect() :
	isLoop(false),
	fLoopTimes(0),
	iDamageType(0),
    iDamageAmount(0),
	dwStartTime(0)
{
	//ZeroMemory(&, sizeof(FRAME));
}

CEffect::~CEffect()
{
	Release();
}

void CEffect::Initialize()
{
	// LoadImages
	// 이펙트 스프라이트는 여기에서 로드
	// FRAME_PROP 은 로컬 변수로 저장하여 써야할 듯
	
	// 파괴 시의 폭발 이벤트
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Effects/bomb_effect/bomb_effect.bmp", L"Effect_Bomb");
	FRAME_PROP tEffect_Bomb = { 46 * 2, 46 * 2, 4, 3, 12 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tEffect_Bomb, L"Effect_Bomb");
	// 피격 이펙트
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Effects/damage_effect/damage_effect.bmp", L"Effect_Damage");
	FRAME_PROP tEffect_Damage = { 180 * 2, 180 * 2, 4, 2, 8 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tEffect_Damage, L"Effect_Damage");
	// 공격 적중 이펙트
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Effects/impact_sprite/impact_sprite.bmp", L"Effect_Impact");
	FRAME_PROP tEffect_Impact = { 180 * 2, 180 * 2, 4, 2, 8 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tEffect_Impact, L"Effect_Impact");



	// MP 소모 이펙트
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Effects/mp_down_sprite/mp_down_sprite.bmp", L"Effect_MPDown");
	FRAME_PROP tEffect_MPDown = { 128 * 2, 17 * 2, 1, 6, 6 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tEffect_MPDown, L"Effect_MPDown");
	// TP 소모 이펙트
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Effects/time_down_sprite/time_down_sprite.bmp", L"Effect_TPDown");
	FRAME_PROP tEffect_TPDown = { 80 * 2, 80 * 2, 6, 1, 6 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tEffect_TPDown, L"Effect_TPDown");
	// 스킬 사용 이펙트
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Effects/ring_effect_sprite/ring_effect_sprite.bmp", L"Effect_SkillUse");
	FRAME_PROP tEffect_SkillUse = { 192 * 2, 192 * 2, 4, 4, 16 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tEffect_SkillUse, L"Effect_SkillUse");
	// 공격 이펙트
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Effects/arrow_ring_sprite/arrow_ring_sprite.bmp", L"Effect_AtkRing");
	FRAME_PROP tEffect_AtkRing = { 48 * 2, 96 * 2, 8, 1, 8 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tEffect_AtkRing, L"Effect_AtkRing");



	// 모래먼지 이펙트
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirinBullet/honmeirin_bullet_smoke/honmeirin_bullet_smoke.bmp", L"Effect_DustSmoke");
	FRAME_PROP tEffect_DustSmoke = { 64 * 2, 64 * 2, 4, 4, 14, 7 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tEffect_DustSmoke, L"Effect_DustSmoke");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/HonMeirinBullet/honmeirin_bullet_smoke/honmeirin_bullet_smoke_R.bmp", L"Effect_DustSmoke_R");
	FRAME_PROP tEffect_DustSmoke_R = { 64 * 2, 64 * 2, 4, 4, 14, 7 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tEffect_DustSmoke_R, L"Effect_DustSmoke_R");

	

	// 숫자 폰트
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/UI/number_status_font_merged/number_status_font_merged.bmp", L"Effect_Number");
	FRAME_PROP tEffect_Number = { 7 * 2, 8 * 2, 10, 9, 86 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tEffect_Number, L"Effect_Number");
	// 스테이터스
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/UI/status_vision_sprite/status_vision_sprite.bmp", L"Effect_Status");
	FRAME_PROP tEffect_Status = { 81 * 2, 8 * 2, 1, 14, 14 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tEffect_Status, L"Effect_Status");



	dwStartTime = GetTickCount();

	Set_Pos(WINCX / 2, WINCY / 2);

	m_eRender = RENDER_EFFECT;

}

int CEffect::Update()
{
	if (m_isDead)
		return OBJ_DEAD;


	if (m_pFrameKey == L"Effect_TPDown" ||
		m_pFrameKey == L"Effect_MPDown")
		m_eRender = RENDER_UI_EFFECT;



	CSceneMgr::SCENEID tCurScene = CSceneMgr::Get_Instance()->Get_CurScene();

	// 스테이지에서만 보이도록 함
	if (tCurScene == CSceneMgr::SCENEID::SC_LOGO ||
		tCurScene == CSceneMgr::SCENEID::SC_END ||
		tCurScene == CSceneMgr::SCENEID::SC_EDIT)
		return OBJ_NOEVENT;


	m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(m_pFrameKey);
	Set_FrameProperty(m_tFramePropCur);	// 현재 프레임에 현 상태에 맞는 정보를 반영
	Set_Scale(m_tFrame.iCX, m_tFrame.iCY);
	

	if (m_pFrameKey == L"Effect_SkillUse")
		m_tFrame.dwSpeed = 20.f;
	else if (m_pFrameKey == L"Effect_Impact" ||
			m_pFrameKey == L"Effect_Damage")
		m_tFrame.dwSpeed = 30.f;
	else
		m_tFrame.dwSpeed = 40.f; // 프레임 전환 속도








	__super::Update_Rect();


	return OBJ_NOEVENT;
}

void CEffect::Late_Update()
{
	// 이펙트 출력이 끝까지 되면 삭제

	if (m_pFrameKey == L"Effect_Number" ||
		m_pFrameKey == L"Effect_Status" ||
		(isLoop && fLoopTimes != 0))
	{
		if (dwStartTime + fLoopTimes * 1000 < GetTickCount())
			m_isDead = true;
	}
	else
	{
		if (m_tFrame.iFrameCur == m_tFrame.iFrameAmount - 1)
			m_isDead = true;
		
		Move_Frame();
	}

}

void CEffect::Render(HDC hDC)
{
	CSceneMgr::SCENEID tCurScene = CSceneMgr::Get_Instance()->Get_CurScene();

	// 스테이지에서만 보이도록 함
	if (tCurScene == CSceneMgr::SCENEID::SC_LOGO ||
		tCurScene == CSceneMgr::SCENEID::SC_END ||
		tCurScene == CSceneMgr::SCENEID::SC_EDIT)
		return;


	// TransparentBlt 를 이용하여 이펙트 출력
	// 위치는 생성 당시 CObjMgr 을 통해 받아와서 저장될 m_tInfo 를 기반으로 해야할 듯

	if (m_pFrameKey == L"Effect_Number")
	{
		// 받아온 데미지값에 기반하여 루프돌며 데미지 숫자 출력

		int iOutX = m_tRect.left;
		int iOutY = m_tRect.top;
		CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // 최종적으로 렌더시킬 좌표.

		HDC	hMemDC;
		FRAME_PROP tCurProp = {};
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

		int iDmgCharNum = log10(iDamageAmount) + 1;	// 자릿수
		int iFirstCharX = 605 + (m_tFrame.iCX / 2) * (3 - iDmgCharNum);	// 자릿수에 따른 위치 보정


		for (int i = 0; i < iDmgCharNum; i++)
		{
			int iDmgChar = (iDamageAmount % (int)pow(10, i + 1)) / (pow(10, i));

			GdiTransparentBlt(	hDC,					// 최종적으로 그릴 DC
		 						iOutX + (iDmgCharNum * 0.5 - i + 0.5) * m_tFrame.iCX,					// 복사받을 위치 X, Y좌표
		 						iOutY,
								m_tFrame.iCX,	// 복사 받을 가로, 세로 길이.
								m_tFrame.iCY,
		 						hMemDC,					// 비트맵을 가지고 있는 DC
								m_tFrame.iCX * ((iDmgChar) % (m_tFrame.iFrameMaxX)),	// 출력하려는 스트라이트 이미지 내에서의 좌표
								m_tFrame.iCY * iDamageType,
								m_tFrame.iCX,	// 비트맵을 출력할 가로, 세로 길이
								m_tFrame.iCY,
		 						RGB(255, 0, 255));		// 제거할 색상
		}
		return;	// 숫자 이펙트였다면 종료
	}

	else if (m_pFrameKey == L"Effect_Status")
	{
		// 받아온 속성에 기반하여 루프돌며 스테이터스 상태 전환 출력

		int iOutX = m_tRect.left;
		int iOutY = m_tRect.top;
		CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // 최종적으로 렌더시킬 좌표.

		HDC	hMemDC;
		FRAME_PROP tCurProp = {};
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

		GdiTransparentBlt(	hDC,					// 최종적으로 그릴 DC
							iOutX,					// 복사받을 위치 X, Y좌표
							iOutY,
							m_tFrame.iCX,	// 복사 받을 가로, 세로 길이.
							m_tFrame.iCY,
							hMemDC,					// 비트맵을 가지고 있는 DC
							0,	// 출력하려는 스트라이트 이미지 내에서의 좌표
							m_tFrame.iCY * (2 * iStatusType + GetTickCount() % 2),
							m_tFrame.iCX,	// 비트맵을 출력할 가로, 세로 길이
							m_tFrame.iCY,
							RGB(255, 0, 255));		// 제거할 색상
		return;
	}


	if (isWorldFix)
	{
		int iOutX = m_tRect.left;
		int iOutY = m_tRect.top;
		CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // 최종적으로 렌더시킬 좌표.

		HDC	hMemDC;
		FRAME_PROP tCurProp = {};
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

		// m_tFrame 에는 Set_FrameProperty 로 지정해준 FRAME_PROP 값이 들어감

		GdiTransparentBlt(	hDC,					// 최종적으로 그릴 DC
		 					iOutX,					// 복사받을 위치 X, Y좌표
		 					iOutY,
							m_tFrame.iCX,	// 복사 받을 가로, 세로 길이.
							m_tFrame.iCY,
		 					hMemDC,					// 비트맵을 가지고 있는 DC
							m_tFrame.iCX * ((m_tFrame.iFrameCur) % (m_tFrame.iFrameMaxX)),	// 출력하려는 스트라이트 이미지 내에서의 좌표
							m_tFrame.iCY * ((m_tFrame.iFrameCur) / (m_tFrame.iFrameMaxX)),
							m_tFrame.iCX,	// 비트맵을 출력할 가로, 세로 길이
							m_tFrame.iCY,
		 					RGB(255, 0, 255));		// 제거할 색상
	}
	else
	{
		HDC	hMemDC;
		FRAME_PROP tCurProp = {};
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

		// m_tFrame 에는 Set_FrameProperty 로 지정해준 FRAME_PROP 값이 들어감

		GdiTransparentBlt(	hDC,					// 최종적으로 그릴 DC
							m_tInfo.fX,					// 복사받을 위치 X, Y좌표
							m_tInfo.fY,
							m_tFrame.iCX,	// 복사 받을 가로, 세로 길이.
							m_tFrame.iCY,
							hMemDC,					// 비트맵을 가지고 있는 DC
							m_tFrame.iCX * ((m_tFrame.iFrameCur) % (m_tFrame.iFrameMaxX)),	// 출력하려는 스트라이트 이미지 내에서의 좌표
							m_tFrame.iCY * ((m_tFrame.iFrameCur) / (m_tFrame.iFrameMaxX)),
							m_tFrame.iCX,	// 비트맵을 출력할 가로, 세로 길이
							m_tFrame.iCY,
							RGB(255, 0, 255));		// 제거할 색상
	}


	

}

void CEffect::Release()
{
}

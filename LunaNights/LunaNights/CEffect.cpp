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
    iLoopTimes(0)
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
	// MP 소모 이펙트
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Effects/mp_down_sprite/mp_down_sprite.bmp", L"Effect_MPDown");
	FRAME_PROP tEffect_MPDown = { 128 * 2, 17 * 2, 1, 6, 6 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tEffect_MPDown, L"Effect_MPDown");
	// TP 소모 이펙트
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/Effects/time_down_sprite/time_down_sprite.bmp", L"Effect_TPDown");
	FRAME_PROP tEffect_TPDown = { 80 * 2, 80 * 2, 6, 1, 6 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tEffect_TPDown, L"Effect_TPDown");

	



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
	
	m_tFrame.dwSpeed = 40.f; // 프레임 전환 속도








	__super::Update_Rect();


	return OBJ_NOEVENT;
}

void CEffect::Late_Update()
{
	// 이펙트 출력이 끝까지 되면 삭제

	if (m_tFrame.iFrameCur == m_tFrame.iFrameAmount - 1)
		m_isDead = true;

	Move_Frame();

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

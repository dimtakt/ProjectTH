#include "pch.h"
#include "CNpc.h"
#include "CBmpMgr.h"
#include "CSpritePropertyMgr.h"
#include "CCameraMgr.h"

CNpc::CNpc()
{
}

CNpc::~CNpc()
{
	Release();
}

void CNpc::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/_non_grouped_things/akyuu_sprite/akyuu_sprite.bmp", L"STAGEOBJ_AKYUU");
	FRAME_PROP tSTAGEOBJ_AKYUU = { 96 * 2, 96 * 2, 4, 4, 16 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tSTAGEOBJ_AKYUU, L"STAGEOBJ_AKYUU");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resources/_non_grouped_things/nitori_escape/nitori_stand.bmp", L"STAGEOBJ_NITORI");
	FRAME_PROP tSTAGEOBJ_NITORI = { 128 * 2, 96 * 2, 4, 1, 4 };
	CSpritePropertyMgr::Get_Instance()->Insert_Property(tSTAGEOBJ_NITORI, L"STAGEOBJ_NITORI");

	m_tFrame.iFrameCur = 0;
	m_tFrame.dwSpeed = 100;				// 프레임 전환 속도
	m_eRender = RENDER_NPC;


}

int CNpc::Update()
{
	m_tFramePropCur = CSpritePropertyMgr::Get_Instance()->Find_Property(m_pFrameKey);
	Set_FrameProperty(m_tFramePropCur);	// 현재 프레임에 현 상태에 맞는 정보를 반영
	Set_Scale(m_tFramePropCur.iCX, m_tFramePropCur.iCY);

	__super::Update_Rect_UpStand();


	return 0;
}

void CNpc::Late_Update()
{
	Move_Frame();
}

void CNpc::Render(HDC hDC)
{
	int iOutX = m_tRect.left;
	int iOutY = m_tRect.top;
	CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY); // 최종적으로 렌더시킬 좌표.

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

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
}

void CNpc::Release()
{
}

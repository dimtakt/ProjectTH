#pragma once

#include "CObj.h"

class CCollisionMgr
{
public:
	// 사각 충돌이 확인될 시 양 쪽의 오브젝트를 사망처리하는 함수 (IntersectRect 함수 사용)
	static void	Collision_Rect_PlayerMonster(std::list<CObj*> DstList, std::list<CObj*> SrcList);
	static void	Collision_Rect_BulletMonster(std::list<CObj*> DstList, std::list<CObj*> SrcList);

	static void Collision_Rect_BulletPlayer(std::list<CObj*> DstList, std::list<CObj*> SrcList);
	
	// 사각 충돌이 확인될 시 Dst에 의해 Src가 밀려나는 함수
	// 칼날 간 간격을 띄우는 데에 사용해야 할 듯, 아니야, 그건 원 충돌로 하는 편이 낫지 않을까
	static void	Collision_RectEx(std::list<CObj*> DstList, std::list<CObj*> SrcList);
	
	// 충돌 여부를 반환 및 겹치는 사각의 크기를 확인 가능한 함수
	static bool	Check_Rect(CObj* pDst, CObj* pSrc, float* pX, float* pY);

	static bool Check_CollideRect(CObj* pDst, CObj* pSrc, float* pX, float* pY);


	// 원 충돌이 확인될 시 양 쪽의 오브젝트를 사망처리하는 함수
	static void	Collision_Circle(std::list<CObj*> DstList, std::list<CObj*> SrcList);

	// 원 충돌 여부를 반환하는 함수
	static bool	Intersect_Circle(CObj* pDst, CObj* pSrc);

};


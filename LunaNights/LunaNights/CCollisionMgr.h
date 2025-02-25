#pragma once

#include "CObj.h"

class CCollisionMgr
{
public:
	// �簢 �浹�� Ȯ�ε� �� �� ���� ������Ʈ�� ���ó���ϴ� �Լ� (IntersectRect �Լ� ���)
	static void	Collision_Rect_PlayerMonster(std::list<CObj*> DstList, std::list<CObj*> SrcList);
	static void	Collision_Rect_BulletMonster(std::list<CObj*> DstList, std::list<CObj*> SrcList);

	static void Collision_Rect_BulletPlayer(std::list<CObj*> DstList, std::list<CObj*> SrcList);
	
	// �簢 �浹�� Ȯ�ε� �� Dst�� ���� Src�� �з����� �Լ�
	// Į�� �� ������ ���� ���� ����ؾ� �� ��, �ƴϾ�, �װ� �� �浹�� �ϴ� ���� ���� ������
	static void	Collision_RectEx(std::list<CObj*> DstList, std::list<CObj*> SrcList);
	
	// �浹 ���θ� ��ȯ �� ��ġ�� �簢�� ũ�⸦ Ȯ�� ������ �Լ�
	static bool	Check_Rect(CObj* pDst, CObj* pSrc, float* pX, float* pY);

	static bool Check_CollideRect(CObj* pDst, CObj* pSrc, float* pX, float* pY);


	// �� �浹�� Ȯ�ε� �� �� ���� ������Ʈ�� ���ó���ϴ� �Լ�
	static void	Collision_Circle(std::list<CObj*> DstList, std::list<CObj*> SrcList);

	// �� �浹 ���θ� ��ȯ�ϴ� �Լ�
	static bool	Intersect_Circle(CObj* pDst, CObj* pSrc);

};


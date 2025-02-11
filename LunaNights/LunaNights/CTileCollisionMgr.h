#pragma once

#include "CObj.h"
#include "Define.h"


// ������ Ÿ�� ������ ����
// �ݸ����� �ٸ����� �����ϴ� �͵� ����� ��?

class CTileCollisionMgr
{	
public:
	static void Collision_Tile2Obj(std::vector<CObj*> _TileVec, std::list<CObj*> _Obj);
	static bool Check_Collision_Tile2Obj(CObj* _pTile, CObj* _pObj, float* _pX, float* _pY);
	



	static int CCW(POINT _ptA, POINT _ptB, POINT _ptC);
	static bool isCollideLine(POINT _LineA1, POINT _LineA2, POINT _LineB1, POINT _LineB2);

	// �ݶ��̴��� TileID�� ������, �ݶ��̴� ������ 4�� �� ��ǥ�� ��ȯ���ִ� �Լ�
	static COL_POINT GetColPoint(INFO _tileInfo, int iTileIndex);




	//// �簢 �浹�� Ȯ�ε� �� �� ���� ������Ʈ�� ���ó���ϴ� �Լ� (IntersectRect �Լ� ���)
	//static void	Collision_Rect(std::list<CObj*> DstList, std::list<CObj*> SrcList);
	//
	//// �簢 �浹�� Ȯ�ε� �� Dst�� ���� Src�� �з����� �Լ�
	//// Į�� �� ������ ���� ���� ����ؾ� �� ��, 
	//static void	Collision_RectEx(std::list<CObj*> DstList, std::list<CObj*> SrcList);
	//
	//// �浹 ���θ� ��ȯ �� ��ġ�� �簢�� ũ�⸦ Ȯ�� ������ �Լ�
	//static bool	Check_Rect(CObj* pDst, CObj* pSrc, float* pX, float* pY);


	//// �� �浹�� Ȯ�ε� �� �� ���� ������Ʈ�� ���ó���ϴ� �Լ�
	//static void	Collision_Circle(std::list<CObj*> DstList, std::list<CObj*> SrcList);

	//// �� �浹 ���θ� ��ȯ�ϴ� �Լ�
	//static bool	Intersect_Circle(CObj* pDst, CObj* pSrc);
};


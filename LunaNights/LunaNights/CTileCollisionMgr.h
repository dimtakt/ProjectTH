#pragma once

#include "CObj.h"
#include "Define.h"


// 각각의 타일 종류에 따라
// 콜리전이 다르도록 설정하는 것도 방법일 듯?

class CTileCollisionMgr
{	
public:
	static void Collision_Tile2Obj(std::vector<CObj*> _TileVec, std::list<CObj*> _Obj);
	static bool Check_Collision_Tile2Obj(CObj* _pTile, CObj* _pObj, float* _pX, float* _pY);
	



	static int CCW(POINT _ptA, POINT _ptB, POINT _ptC);
	static bool isCollideLine(POINT _LineA1, POINT _LineA2, POINT _LineB1, POINT _LineB2);

	// 콜라이더의 TileID를 넣으면, 콜라이더 기준의 4개 점 좌표를 반환해주는 함수
	static COL_POINT GetColPoint(INFO _tileInfo, int iTileIndex);




	//// 사각 충돌이 확인될 시 양 쪽의 오브젝트를 사망처리하는 함수 (IntersectRect 함수 사용)
	//static void	Collision_Rect(std::list<CObj*> DstList, std::list<CObj*> SrcList);
	//
	//// 사각 충돌이 확인될 시 Dst에 의해 Src가 밀려나는 함수
	//// 칼날 간 간격을 띄우는 데에 사용해야 할 듯, 
	//static void	Collision_RectEx(std::list<CObj*> DstList, std::list<CObj*> SrcList);
	//
	//// 충돌 여부를 반환 및 겹치는 사각의 크기를 확인 가능한 함수
	//static bool	Check_Rect(CObj* pDst, CObj* pSrc, float* pX, float* pY);


	//// 원 충돌이 확인될 시 양 쪽의 오브젝트를 사망처리하는 함수
	//static void	Collision_Circle(std::list<CObj*> DstList, std::list<CObj*> SrcList);

	//// 원 충돌 여부를 반환하는 함수
	//static bool	Intersect_Circle(CObj* pDst, CObj* pSrc);
};


#include "pch.h"
#include "CTileCollisionMgr.h"
#include "CTile.h"

void CTileCollisionMgr::Collision_Tile2Obj(std::vector<CObj*> _TileVec, std::list<CObj*> _ObjList)
{
	float fWidth(0.f), fHeight(0.f);

	for (auto& _Tile : _TileVec)
	{
		for (auto& _Obj : _ObjList)
		{


			// if 충돌하면
			// 플레이어 좌표를 이전으로 돌림
		}
	}
}

// 타일 영역과 오브젝트가 충돌하면 참을 반환
// float 인자는 얼마나 겹치는지를 반환하도록 제작하여 넘친 만큼의 좌표를 재조정함
bool CTileCollisionMgr::Check_Collision_Tile2Obj(CObj* _pTile, CObj* _pObj, float* _pX, float* _pY)
{
	INFO tTileInfo = *_pTile->Get_Info();
	INFO tObjInfo = *_pObj->Get_Info();

	unsigned int iTileID = dynamic_cast<CTile*>(_pTile)->Get_DrawID();	// 이걸로 콜라이더의 생김새를 판별
	COL_POINT tTilePoint = GetColPoint(tTileInfo, iTileID);

	POINT tTilePoints[4] = { tTilePoint.ptLUp, tTilePoint.ptRUp, tTilePoint.ptLDown, tTilePoint.ptRDown };
	POINT tObjPoints[4] = { tObjInfo.fX - tObjInfo.fCX * 0.5f,
							tObjInfo.fX + tObjInfo.fCX * 0.5f,
							tObjInfo.fY - tObjInfo.fCY * 0.5f,
							tObjInfo.fY + tObjInfo.fCY * 0.5f };
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			int iTmp = ((i + 1) >= 4) ? 0 : i + 1;
			int jTmp = ((j + 1) >= 4) ? 0 : j + 1;

			bool isCollided = isCollideLine(	tTilePoints[i], tTilePoints[iTmp],
												tObjPoints[j], tObjPoints[jTmp]);

			if (isCollided)
				return true;
		}
	}

	return false;
}

// CCW 알고리즘
// A, B, C 순서대로 봤을 때, 반시계 방향이면 양수를, 시계 방향이면 음수를, 일직선 상이면 0을 리턴
// 예시 https://puu.sh/Ko2cX/04d85de5ed.png
int CTileCollisionMgr::CCW(POINT _ptA, POINT _ptB, POINT _ptC)
{
	return ((_ptB.x - _ptA.x) * (_ptC.y - _ptA.y) - (_ptB.y - _ptA.y) * (_ptC.x - _ptA.x));
}

// 두 선이 겹치면 참을 반환
bool CTileCollisionMgr::isCollideLine(POINT _LineA1, POINT _LineA2, POINT _LineB1, POINT _LineB2)
{
	// 두 선분을 각각의 시작 기준으로 잡고 확인했을 때,
	// 두 경우 모두, 각각의 경우에 하나는 시계, 하나는 반시계인 경우에 겹친다.
	if (CCW(_LineA1, _LineA2, _LineB1) * CCW(_LineA1, _LineA2, _LineB2) < 0 &&
		CCW(_LineB1, _LineA2, _LineA1) * CCW(_LineB1, _LineA2, _LineB2) < 0)
		return true;

	// 단, 둘 다 0을 리턴. 즉 둘 다 하나의 직선상에 (일직선상에) 위치한 경우 예외 발생
	// 이 경우 포개어짐을 확인한다. 어차피 일직선상에 있으므로 x 하나만 잡고 검사함
	if (CCW(_LineA1, _LineA2, _LineB1) * CCW(_LineA1, _LineA2, _LineB2) == 0 &&
		CCW(_LineB1, _LineA2, _LineA1) * CCW(_LineB1, _LineA2, _LineB2) == 0)
	{
		// 각각 점의 x를 오름차순으로 정렬한 뒤,
		// 겹치는지를 확인
		float fX[4]{ _LineA1.x, _LineA2.x, _LineB1.x, _LineB2.x };

		if (fX[0] > fX[1]) { float fTmp = fX[0]; fX[0] = fX[1]; fX[1] = fTmp; }
		if (fX[2] > fX[3]) { float fTmp = fX[2]; fX[2] = fX[3]; fX[3] = fTmp; }

		// 겹치는 경우를 일직선상에서 보면 (A시작)---(B시작)--(A끝)--(B시작) 과 같은 꼴
		return (fX[2] <= fX[1] && fX[0] <= fX[3]);
	}

	return false;
}

COL_POINT CTileCollisionMgr::GetColPoint(INFO _tileInfo, int _tileIndex)
{
	// 받아온 정보는 중점임에 유의
	// 단 플레이어와 같은 스탠딩 오브젝트는 짚고 있는 땅바닥이 기준

	LONG X	= _tileInfo.fX;
	LONG Y	= _tileInfo.fY;
	LONG CX = _tileInfo.fCX;
	LONG CY = _tileInfo.fCY;
	
	// COL_POINT 의 형태는 {좌상, 우상, 좌하, 우하} 순임
	switch (_tileIndex)
	{
	// 0 은 빈 셀
	// 1~7
	
	//case 0:
	case 1:		return COL_POINT{	{X - CX / 2, Y - CY / 2}, {X + CX / 2, Y - CY / 2},
									{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y + CY / 2} };
	case 2:		return COL_POINT{	{X - CX / 2, Y - CY / 2}, {X + CX / 2, Y},
									{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y + CY / 2} };
	case 3:		return COL_POINT{	{X - CX / 2, Y}			, {X + CX / 2, Y + CY / 2},
									{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y + CY / 2} };
	case 4:		return COL_POINT{	{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y},
									{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y + CY / 2} };
	case 5:		return COL_POINT{	{X - CX / 2, Y}			, {X + CX / 2, Y - CY / 2},
									{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y + CY / 2} };
	case 6:		return COL_POINT{	{X - CX / 2, Y - CY / 2}, {X + CX / 2, Y + CY / 2},
									{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y + CY / 2} };
	case 7:		return COL_POINT{	{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y - CY / 2},
									{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y + CY / 2} };

	// 8~15
	case 8:		return COL_POINT{	{X - CX / 2, Y - CY / 2}, {X + CX / 2, Y - CY / 4},
									{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y + CY / 2} };
	case 9:		return COL_POINT{	{X - CX / 2, Y - CY / 4}, {X + CX / 2, Y},
									{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y + CY / 2} };
	case 10:	return COL_POINT{	{X - CX / 2, Y}			, {X + CX / 2, Y + CY / 4},
									{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y + CY / 2} };
	case 11:	return COL_POINT{	{X - CX / 2, Y + CY / 4}, {X + CX / 2, Y + CY / 2},
									{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y + CY / 2} };
	case 12:	return COL_POINT{	{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y + CY / 4},
									{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y + CY / 2} };
	case 13:	return COL_POINT{	{X - CX / 2, Y + CY / 4}, {X + CX / 2, Y},
									{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y + CY / 2} };
	case 14:	return COL_POINT{	{X - CX / 2, Y}			, {X + CX / 2, Y - CY / 4},
									{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y + CY / 2} };
	case 15:	return COL_POINT{	{X - CX / 2, Y - CY / 4}, {X + CX / 2, Y - CY / 2},
									{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y + CY / 2} };
	


	// 16~23
	case 16:	return COL_POINT{	{X - CX / 2, Y}			, {X + CX / 2, Y},
									{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y + CY / 2} };
	case 17:	return COL_POINT{	{X - CX / 2, Y - CY / 4}, {X + CX / 2, Y - CY / 4},
									{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y + CY / 2} };
	case 18:	return COL_POINT{	{X - CX / 2, Y - CY / 2}, {X + CX / 2, Y - CY / 2},
									{X - CX / 2, Y}			, {X + CX / 2, Y} };
	case 19:	return COL_POINT{	{X - CX / 2, Y - CY / 2}, {X + CX / 2, Y - CY / 2},
									{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y - CY / 2} };
	case 20:	return COL_POINT{	{X - CX / 2, Y - CY / 2}, {X + CX / 2, Y - CY / 2},
									{X - CX / 2, Y - CY / 2}, {X + CX / 2, Y + CY / 2} };
	case 21:	return COL_POINT{	{X - CX / 2, Y - CY / 2}, {X + CX / 2, Y + CY / 2},
									{X - CX / 2, Y}			, {X, Y + CY / 2} };
	case 22:	return COL_POINT{	{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y - CY / 2},
									{X, Y + CY / 2}			, {X + CX / 2, Y} };
	case 23:	return COL_POINT{	{X, Y - CY / 2}			, {X + CX / 2, Y - CY / 2},
									{X, Y - CY / 2}			, {X + CX / 2, Y} };

	
		   
		   
	// 24~30
	case 24:	return COL_POINT{	{X - CX / 2, Y - CY / 2}, {X, Y - CY / 2},
									{X - CX / 2, Y}			, {X, Y - CY / 2} };
	case 25:	return COL_POINT{	{X, Y - CY / 2}			, {X + CX / 2, Y - CY / 2},
									{X, Y + CY / 2}			, {X + CX / 2, Y + CY / 2} };
	case 26:	return COL_POINT{	{X - CX / 2, Y - CY / 2}, {X, Y - CY / 2},
									{X - CX / 2, Y + CY / 2}, {X, Y + CY / 2} };
	case 27:	return COL_POINT{	{X, Y + CY / 2}			, {X + CX / 2, Y},			// 이거 해야함
									{X, Y + CY / 2}			, {X + CX / 2, Y + CY / 2} };
	case 28:	return COL_POINT{	{X - CX / 2, Y}			, {X, Y + CY / 2},
									{X - CX / 2, Y + CY / 2}, {X, Y + CY / 2} };
	case 29:	return COL_POINT{	{X - CX / 2, Y}			, {X, Y - CY / 2},
									{X - CX / 2, Y + CY / 2}, {X + CX / 2, Y - CY / 2} };
	case 30:	return COL_POINT{	{X, Y - CY / 2}			, {X + CX / 2, Y},
									{X - CX / 2, Y - CY / 2}, {X + CX / 2, Y + CY / 2} };
	
	}

	return COL_POINT{ 0,0,0,0,false };
}

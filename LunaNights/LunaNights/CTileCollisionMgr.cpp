#include "pch.h"
#include "CTileCollisionMgr.h"
#include "CCollisionMgr.h"
#include "CCameraMgr.h"
#include "CTileMgr.h"
#include "CTile.h"

void CTileCollisionMgr::Collision_Tile2Obj(std::vector<CObj*> _TileVec, std::list<CObj*> _ObjList)
{
	float fWidth(0.f), fHeight(0.f);

	for (auto& _Tile : _TileVec)
	{
		for (auto& _Obj : _ObjList)
		{
			if (Check_Collision_Tile2Obj(_Tile, _Obj, nullptr, nullptr))
			{
				// if 충돌하면
				// 플레이어 좌표를 이전으로 돌리든 뭐든..
				// 이 부분은 나중에 구현.. ksta
				
			}

		}
	}
}

// 타일 영역과 오브젝트가 충돌하면 참을 반환
// float 인자는 얼마나 겹치는지를 반환하도록 제작하여 넘친 만큼의 좌표를 재조정함
bool CTileCollisionMgr::Check_Collision_Tile2Obj(CObj* _pTile, CObj* _pObj, float* _pX, float* _pY)
{
	CTile* tmpTile = dynamic_cast<CTile*>(_pTile);

	INFO tTileInfo = *tmpTile->Get_Info();
	INFO tObjInfo = *_pObj->Get_CollideInfo();

	unsigned int iTileID = tmpTile->Get_DrawID();	// 이걸로 콜라이더의 생김새를 판별
	if (!(1 <= iTileID && iTileID <= 30)) return false; // 만약 대상 타일이 콜라이더가 존재하지 않을 경우, 즉시 false 리턴함
	
	COL_POINT tTilePoint = GetColPoint(tTileInfo, iTileID);


	
	// 우선 AABB 충돌 검사 실행
	if (CCollisionMgr::Check_Rect(_pTile, _pObj, nullptr, nullptr)) {
		// AABB 충돌 검사에서 충돌이 확인되었다면, 상세 충돌 검사 실행
		// 상세 충돌 검사 (CCW 알고리즘을 이용하여 각 타일을 구성하는 선 마다의 충돌 여부 반환)
		
		// 계산의 편의성을 위해 배열에 담음
		POINT tObjPoints[4] = { {tObjInfo.fX - tObjInfo.fCX * 0.5f, tObjInfo.fY - tObjInfo.fCY * 0.5f},		// 좌상
								{tObjInfo.fX + tObjInfo.fCX * 0.5f, tObjInfo.fY - tObjInfo.fCY * 0.5f},		// 우상
								{tObjInfo.fX + tObjInfo.fCX * 0.5f, tObjInfo.fY + tObjInfo.fCY * 0.5f},		// 우하
								{tObjInfo.fX - tObjInfo.fCX * 0.5f, tObjInfo.fY + tObjInfo.fCY * 0.5f} };	// 좌하
		// 상세 충돌 검사용 배열
		POINT tTilePoints[4] = { tTilePoint.ptLUp, tTilePoint.ptRUp, tTilePoint.ptRDown, tTilePoint.ptLDown };	// 좌상, 우상, 우하, 좌하

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				int iTmp = ((i + 1) >= 4) ? 0 : i + 1;
				int jTmp = ((j + 1) >= 4) ? 0 : j + 1;

				bool isCollided = isCollideLine(tTilePoints[i], tTilePoints[iTmp], tObjPoints[j], tObjPoints[jTmp]);
				if (isCollided)		return true;
			}
		}
	}
	// 모든 검사 이후에 충돌 확인이 되지 않았다면, false 반환
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
	// ** 예외사항 1
	// 만약 비교 대상 중 점이 존재하는 경우 (선 중 하나의 1, 2 위치가 같음)
	// 충돌 검사에 문제 발생하므로, 검사 X (즉시 false 리턴)
	if ((_LineA1.x == _LineA2.x && _LineA1.y == _LineA2.y) ||
		(_LineB1.x == _LineB2.x && _LineB1.y == _LineB2.y))
		return false;

	// 두 선분을 각각의 시작 기준으로 잡고 확인했을 때,
	// 두 경우 모두, 각각의 경우에 하나는 시계, 하나는 반시계인 경우에 겹친다.
	if (CCW(_LineA1, _LineA2, _LineB1) * CCW(_LineA1, _LineA2, _LineB2) <= 0 &&
		CCW(_LineB1, _LineB2, _LineA1) * CCW(_LineB1, _LineB2, _LineA2) <= 0)
	{
		// ** 예외사항 2
		// 단, 둘 다 0을 리턴. 즉 둘 다 하나의 직선상에 (일직선상에) 위치한 경우 예외 발생
		// 이 경우 포개어짐을 확인한다. 어차피 일직선상에 있으므로 x 하나만 잡고 검사함
		if (CCW(_LineA1, _LineA2, _LineB1) * CCW(_LineA1, _LineA2, _LineB2) == 0 &&
			CCW(_LineB1, _LineB2, _LineA1) * CCW(_LineB1, _LineB2, _LineA2) == 0)
		{
			// 예외사항 3
			// 단, X 축이 모두 같은 경우에 예외 발생.
			if (_LineA1.x == _LineA2.x)
			{
				float fY[4]{ _LineA1.y, _LineA2.y, _LineB1.y, _LineB2.y };

				if (fY[0] > fY[1]) { float fTmp = fY[0]; fY[0] = fY[1]; fY[1] = fTmp; }
				if (fY[2] > fY[3]) { float fTmp = fY[2]; fY[2] = fY[3]; fY[3] = fTmp; }

				// 겹치는 경우를 일직선상에서 보면 (A시작)---(B시작)--(A끝)--(B시작) 과 같은 꼴
				if (fY[2] <= fY[1] && fY[0] <= fY[3])
				{
					//Debug_CheckLine(_LineA1, _LineA2, _LineB1, _LineB2);
					return true;
				}
				else
					return false;
			}
			else
			{
				// 각각 점의 x를 오름차순으로 정렬한 뒤,
				// 겹치는지를 확인
				float fX[4]{ _LineA1.x, _LineA2.x, _LineB1.x, _LineB2.x };

				if (fX[0] > fX[1]) { float fTmp = fX[0]; fX[0] = fX[1]; fX[1] = fTmp; }
				if (fX[2] > fX[3]) { float fTmp = fX[2]; fX[2] = fX[3]; fX[3] = fTmp; }

				// 겹치는 경우를 일직선상에서 보면 (A시작)---(B시작)--(A끝)--(B시작) 과 같은 꼴
				if (fX[2] <= fX[1] && fX[0] <= fX[3])
				{
					//Debug_CheckLine(_LineA1, _LineA2, _LineB1, _LineB2);
					return true;
				}
			}
		}

		//Debug_CheckLine(_LineA1, _LineA2, _LineB1, _LineB2);
		return true;
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
	case 27:	return COL_POINT{	{X, Y + CY / 2}			, {X + CX / 2, Y},
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



bool CTileCollisionMgr::Collision_Line(float& _pY, float _fX)
{
	// 자연스러운 선타기 조작감을 위한, 판정을 널널하게 하는 보정 값.
	float fMargin = 40.f;



	// 검사할 타일들의 정보들을 가져옴
	std::vector<CObj*> pColliderVecTile = CTileMgr::Get_Instance()->Get_CollideVecTile();
	
	// 검사할 타일이 없다면 false 반환
	if (pColliderVecTile.size() == 0)
		return false;

	
	// 라인 정보를 담을 벡터 우선 생성
	std::vector<LINE_POINT> tTileTopVec;
	for (int i = 0; i < pColliderVecTile.size(); i++)
	{
		// 타일 정보에 기반하여 맨 위에 위치한 선분을 찾아, push_back함.
		// (COL_POINT의 4개 점 중 좌상 우상 점이 연결된 선분)
		CTile* tmpTile = dynamic_cast<CTile*>(pColliderVecTile[i]);
		COL_POINT tmpTileColPoint = CTileCollisionMgr::GetColPoint(*tmpTile->Get_Info(), tmpTile->Get_DrawID());
		
		LINE_POINT tmpLine = { tmpTileColPoint.ptLUp, tmpTileColPoint.ptRUp };
		tTileTopVec.push_back(tmpLine);
	}

	// 받아온 플레이어의 _pY, _fX 정보에 기반하여 해당되는 선분을 탐색
	// 1. 플레이어의 _fX 위치 범위 내에 있는 선분이어야 함
	// 2. 플레이어의 _pY 위치보다는 더 낮아야 함 (값이 더 높아야 함)
	// 3. 해당되는 선분이 여러개라면, 그중에선 가장 높아야 함 (값이 더 낮아야 함)
	// 이렇게 구한 선분의 정보를 이용, 플레이어의 최종 _pY 값을 계산 후 반영

	// 타겟 우선 선언
	LINE_POINT* pTarget = nullptr;

	// 조건에 맞는 타겟 할당..
	for (auto& pLine : tTileTopVec)
	{
		// 1번 조건 검사
		if (_fX >= pLine.ptStart.x && _fX < pLine.ptEnd.x)
		{
			HDC m_DC = GetDC(g_hWnd);
			HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
			HPEN hOldPen = (HPEN)SelectObject(m_DC, hPen);

			int LineB1X = pLine.ptStart.x-1, LineB1Y = pLine.ptStart.y-1;
			int LineB2X = pLine.ptEnd.x+1, LineB2Y = pLine.ptEnd.y+1;
			int LineA1X = pLine.ptStart.x-1, LineA1Y = pLine.ptStart.y-1;
			int LineA2X = pLine.ptEnd.x+1, LineA2Y = pLine.ptEnd.y+1;

			CCameraMgr::Get_Instance()->Get_RenderPos(LineB1X, LineB1Y);
			CCameraMgr::Get_Instance()->Get_RenderPos(LineB2X, LineB2Y);
			CCameraMgr::Get_Instance()->Get_RenderPos(LineA1X, LineA1Y);
			CCameraMgr::Get_Instance()->Get_RenderPos(LineA2X, LineA2Y);

			MoveToEx(m_DC, LineB1X, LineB1Y, nullptr);
			LineTo(m_DC, LineB2X, LineB2Y);
			MoveToEx(m_DC, LineA1X, LineA1Y, nullptr);
			LineTo(m_DC, LineA2X, LineA2Y);

			SelectObject(m_DC, hOldPen);
			DeleteObject(hPen);







			// 타겟이 비어있는 경우, 3번 조건 필요 X. 즉시 조건에 부합하는지 검사
			if (pTarget == nullptr)
			{
				float fNewX1 = pLine.ptStart.x;
				float fNewY1 = pLine.ptStart.y;

				float fNewX2 = pLine.ptEnd.x;
				float fNewY2 = pLine.ptEnd.y;

				float fNewLineY = ((fNewY2 - fNewY1) / (fNewX2 - fNewX1)) * (_fX - fNewX1) + fNewY1;
				
				// 2번 조건 검사
				// 플레이어가 해당 선보다 아래에 있되, 마진 이내의 차이면 할당
				if ((_pY - fMargin <= fNewLineY))
					pTarget = &pLine;
			}

			// 타겟이 이미 있는 경우, 3번 조건 검사 필요.
			else
			{	
				// fTargetLineY 의 계산
				float fTargetX1 = pTarget->ptStart.x;
				float fTargetY1 = pTarget->ptStart.y;

				float fTargetX2 = pTarget->ptEnd.x;
				float fTargetY2 = pTarget->ptEnd.y;

				float fTargetLineY = ((fTargetY2 - fTargetY1) / (fTargetX2 - fTargetX1)) * (_fX - fTargetX1) + fTargetY1;
			
				// fNewLineY 의 계산
				float fNewX1 = pLine.ptStart.x;
				float fNewY1 = pLine.ptStart.y;

				float fNewX2 = pLine.ptEnd.x;
				float fNewY2 = pLine.ptEnd.y;

				float fNewLineY = ((fNewY2 - fNewY1) / (fNewX2 - fNewX1)) * (_fX - fNewX1) + fNewY1;

				// 2, 3번 조건 동시검사
				// 플레이어가 해당 선보다 아래에 있되, 마진 이내의 차이면 할당
				// 단, 기존의 타겟 선 보다는 더 위에 있어야 함
				if ((_pY - fMargin <= fNewLineY && fTargetLineY >= fNewLineY))
					pTarget = &pLine;
			}
		}
	}

	// 타겟이 없다면 false 반환
	if (!pTarget)
		return false;

	// 계산 후 결과에 따라 _pY 값 변환
	float	x1 = pTarget->ptStart.x;
	float	y1 = pTarget->ptStart.y;
	float	x2 = pTarget->ptEnd.x;
	float	y2 = pTarget->ptEnd.y;

	float fResultY = ((y2 - y1) / (x2 - x1)) * (_fX - x1) + y1;
	_pY = fResultY;





	// 타겟 선을 찾았음 (즉, 플레이어가 지형에 닿았음) 을 알림
	return true;
}





void CTileCollisionMgr::Debug_CheckLine(POINT _LineA1, POINT _LineA2, POINT _LineB1, POINT _LineB2)
{
#pragma region Console Logging

	std::cout << "[INFO][CTileCollisionMgr::isCollideLine] Collision Detected" << std::endl;
	std::cout << "[Player :" << _LineB1.x << ", " << _LineB1.y << ", " << _LineB2.x << ", " << _LineB2.y << "]" << std::endl;
	std::cout << "[Tile :" << _LineA1.x << ", " << _LineA1.y << ", " << _LineA2.x << ", " << _LineA2.y << "]" << std::endl;

#pragma endregion

#pragma region Show to Screen (Pink Color)

	HDC m_DC = GetDC(g_hWnd);
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
	HPEN hOldPen = (HPEN)SelectObject(m_DC, hPen);

	int LineB1X = _LineB1.x, LineB1Y = _LineB1.y;
	int LineB2X = _LineB2.x, LineB2Y = _LineB2.y;
	int LineA1X = _LineA1.x, LineA1Y = _LineA1.y;
	int LineA2X = _LineA2.x, LineA2Y = _LineA2.y;

	CCameraMgr::Get_Instance()->Get_RenderPos(LineB1X, LineB1Y);
	CCameraMgr::Get_Instance()->Get_RenderPos(LineB2X, LineB2Y);
	CCameraMgr::Get_Instance()->Get_RenderPos(LineA1X, LineA1Y);
	CCameraMgr::Get_Instance()->Get_RenderPos(LineA2X, LineA2Y);

	MoveToEx(m_DC, LineB1X, LineB1Y, nullptr);
	LineTo(m_DC, LineB2X, LineB2Y);
	MoveToEx(m_DC, LineA1X, LineA1Y, nullptr);
	LineTo(m_DC, LineA2X, LineA2Y);

	SelectObject(m_DC, hOldPen);
	DeleteObject(hPen);

#pragma endregion

}



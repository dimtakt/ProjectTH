#include "pch.h"
#include "CTileCollisionMgr.h"
#include "CCollisionMgr.h"
#include "CCameraMgr.h"
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
				// if �浹�ϸ�
				// �÷��̾� ��ǥ�� �������� ������ ����..
				// �� �κ��� ���߿� ����.. ksta
				
			}

		}
	}
}

// Ÿ�� ������ ������Ʈ�� �浹�ϸ� ���� ��ȯ
// float ���ڴ� �󸶳� ��ġ������ ��ȯ�ϵ��� �����Ͽ� ��ģ ��ŭ�� ��ǥ�� ��������
bool CTileCollisionMgr::Check_Collision_Tile2Obj(CObj* _pTile, CObj* _pObj, float* _pX, float* _pY)
{
	CTile* tmpTile = dynamic_cast<CTile*>(_pTile);

	INFO tTileInfo = *tmpTile->Get_Info();
	INFO tObjInfo = *_pObj->Get_Info();

	unsigned int iTileID = tmpTile->Get_DrawID();	// �̰ɷ� �ݶ��̴��� ������� �Ǻ�
	if (!(1 <= iTileID && iTileID <= 30)) return false; // ���� ��� Ÿ���� �ݶ��̴��� �������� ���� ���, ��� false ������
	
	COL_POINT tTilePoint = GetColPoint(tTileInfo, iTileID);


	
	// �켱 AABB �浹 �˻� ����
	if (CCollisionMgr::Check_Rect(_pTile, _pObj, nullptr, nullptr)) {
		// AABB �浹 �˻翡�� �浹�� Ȯ�εǾ��ٸ�, �� �浹 �˻� ����
		// �� �浹 �˻� (CCW �˰����� �̿��Ͽ� �� Ÿ���� �����ϴ� �� ������ �浹 ���� ��ȯ)
		
		// ����� ���Ǽ��� ���� �迭�� ����
		// AABB �˻�� �迭
		POINT tTileRectPoints[4] = { { tTileInfo.fX - tTileInfo.fCX * 0.5f, tTileInfo.fY - tTileInfo.fCY * 0.5f },	// �»�
									{ tTileInfo.fX + tTileInfo.fCX * 0.5f, tTileInfo.fY - tTileInfo.fCY * 0.5f },	// ���
									{ tTileInfo.fX + tTileInfo.fCX * 0.5f, tTileInfo.fY + tTileInfo.fCY * 0.5f },	// ����
									{ tTileInfo.fX - tTileInfo.fCX * 0.5f, tTileInfo.fY + tTileInfo.fCY * 0.5f } };	// ����
		POINT tObjPoints[4] = { {tObjInfo.fX - tObjInfo.fCX * 0.5f, tObjInfo.fY - tObjInfo.fCY * 0.5f},		// �»�
								{tObjInfo.fX + tObjInfo.fCX * 0.5f, tObjInfo.fY - tObjInfo.fCY * 0.5f},		// ���
								{tObjInfo.fX + tObjInfo.fCX * 0.5f, tObjInfo.fY + tObjInfo.fCY * 0.5f},		// ����
								{tObjInfo.fX - tObjInfo.fCX * 0.5f, tObjInfo.fY + tObjInfo.fCY * 0.5f} };	// ����
		// �� �浹 �˻�� �迭
		POINT tTilePoints[4] = { tTilePoint.ptLUp, tTilePoint.ptRUp, tTilePoint.ptRDown, tTilePoint.ptLDown };	// �»�, ���, ����, ����

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
	// ��� �˻� ���Ŀ� �浹 Ȯ���� ���� �ʾҴٸ�, false ��ȯ
	return false;
}

// CCW �˰���
// A, B, C ������� ���� ��, �ݽð� �����̸� �����, �ð� �����̸� ������, ������ ���̸� 0�� ����
// ���� https://puu.sh/Ko2cX/04d85de5ed.png
int CTileCollisionMgr::CCW(POINT _ptA, POINT _ptB, POINT _ptC)
{
	return ((_ptB.x - _ptA.x) * (_ptC.y - _ptA.y) - (_ptB.y - _ptA.y) * (_ptC.x - _ptA.x));
}

// �� ���� ��ġ�� ���� ��ȯ
bool CTileCollisionMgr::isCollideLine(POINT _LineA1, POINT _LineA2, POINT _LineB1, POINT _LineB2)
{
	// ** ���ܻ��� 1
	// ���� �� ��� �� ���� �����ϴ� ��� (�� �� �ϳ��� 1, 2 ��ġ�� ����)
	// �浹 �˻翡 ���� �߻��ϹǷ�, �˻� X (��� false ����)
	if ((_LineA1.x == _LineA2.x && _LineA1.y == _LineA2.y) ||
		(_LineB1.x == _LineB2.x && _LineB1.y == _LineB2.y))
		return false;

	// �� ������ ������ ���� �������� ��� Ȯ������ ��,
	// �� ��� ���, ������ ��쿡 �ϳ��� �ð�, �ϳ��� �ݽð��� ��쿡 ��ģ��.
	if (CCW(_LineA1, _LineA2, _LineB1) * CCW(_LineA1, _LineA2, _LineB2) < 0 &&
		CCW(_LineB1, _LineB2, _LineA1) * CCW(_LineB1, _LineB2, _LineA2) < 0)
	{
		Debug_CheckLine(_LineA1, _LineA2, _LineB1, _LineB2);
		return true;
	}

	// ** ���ܻ��� 2
	// ��, �� �� 0�� ����. �� �� �� �ϳ��� ������ (��������) ��ġ�� ��� ���� �߻�
	// �� ��� ���������� Ȯ���Ѵ�. ������ �������� �����Ƿ� x �ϳ��� ��� �˻���
	if (CCW(_LineA1, _LineA2, _LineB1) * CCW(_LineA1, _LineA2, _LineB2) == 0 &&
		CCW(_LineB1, _LineA2, _LineA1) * CCW(_LineB1, _LineA2, _LineB2) == 0)
	{
		// ���� ���� x�� ������������ ������ ��,
		// ��ġ������ Ȯ��
		float fX[4]{ _LineA1.x, _LineA2.x, _LineB1.x, _LineB2.x };

		if (fX[0] > fX[1]) { float fTmp = fX[0]; fX[0] = fX[1]; fX[1] = fTmp; }
		if (fX[2] > fX[3]) { float fTmp = fX[2]; fX[2] = fX[3]; fX[3] = fTmp; }

		// ��ġ�� ��츦 �������󿡼� ���� (A����)---(B����)--(A��)--(B����) �� ���� ��
		if (fX[2] <= fX[1] && fX[0] <= fX[3])
		{
			Debug_CheckLine(_LineA1, _LineA2, _LineB1, _LineB2);
			return true;
		}
	}


	return false;
}

COL_POINT CTileCollisionMgr::GetColPoint(INFO _tileInfo, int _tileIndex)
{
	// �޾ƿ� ������ �����ӿ� ����
	// �� �÷��̾�� ���� ���ĵ� ������Ʈ�� ¤�� �ִ� ���ٴ��� ����

	LONG X	= _tileInfo.fX;
	LONG Y	= _tileInfo.fY;
	LONG CX = _tileInfo.fCX;
	LONG CY = _tileInfo.fCY;
	
	// COL_POINT �� ���´� {�»�, ���, ����, ����} ����
	switch (_tileIndex)
	{
	// 0 �� �� ��
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
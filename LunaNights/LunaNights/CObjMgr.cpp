#include "pch.h"
#include "CObjMgr.h"
#include "CCollisionMgr.h"
#include "CTileCollisionMgr.h"
#include "CTileMgr.h"

#include "CCameraMgr.h"
#include "CPlayerBullet.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
}

CObjMgr::~CObjMgr()
{
	Release();
}

CObj* CObjMgr::Get_Target(OBJ_ID eID, CObj* pObj)
{
	if (m_ObjList[eID].empty())
		return nullptr;

	CObj* pTarget = nullptr;
	float	fDistance = 0.f;

	for (auto& iter : m_ObjList[eID])
	{
		if (iter->Get_Dead())
			continue;

		float	fWidth = abs(pObj->Get_Info()->fX - iter->Get_Info()->fX);
		float	fHeight = abs(pObj->Get_Info()->fY - iter->Get_Info()->fY);

		float	fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

		if ((!pTarget) || fDistance > fDiagonal)
		{
			pTarget = iter;

			fDistance = fDiagonal;
		}
	}

	return pTarget;
}

void CObjMgr::Add_Object(OBJ_ID eID, CObj* pObj)
{
	if (OBJ_END <= eID || nullptr == pObj)
		return;

	m_ObjList[eID].push_back(pObj);
}

void CObjMgr::Update()
{
	Optimize_DeleteOutsideScreen();

	for (unsigned int i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end(); )
		{
			int iResult = (*iter)->Update();

			if (OBJ_DEAD == iResult)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}

}

void CObjMgr::Late_Update()
{
	for (unsigned int i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
		{
			pObj->Late_Update();

			if (m_ObjList[i].empty())
				break;

			RENDERID eRender = pObj->Get_RenderID();
			m_RenderList[eRender].push_back(pObj);
		}
	}

	// 충돌처리 활성화는 이곳에서 ksta
	
	std::vector<CObj*> vTiles = CTileMgr::Get_Instance()->Get_CollideVecTile();
	if (vTiles.size() != 0)
	{
		CTileCollisionMgr::Collision_Tile2Obj(vTiles, m_ObjList[OBJ_PLAYER]);
		CTileCollisionMgr::Collision_Tile2Obj(vTiles, m_ObjList[OBJ_MONSTER]);
	}

	//CCollisionMgr::Collision_Rect(m_ObjList[MOUSE], m_ObjList[MONSTER]);
	//CCollisionMgr::Collision_Circle(m_ObjList[BULLET], m_ObjList[MONSTER]);
	//CCollisionMgr::Collision_RectEx(m_ObjList[MONSTER], m_ObjList[PLAYER]);
}

void CObjMgr::Render(HDC hDC)
{
	for (unsigned int i = 0; i < RENDER_END; ++i)
	{
		m_RenderList[i].sort([](CObj* pDst, CObj* pSrc)->bool
			{
				return pDst->Get_Info()->fY < pSrc->Get_Info()->fY;
			});

		for (auto& pObj : m_RenderList[i])
			pObj->Render(hDC);

		m_RenderList[i].clear();
	}
}

void CObjMgr::Release()
{
	for (unsigned int i = 0; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<CObj*>);
		m_ObjList[i].clear();
	}
}

void CObjMgr::Delete_ID(OBJ_ID eID)
{
	for (auto& pObj : m_ObjList[eID])
		Safe_Delete(pObj);

	m_ObjList[eID].clear();
}

//void CObjMgr::Delete_SpecificObj(OBJ_ID _eID, CObj* _obj)
//{
//	if (_obj == nullptr)
//		return;
//
//	for (auto iter = m_ObjList[_eID].begin(); iter != m_ObjList[_eID].end(); ++iter)
//	{
//		if (*iter == _obj)
//		{
//			Safe_Delete<CObj*>(_obj);
//			m_ObjList[_eID].erase(iter);
//			return;
//		}
//	}
//
//}

void CObjMgr::Optimize_DeleteOutsideScreen()
{
	// 총알에 한해 화면 밖을 나갔는지 검사
	for (auto iter = m_ObjList[OBJ_PLAYERBULLET].begin(); iter != m_ObjList[OBJ_PLAYERBULLET].end();)
	{
		if (m_ObjList[OBJ_PLAYERBULLET].size() <= 0)
			return;

		const INFO* tObjPos = dynamic_cast<CPlayerBullet*>(*iter)->Get_Info();

		int iOutX = 0, iOutY = 0;
		CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY);

		// 화면 밖을 나갔다면
		if ((tObjPos->fX + tObjPos->fCX < 0 - iOutX || tObjPos->fX - iOutY - tObjPos->fCX > WINCX - iOutX) ||
			(tObjPos->fY + tObjPos->fCY < 0 - iOutY || tObjPos->fY - iOutY - tObjPos->fCY > WINCY - iOutY))
			// 삭제
		{
			Safe_Delete<CObj*>(*iter);
			m_ObjList[OBJ_PLAYERBULLET].erase(iter);
			return;
		}
		else
			++iter;
	}
}
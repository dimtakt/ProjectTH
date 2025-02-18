#include "pch.h"
#include "CObjMgr.h"
#include "CCollisionMgr.h"
#include "CTileCollisionMgr.h"
#include "CTileMgr.h"

#include "CCameraMgr.h"
#include "CPlayerBullet.h"
#include "CAbstractFactory.h"
#include "CWolf.h"
#include "CWisp.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr(): m_bPlayer(false)
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

	// �浹ó�� Ȱ��ȭ�� �̰����� ksta
	
	std::vector<CObj*> vTiles = CTileMgr::Get_Instance()->Get_CollideVecTile();
	if (vTiles.size() != 0)
	{
		CTileCollisionMgr::Collision_Tile2Obj(vTiles, m_ObjList[OBJ_PLAYER]);
		CTileCollisionMgr::Collision_Tile2Obj(vTiles, m_ObjList[OBJ_MONSTER_WOLF]);
	}

	//CCollisionMgr::Collision_Rect(m_ObjList[MOUSE], m_ObjList[MONSTER]);
	//CCollisionMgr::Collision_Circle(m_ObjList[BULLET], m_ObjList[MONSTER]);
	//CCollisionMgr::Collision_RectEx(m_ObjList[MONSTER], m_ObjList[PLAYER]);

	CCollisionMgr::Collision_Rect_PlayerMonster(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_MONSTER_WISP]);
	CCollisionMgr::Collision_Rect_PlayerMonster(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_MONSTER_WOLF]);

	CCollisionMgr::Collision_Rect_BulletMonster(m_ObjList[OBJ_PLAYERBULLET], m_ObjList[OBJ_MONSTER_WISP]);
	CCollisionMgr::Collision_Rect_BulletMonster(m_ObjList[OBJ_PLAYERBULLET], m_ObjList[OBJ_MONSTER_WOLF]);
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
	// �Ѿ˿� ���� ȭ�� ���� �������� �˻�
	for (auto iter = m_ObjList[OBJ_PLAYERBULLET].begin(); iter != m_ObjList[OBJ_PLAYERBULLET].end();)
	{
		if (m_ObjList[OBJ_PLAYERBULLET].size() <= 0)
			return;

		const INFO* tObjPos = dynamic_cast<CPlayerBullet*>(*iter)->Get_Info();

		int iOutX = 0, iOutY = 0;
		CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY);

		// ȭ�� ���� �����ٸ�
		if ((tObjPos->fX + tObjPos->fCX < 0 - iOutX || tObjPos->fX - iOutY - tObjPos->fCX > WINCX - iOutX) ||
			(tObjPos->fY + tObjPos->fCY < 0 - iOutY || tObjPos->fY - iOutY - tObjPos->fCY > WINCY - iOutY))
			// ����
		{
			Safe_Delete<CObj*>(*iter);
			m_ObjList[OBJ_PLAYERBULLET].erase(iter);
			return;
		}
		else
			++iter;
	}
}


void CObjMgr::Save_Data(const TCHAR* _dataFileName)
{
	if (_dataFileName)
	{
		HANDLE	hFile = CreateFile(_dataFileName, // ���� �̸��� ������ ���
			GENERIC_WRITE,		// ���� ���� ��� (���� : GENERIC_WRITE, �б� : GENERIC_READ)
			NULL,				// ���� ���, NULL�� ��� �������� ����
			NULL,				// ���� �Ӽ�, NULL�� ��� �⺻������ ����
			CREATE_ALWAYS,		// CREATE_ALWAYS : ������ ������ �����, ������ ���� ����, OPEN_EXISTING : ������ �ִ� ��쿡�� �ҷ����� ����
			FILE_ATTRIBUTE_NORMAL, // ���� �Ӽ� : �ƹ��� �Ӽ��� ���� �Ϲ����� ����
			NULL);				// ������ ������ �Ӽ��� ������ ���ø� ����, ������

		if (INVALID_HANDLE_VALUE == hFile)
		{
			MessageBox(g_hWnd, _T("Save File"), L"Fail", MB_OKCANCEL);
			return;
		}

		DWORD	dwByte(0);
		for (unsigned int i = 0; i < OBJ_END; ++i) {
			for (auto& pObj : m_ObjList[i])
			{
				WriteFile(hFile, pObj->Get_Info(), sizeof(INFO), &dwByte, nullptr);	// ��ġ���� ����
				WriteFile(hFile, &i, sizeof(unsigned int), &dwByte, nullptr);		// ���� ���� ����
			}
		}

		CloseHandle(hFile);
		MessageBox(g_hWnd, _T("Save �Ϸ�"), L"���� ����Ʈ ���� ����", MB_OK);
	}

}


void CObjMgr::Load_Data(const TCHAR* _dataFileName)
{
	// ���Ͱ� �߰��� �� ���� ���� ������ҵ�?
	Delete_ID(OBJ_MONSTER_WISP);
	Delete_ID(OBJ_MONSTER_WOLF);

	HANDLE	hFile = CreateFile(_dataFileName, // ���� �̸��� ������ ���
		GENERIC_READ,		// ���� ���� ��� (���� : GENERIC_WRITE, �б� : GENERIC_READ)
		NULL,				// ���� ���, NULL�� ��� �������� ����
		NULL,				// ���� �Ӽ�, NULL�� ��� �⺻������ ����
		OPEN_EXISTING,		// CREATE_ALWAYS : ������ ������ �����, ������ ���� ����, OPEN_EXISTING : ������ �ִ� ��쿡�� �ҷ����� ����
		FILE_ATTRIBUTE_NORMAL, // ���� �Ӽ� : �ƹ��� �Ӽ��� ���� �Ϲ����� ����
		NULL);				// ������ ������ �Ӽ��� ������ ���ø� ����, ������

	if (INVALID_HANDLE_VALUE == hFile)
	{
		//MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OKCANCEL);
		return;
	}

	DWORD	dwByte(0);

	//DATA	tData{};
	INFO tInfo{};
	unsigned int iTmp(OBJ_END);
	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		ReadFile(hFile, &iTmp, sizeof(unsigned int), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		switch (iTmp)
		{

		// �÷��̾� ������ �ε��ؿ��� ���� ����
		
		//case OBJ_PLAYER:
		//	if (!m_bPlayer)
		//	{
		//		Add_Object(static_cast<OBJ_ID>(iTmp), CAbstractFactory<CPlayer>::Create(tInfo.fX, tInfo.fY));
		//		m_bPlayer = true;
		//	}
		//
		//	break;


		// �ҷ��� OBJ_ID ���� ����ȯ�ؼ� �ش� ���� �´� ����Ʈ�� ����,
		// CAbstractFactory �� ���� ��ü�� ���� ��ġ�� ������
		case OBJ_MONSTER_WOLF:
			Add_Object(static_cast<OBJ_ID>(iTmp), CAbstractFactory<CWolf>::Create(tInfo.fX, tInfo.fY));
			break;

		case OBJ_MONSTER_WISP:
			Add_Object(static_cast<OBJ_ID>(iTmp), CAbstractFactory<CWisp>::Create(tInfo.fX, tInfo.fY));
			break;

		default:
			break;
		}
	}

	CloseHandle(hFile);
	//MessageBox(g_hWnd, _T("Load �Ϸ�"), L"����", MB_OK);
}

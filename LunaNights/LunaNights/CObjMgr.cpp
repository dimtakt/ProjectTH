#include "pch.h"
#include "CObjMgr.h"
#include "CCollisionMgr.h"
#include "CTileCollisionMgr.h"
#include "CTileMgr.h"
#include "CSceneMgr.h"

#include "CCameraMgr.h"
#include "CPlayerBullet.h"
#include "CAbstractFactory.h"
#include "CWolf.h"
#include "CWisp.h"
#include "CNpc.h"
#include "CPlayer.h"
#include "CCollideRect.h"

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
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pInstance->Get_Player());

		// 스테이지 내에서만.
		if (CSceneMgr::Get_Instance()->Get_CurScene() != CSceneMgr::SC_LOGO &&
			CSceneMgr::Get_Instance()->Get_CurScene() != CSceneMgr::SC_EDIT)
		{

			// 스네일모드 : 플레이어/이펙트/UI 제외 2배 느리게.
			if ((i != OBJ_PLAYER && i != OBJ_EFFECT && i != OBJ_UI) &&
				pPlayer->Get_Stat(CPlayer::TIMEMODE) == 1)
			{
				if (GetTickCount() % 2)
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

			// 시간정지모드 : 플레이어/이펙트/UI 제외 정지. 단 플레이어 칼날은 Rect만 업데이트 (안하면 렌더가 안됨)
			else if ((i != OBJ_PLAYER && i != OBJ_EFFECT && i != OBJ_UI && i != OBJ_NPC) &&
				pPlayer->Get_Stat(CPlayer::TIMEMODE) == 2)
			{
				if (i == OBJ_PLAYERBULLET)
				{
					for (auto iter = m_ObjList[i].begin();
						iter != m_ObjList[i].end(); )
					{
						(*iter)->Update_Rect();
						++iter;
					}
				}

			}

			// 일반모드
			else
			{
				// 원래대로.
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
		
		// 스테이지가 아닌경우.
		else
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

}

#pragma region old Update 0220-0356

//void CObjMgr::Update()
//{
//	Optimize_DeleteOutsideScreen();
//
//
//
//	for (unsigned int i = 0; i < OBJ_END; ++i)
//	{
//
//
//		for (auto iter = m_ObjList[i].begin();
//			iter != m_ObjList[i].end(); )
//		{
//			int iResult = (*iter)->Update();
//
//			if (OBJ_DEAD == iResult)
//			{
//				Safe_Delete<CObj*>(*iter);
//				iter = m_ObjList[i].erase(iter);
//			}
//			else
//				++iter;
//		}
//
//
//
//	}
//
//}

#pragma endregion



void CObjMgr::Late_Update()
{
	for (unsigned int i = 0; i < OBJ_END; ++i)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pInstance->Get_Player());

		// 스테이지 내에서만.
		if (CSceneMgr::Get_Instance()->Get_CurScene() != CSceneMgr::SC_LOGO &&
			CSceneMgr::Get_Instance()->Get_CurScene() != CSceneMgr::SC_EDIT)
		{

			// 스네일모드 : 플레이어/이펙트/UI 제외 2배 느리게.
			if ((i != OBJ_PLAYER && i != OBJ_EFFECT && i != OBJ_UI) &&
				pPlayer->Get_Stat(CPlayer::TIMEMODE) == 1)
			{
				if (GetTickCount() % 2)
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
				else
				{
					for (auto& pObj : m_ObjList[i])
					{
						//pObj->Late_Update();

						if (m_ObjList[i].empty())
							break;

						RENDERID eRender = pObj->Get_RenderID();
						m_RenderList[eRender].push_back(pObj);
					}
				}
			}
			// 시간정지모드 : 플레이어/이펙트/UI 제외 정지. 단 플레이어 칼날은 Rect만 업데이트 (안하면 렌더가 안됨)
			else if ((i != OBJ_PLAYER && i != OBJ_EFFECT && i != OBJ_UI && i != OBJ_NPC) &&
				pPlayer->Get_Stat(CPlayer::TIMEMODE) == 2)
			{
				// nothing.
				for (auto& pObj : m_ObjList[i])
				{
					//pObj->Late_Update();

					if (m_ObjList[i].empty())
						break;

					RENDERID eRender = pObj->Get_RenderID();
					m_RenderList[eRender].push_back(pObj);
				}
			}
			// 일반모드
			else
			{
				// 원래대로.
				for (auto& pObj : m_ObjList[i])
				{
					pObj->Late_Update();

					if (m_ObjList[i].empty())
						break;

					RENDERID eRender = pObj->Get_RenderID();
					m_RenderList[eRender].push_back(pObj);
				}
			}
		}
		else
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
	}

	// 충돌처리 활성화는 이곳에서 ksta
	
	std::vector<CObj*> vTiles = CTileMgr::Get_Instance()->Get_CollideVecTile();
	if (vTiles.size() != 0)
	{
		CTileCollisionMgr::Collision_Tile2Obj(vTiles, m_ObjList[OBJ_PLAYER]);
		CTileCollisionMgr::Collision_Tile2Obj(vTiles, m_ObjList[OBJ_MONSTER_WOLF]);
	}

	//CCollisionMgr::Collision_Rect(m_ObjList[MOUSE], m_ObjList[MONSTER]);
	//CCollisionMgr::Collision_Circle(m_ObjList[BULLET], m_ObjList[MONSTER]);
	//CCollisionMgr::Collision_RectEx(m_ObjList[MONSTER], m_ObjList[PLAYER]);


	// 플레이어와 몬스터의 충돌 확인, 충돌 시 플레이어만 데미지를 입음
	CCollisionMgr::Collision_Rect_PlayerMonster(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_MONSTER_WISP]);
	CCollisionMgr::Collision_Rect_PlayerMonster(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_MONSTER_WOLF]);

	CCollisionMgr::Collision_Rect_BulletPlayer(m_ObjList[OBJ_BOSSBULLET], m_ObjList[OBJ_PLAYER]);



	if (CSceneMgr::Get_Instance()->Get_CurScene() != CSceneMgr::SC_EDIT &&
		dynamic_cast<CPlayer*>(Get_Instance()->Get_Player())->Get_isBossStart())
		CCollisionMgr::Collision_Rect_PlayerMonster(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_BOSS]);


	// 플레이어 총알과 몬스터의 충돌 확인, 충돌 시 총알은 삭제 및 몬스터는 데미지를 입음
	if (CSceneMgr::Get_Instance()->Get_CurScene() != CSceneMgr::SC_LOGO &&
		CSceneMgr::Get_Instance()->Get_CurScene() != CSceneMgr::SC_EDIT &&
		dynamic_cast<CPlayer*>(Get_Instance()->Get_Player())->Get_Stat(CPlayer::TIMEMODE) != 2)
	{
		CCollisionMgr::Collision_Rect_BulletMonster(m_ObjList[OBJ_PLAYERBULLET], m_ObjList[OBJ_MONSTER_WISP]);
		CCollisionMgr::Collision_Rect_BulletMonster(m_ObjList[OBJ_PLAYERBULLET], m_ObjList[OBJ_MONSTER_WOLF]);

		if (CSceneMgr::Get_Instance()->Get_CurScene() != CSceneMgr::SC_EDIT &&
			dynamic_cast<CPlayer*>(Get_Instance()->Get_Player())->Get_isBossStart())
			CCollisionMgr::Collision_Rect_BulletMonster(m_ObjList[OBJ_PLAYERBULLET], m_ObjList[OBJ_BOSS]);
	}


	// 지상에 서 있어야 하는 객체와 충돌체의 충돌 확인, 충돌 시 해당 충돌체는 벽처럼 작용하여 객체의 이동을 막음 
	CCollisionMgr::Collision_RectEx(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_COLLIDERECT]);
	CCollisionMgr::Collision_RectEx(m_ObjList[OBJ_MONSTER_WOLF], m_ObjList[OBJ_COLLIDERECT]);
	CCollisionMgr::Collision_RectEx(m_ObjList[OBJ_BOSS], m_ObjList[OBJ_COLLIDERECT]);


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


void CObjMgr::Save_Data(const TCHAR* _dataFileName)
{
	if (_dataFileName)
	{
		HANDLE	hFile = CreateFile(_dataFileName, // 파일 이름을 포함한 경로
			GENERIC_WRITE,		// 파일 접근 모드 (쓰기 : GENERIC_WRITE, 읽기 : GENERIC_READ)
			NULL,				// 공유 방식, NULL인 경우 공유하지 않음
			NULL,				// 보안 속성, NULL인 경우 기본값으로 세팅
			CREATE_ALWAYS,		// CREATE_ALWAYS : 파일이 있으면 덮어쓰고, 없으면 파일 생성, OPEN_EXISTING : 파일이 있는 경우에만 불러오기 수행
			FILE_ATTRIBUTE_NORMAL, // 파일 속성 : 아무런 속성이 없는 일반적인 파일
			NULL);				// 생성될 파일의 속성을 제공할 템플릿 파일, 사용안함

		if (INVALID_HANDLE_VALUE == hFile)
		{
			MessageBox(g_hWnd, _T("Save File"), L"Fail", MB_OKCANCEL);
			return;
		}

		DWORD	dwByte(0);
		for (unsigned int i = 0; i < OBJ_END; ++i) {
			for (auto& pObj : m_ObjList[i])
			{
				WriteFile(hFile, pObj->Get_Info(), sizeof(INFO), &dwByte, nullptr);	// 위치정보 저장
				WriteFile(hFile, &i, sizeof(unsigned int), &dwByte, nullptr);		// 몬스터 유형 저장
			}
		}

		CloseHandle(hFile);
		MessageBox(g_hWnd, _T("Save 완료"), L"몬스터 리스트 저장 성공", MB_OK);
	}

}


void CObjMgr::Load_Data(const TCHAR* _dataFileName)
{
	// 몬스터가 추가될 때 마다 새로 해줘야할듯?
	Delete_ID(OBJ_MONSTER_WISP);
	Delete_ID(OBJ_MONSTER_WOLF);
	Delete_ID(OBJ_COLLIDERECT);
	Delete_ID(OBJ_NPC);
	Delete_ID(OBJ_BOSS);

	HANDLE	hFile = CreateFile(_dataFileName, // 파일 이름을 포함한 경로
		GENERIC_READ,		// 파일 접근 모드 (쓰기 : GENERIC_WRITE, 읽기 : GENERIC_READ)
		NULL,				// 공유 방식, NULL인 경우 공유하지 않음
		NULL,				// 보안 속성, NULL인 경우 기본값으로 세팅
		OPEN_EXISTING,		// CREATE_ALWAYS : 파일이 있으면 덮어쓰고, 없으면 파일 생성, OPEN_EXISTING : 파일이 있는 경우에만 불러오기 수행
		FILE_ATTRIBUTE_NORMAL, // 파일 속성 : 아무런 속성이 없는 일반적인 파일
		NULL);				// 생성될 파일의 속성을 제공할 템플릿 파일, 사용안함

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


		if (iTmp != OBJ_COLLIDERECT)
			std::cout << "[INFO][CObjMgr::LoadData] Obj Load.. : [Kind : " << iTmp << "] [Pos : " << tInfo.fX << ", " << tInfo.fY << "]" << std::endl;
		else
			std::cout << "[INFO][CObjMgr::LoadData] CollideRect Load.. : [Pos : " << tInfo.fX << ", " << tInfo.fY << "]" << std::endl;



		if (0 == dwByte)
			break;

		switch (iTmp)
		{

		// 플레이어 정보는 로드해오지 않을 것임
		
		//case OBJ_PLAYER:
		//	if (!m_bPlayer)
		//	{
		//		Add_Object(static_cast<OBJ_ID>(iTmp), CAbstractFactory<CPlayer>::Create(tInfo.fX, tInfo.fY));
		//		m_bPlayer = true;
		//	}
		//
		//	break;


		// 불러온 OBJ_ID 값을 형변환해서 해당 값에 맞는 리스트에 삽입,
		// CAbstractFactory 를 통해 객체를 지정 위치에 생성함
		case OBJ_MONSTER_WOLF:
			Add_Object(static_cast<OBJ_ID>(iTmp), CAbstractFactory<CWolf>::Create(tInfo.fX, tInfo.fY));
			break;

		case OBJ_MONSTER_WISP:
			Add_Object(static_cast<OBJ_ID>(iTmp), CAbstractFactory<CWisp>::Create(tInfo.fX, tInfo.fY));
			break;

		case OBJ_COLLIDERECT:
			Add_Object(static_cast<OBJ_ID>(iTmp), CAbstractFactory<CCollideRect>::CreateRectCollider(tInfo.fX - tInfo.fCX / 2, tInfo.fY - tInfo.fCY / 2, tInfo.fX + tInfo.fCX / 2, tInfo.fY + tInfo.fCY / 2));
			break;

		default:
			break;
		}
	}

	CloseHandle(hFile);
	//MessageBox(g_hWnd, _T("Load 완료"), L"성공", MB_OK);
}

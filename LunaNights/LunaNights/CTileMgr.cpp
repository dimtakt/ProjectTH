#include "pch.h"
#include "CTileMgr.h"
#include "CAbstractFactory.h"
#include "CCameraMgr.h"
#include "CSpritePropertyMgr.h"
#include "CTileCollisionMgr.h"

CTileMgr* CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
{
	// 미리 로드할 타일의 갯수를 정해, 메모리를 확보함
	m_vecTile.reserve(TILEX * TILEY);
	m_collideVecTile.reserve(TILEX * TILEY);
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	// 화면 크기에 맞게 타일을 생성함
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float fX = (float)((TILECX) * j) + ((TILECX) >> 1);
			float fY = (float)((TILECY) * i) + ((TILECY) >> 1);

			CObj* pTile = CAbstractFactory<CTile>::Create(fX, fY);
			m_vecTile.push_back(pTile);
			CObj* pCollideTile = CAbstractFactory<CTile>::Create(fX, fY);
			m_collideVecTile.push_back(pCollideTile);
			
			std::cout << "[INFO][CTileMgr::Initialize] Generated Tile Position : " << fX << ", " << fY << std::endl;
		}
	}

	m_tPropertyName = L"BG_Front";

	std::cout << "[INFO][CTileMgr::Initialize] Initialize Complete!" << std::endl;

}

void CTileMgr::Update()
{
	for (auto& pTile : m_vecTile)
		pTile->Update();
	for (auto& pTile : m_collideVecTile)
		pTile->Update();
}

void CTileMgr::Late_Update()
{
	for (auto& pTile : m_vecTile)
		pTile->Late_Update();
	for (auto& pTile : m_collideVecTile)
		pTile->Late_Update();
	// 타일과의 충돌처리는..?
}

void CTileMgr::Render(HDC hDC)
{
	float fOutX, fOutY;
	CCameraMgr::Get_Instance()->Get_CameraPos(fOutX, fOutY);

	// 렌더 스타트의 타일단위 좌표
	fOutX = (fOutX - (WINCX / 2)) / (TILECX);
	fOutY = (fOutY - (WINCY / 2)) / (TILECY);
	
	int iTmp = 0;

	int iMaxX = (fOutX + (WINCX / (TILECX))) + 2;
	int iMaxY = (fOutY + (WINCY / (TILECY))) + 2;

	for (int i = (int)fOutY; i < iMaxY; i++)
	{
		for (int j = (int)fOutX; j < iMaxX; j++)
		{
			if (i < 0 || i >= TILEY) continue;
			if (j < 0 || j >= TILEX) continue;

			int iIndex = i * TILEX + j;
			if ((size_t)iIndex >= m_vecTile.size())
				continue;

			dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_PropName(m_tPropertyName);
			dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_FrameProperty(CSpritePropertyMgr::Get_Instance()->Find_Property(m_tPropertyName));
			m_vecTile[iIndex]->Render(hDC);






			// **** 충돌 기준정보 확인
			if (m_collideVecTile.size() != 0)
			{
				int iOutX = 0, iOutY = 0;
				CCameraMgr::Get_Instance()->Get_RenderPos(iOutX, iOutY);
			
				CTile* pTile = dynamic_cast<CTile*>(m_collideVecTile[iIndex]);
			
				INFO tileInfo = *pTile->Get_Info();
				int tileIndex = pTile->Get_DrawID();
			
				COL_POINT tileColPoint = CTileCollisionMgr::GetColPoint(tileInfo, tileIndex);
			
				tileColPoint.ptLUp.x += iOutX;
				tileColPoint.ptLUp.y += iOutY;
				tileColPoint.ptRUp.x += iOutX;
				tileColPoint.ptRUp.y += iOutY;
				tileColPoint.ptRDown.x += iOutX;
				tileColPoint.ptRDown.y += iOutY;
				tileColPoint.ptLDown.x += iOutX;
				tileColPoint.ptLDown.y += iOutY;
			
				HPEN hGreenPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
				HPEN hOldPen = (HPEN)SelectObject(hDC, hGreenPen);
			
				MoveToEx(hDC, tileColPoint.ptLUp.x, tileColPoint.ptLUp.y, nullptr);
				LineTo(hDC, tileColPoint.ptRUp.x, tileColPoint.ptRUp.y);
				LineTo(hDC, tileColPoint.ptRDown.x, tileColPoint.ptRDown.y);
				LineTo(hDC, tileColPoint.ptLDown.x, tileColPoint.ptLDown.y);
				LineTo(hDC, tileColPoint.ptLUp.x, tileColPoint.ptLUp.y);
			
				SelectObject(hDC, hOldPen);
				DeleteObject(hGreenPen); 
			}






			iTmp++;
		}
	}

	//for (auto& pTile : m_vecTile)
	//	pTile->Render(hDC);

#pragma region Debug Log for Tile Range & Amount Check
	
	//std::cout << "[INFO][CTileMgr::Render] Start X, Y and End X, Y : (" << fOutX << ", " << iMaxX << "), (" << fOutY << ", " << iMaxY << ")" << std::endl;
	//std::cout << "[INFO][CTileMgr::Render] Tile Render Range (Tiled Size) : X (" << (int)fOutX << ", " << iMaxX << "), Y(" << (int)fOutY << ", " << iMaxY << ")" << std::endl;
	//std::cout << "[INFO][CTileMgr::Render] Tile Render Amount : " << iTmp << " / " << m_vecTile.size() << "(Tile Amount)" << std::endl;

#pragma endregion

}

void CTileMgr::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), DeleteObj());
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();

	for_each(m_collideVecTile.begin(), m_collideVecTile.end(), DeleteObj());
	m_collideVecTile.clear();
	m_collideVecTile.shrink_to_fit();
}

void CTileMgr::Picking_Tile(POINT ptMouse, int _iDrawID, int _iOption)
{
	int	x = ptMouse.x / (TILECX) ;
	int	y = ptMouse.y / (TILECY) ;

	int		iIndex = y * TILEX + x;

	if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
		return;

	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_DrawID(_iDrawID);
	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_Option(_iOption);

	std::cout << "[INFO][CTileMgr::Picking_Tile] Picked Pos : " << x << ", " << y << std::endl;
}

void CTileMgr::Save_Tile(const TCHAR* _dataFileName)
{
	HANDLE	hFile = CreateFile(_dataFileName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	int		iDrawID(0), iOption(0);
	DWORD	dwByte(0);

	for (auto& pTile : m_vecTile)
	{
		iDrawID = dynamic_cast<CTile*>(pTile)->Get_DrawID();
		iOption = dynamic_cast<CTile*>(pTile)->Get_Option();

		WriteFile(hFile, pTile->Get_Info(), sizeof(INFO), &dwByte, NULL);
		WriteFile(hFile, &iDrawID, sizeof(int), &dwByte, NULL);
		WriteFile(hFile, &iOption, sizeof(int), &dwByte, NULL);
	}

	CloseHandle(hFile);

	//MessageBox(g_hWnd, L"Tile Save", L"성공", MB_OK);
}

// 인자로 파일 위치와 해당 타일의 프로퍼티를 받아,
// 기존 로드된 타일 정보들을 삭제하고 다시 불러오는 함수
void CTileMgr::Load_Tile(const TCHAR* _dataFileName, const TCHAR* _propertyName, bool _isWithCollideTile,
						const TCHAR* _collideFileName, const TCHAR* _collidePropertyName)
{
	Release();

	// *************************************************
	// 데이터 하나만 로드 (Edit 모드 처럼)
	// *************************************************
	if (true)
	{
		HANDLE	hFile = CreateFile(_dataFileName,
			GENERIC_READ,
			NULL, NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		int		iDrawID(0), iOption(0);
		INFO	tInfo{};
		DWORD	dwByte(0);

		while (true)
		{
			ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, NULL);
			ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, NULL);
			ReadFile(hFile, &iOption, sizeof(int), &dwByte, NULL);

			if (dwByte == 0)
				break;

			CObj* pTile = CAbstractFactory<CTile>::Create(tInfo.fX, tInfo.fY);
			dynamic_cast<CTile*>(pTile)->Set_DrawID(iDrawID);
			dynamic_cast<CTile*>(pTile)->Set_Option(iOption);
			dynamic_cast<CTile*>(pTile)->Update_Rect();
			dynamic_cast<CTile*>(pTile)->Set_FrameProperty(CSpritePropertyMgr::Get_Instance()->Find_Property(_propertyName));

			m_vecTile.push_back(pTile);

		}
		
		CloseHandle(hFile);
	}
		
	// *************************************************
	// 기존 데이터에 콜라이더 데이터의 부름도 필요할 때
	// *************************************************
	if (_isWithCollideTile)
	{
		HANDLE	hCollideFile = CreateFile(_collideFileName,
			GENERIC_READ,
			NULL, NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (INVALID_HANDLE_VALUE == hCollideFile)
			return;

		int		iDrawID(0), iOption(0);
		INFO	tInfo{};
		DWORD	dwByte(0);

		while (true)
		{
			ReadFile(hCollideFile, &tInfo, sizeof(INFO), &dwByte, NULL);
			ReadFile(hCollideFile, &iDrawID, sizeof(int), &dwByte, NULL);
			ReadFile(hCollideFile, &iOption, sizeof(int), &dwByte, NULL);

			if (dwByte == 0)
				break;

			CObj* pTile = CAbstractFactory<CTile>::Create(tInfo.fX, tInfo.fY);
			dynamic_cast<CTile*>(pTile)->Set_DrawID(iDrawID);
			dynamic_cast<CTile*>(pTile)->Set_Option(iOption);
			dynamic_cast<CTile*>(pTile)->Update_Rect();
			dynamic_cast<CTile*>(pTile)->Set_FrameProperty(CSpritePropertyMgr::Get_Instance()->Find_Property(_collidePropertyName));

			m_collideVecTile.push_back(pTile);


			std::cout << m_vecTile.size() << std::endl;


		}

		CloseHandle(hCollideFile);

	}

	//MessageBox(g_hWnd, L"Tile Load", L"성공", MB_OK);
}
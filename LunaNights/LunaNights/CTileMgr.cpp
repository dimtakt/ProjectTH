#include "pch.h"
#include "CTileMgr.h"
#include "CAbstractFactory.h"
#include "CCameraMgr.h"

CTileMgr* CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
{
	// 미리 로드할 타일의 갯수를 정해, 메모리를 확보함
	m_vecTile.reserve(TILEX * TILEY);
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
			float fX = (float)((TILECX * TILESIZERATIO) * j) + ((TILECX * TILESIZERATIO) >> 1);
			float fY = (float)((TILECY * TILESIZERATIO) * i) + ((TILECY * TILESIZERATIO) >> 1);

			CObj* pTile = CAbstractFactory<CTile>::Create(fX, fY);
			m_vecTile.push_back(pTile);
			
			std::cout << "[INFO][CTileMgr::Initialize] Generateed Tile Position : " << fX << ", " << fY << std::endl;
		}
	}


	std::cout << "[INFO][CTileMgr::Initialize] Initialize Complete!" << std::endl;

}

void CTileMgr::Update()
{
	for (auto& pTile : m_vecTile)
		pTile->Update();

	//
	std::cout << "[INFO][CTileMgr::Update] m_vecTile[0] Rect : Left(" << m_vecTile[0]->Get_Rect()->left << "), Top(" << m_vecTile[0]->Get_Rect()->right << ")" << std::endl;
}

void CTileMgr::Late_Update()
{
	for (auto& pTile : m_vecTile)
		pTile->Late_Update();
	return;
}

void CTileMgr::Render(HDC hDC)
{
	float fOutX, fOutY;
	CCameraMgr::Get_Instance()->Get_CameraPos(fOutX, fOutY);

	fOutX /= (TILECX * TILESIZERATIO);		// 지금 화면이 몇 칸 만큼 더 갔는가 가 되어야 함
	fOutY /= (TILECY * TILESIZERATIO);		// 지금 화면이 몇 칸 만큼 더 갔는가 가 되어야 함

	int	iMaxX = fOutX + (WINCX / (TILECX * TILESIZERATIO)); //+ 2; // 화면의 크기에 맞춰서 생길 타일의 최대 칸수를 의미
	int	iMaxY = fOutY + (WINCY / (TILECY * TILESIZERATIO)); //+ 2; // 화면의 크기에 맞춰서 생길 타일의 최대 칸수를 의미


	// 이부분 면밀히 볼 것

	// 아마 스크롤의 경우에는 기본값이 0, 0인 반면에
	// 카메라의  경우에는 기본값이 WINCX / 2, WINCY / 2 라서 생기는 문제같음
	// 일단 잘래.. 
	for (int i = fOutY; i < iMaxY; ++i)
	{
		for (int j = fOutX; j < iMaxX; ++j)
		{
			int	iIndex = i * (TILEX) + j; // ksta

			if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
				continue;

			m_vecTile[iIndex]->Render(hDC);
		}
	}

	std::cout << "[INFO][CTileMgr::Render] Tile Vector Size : " << m_vecTile.size() << std::endl;
}

void CTileMgr::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), DeleteObj());
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void CTileMgr::Picking_Tile(POINT ptMouse, int _iDrawID, int _iOption)
{
	int	x = ptMouse.x / TILECX;
	int	y = ptMouse.y / TILECY;

	int		iIndex = y * TILEX + x;

	if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
		return;

	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_DrawID(_iDrawID);
	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_Option(_iOption);

}

void CTileMgr::Save_Tile()
{
	HANDLE	hFile = CreateFile(L"../Data/Tile.dat", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

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

	MessageBox(g_hWnd, L"Tile Save", L"성공", MB_OK);
}

void CTileMgr::Load_Tile()
{
	HANDLE	hFile = CreateFile(L"../Data/Tile.dat",
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

	Release();

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

		m_vecTile.push_back(pTile);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Tile Load", L"성공", MB_OK);
}

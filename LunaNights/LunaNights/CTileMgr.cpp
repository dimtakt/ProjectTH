#include "pch.h"
#include "CTileMgr.h"
#include "CAbstractFactory.h"
#include "CCameraMgr.h"

CTileMgr* CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
{
	// �̸� �ε��� Ÿ���� ������ ����, �޸𸮸� Ȯ����
	m_vecTile.reserve(TILEX * TILEY);
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	// ȭ�� ũ�⿡ �°� Ÿ���� ������
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

	fOutX /= (TILECX * TILESIZERATIO);		// ���� ȭ���� �� ĭ ��ŭ �� ���°� �� �Ǿ�� ��
	fOutY /= (TILECY * TILESIZERATIO);		// ���� ȭ���� �� ĭ ��ŭ �� ���°� �� �Ǿ�� ��

	int	iMaxX = fOutX + (WINCX / (TILECX * TILESIZERATIO)); //+ 2; // ȭ���� ũ�⿡ ���缭 ���� Ÿ���� �ִ� ĭ���� �ǹ�
	int	iMaxY = fOutY + (WINCY / (TILECY * TILESIZERATIO)); //+ 2; // ȭ���� ũ�⿡ ���缭 ���� Ÿ���� �ִ� ĭ���� �ǹ�


	// �̺κ� ����� �� ��

	// �Ƹ� ��ũ���� ��쿡�� �⺻���� 0, 0�� �ݸ鿡
	// ī�޶���  ��쿡�� �⺻���� WINCX / 2, WINCY / 2 �� ����� ��������
	// �ϴ� �߷�.. 
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

	MessageBox(g_hWnd, L"Tile Save", L"����", MB_OK);
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

	MessageBox(g_hWnd, L"Tile Load", L"����", MB_OK);
}

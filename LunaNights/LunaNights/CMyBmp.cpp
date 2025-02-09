#include "pch.h"
#include "CMyBmp.h"

CMyBmp::CMyBmp()
{
}

CMyBmp::~CMyBmp()
{
}

void CMyBmp::Load_Bmp(const TCHAR* pFilePath)
{
	HDC		hDC = GetDC(g_hWnd);

	m_hMemDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hWnd, hDC);

	m_hBitmap = (HBITMAP)LoadImage(NULL,		// 프로그램 인스턴스 핸들
									pFilePath,  // 파일의 경로
									IMAGE_BITMAP, // 어떤 타입으로 읽어올지 전달
									0, 0,		// 가로, 세로 크기
									LR_LOADFROMFILE | LR_CREATEDIBSECTION); // LR_LOADFROMFILE : 파일로부터 이미지 정보를 읽어들임
																			// LR_CREATEDIBSECTION : 읽어온 파일을 DIB 형태로 변환	
	// SelectObject : 준비한 DC의 GDI 오브젝트에 불러온 비트맵을 선택
	m_hOldbmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
}

void CMyBmp::Release()
{
	SelectObject(m_hMemDC, m_hOldbmp);
	DeleteObject(m_hBitmap);

	DeleteDC(m_hMemDC);

}

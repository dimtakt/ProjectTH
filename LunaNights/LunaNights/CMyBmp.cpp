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

	m_hBitmap = (HBITMAP)LoadImage(NULL,		// ���α׷� �ν��Ͻ� �ڵ�
									pFilePath,  // ������ ���
									IMAGE_BITMAP, // � Ÿ������ �о���� ����
									0, 0,		// ����, ���� ũ��
									LR_LOADFROMFILE | LR_CREATEDIBSECTION); // LR_LOADFROMFILE : ���Ϸκ��� �̹��� ������ �о����
																			// LR_CREATEDIBSECTION : �о�� ������ DIB ���·� ��ȯ	
	// SelectObject : �غ��� DC�� GDI ������Ʈ�� �ҷ��� ��Ʈ���� ����
	m_hOldbmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
}

void CMyBmp::Release()
{
	SelectObject(m_hMemDC, m_hOldbmp);
	DeleteObject(m_hBitmap);

	DeleteDC(m_hMemDC);

}

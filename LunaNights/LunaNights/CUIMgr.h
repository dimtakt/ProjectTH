//#pragma once
//class CUIMgr
//{
//private:
//	CUIMgr();
//	virtual ~CUIMgr();
//
//public:
//	void		Initialize();
//	void		Update();
//	void		Late_Update();
//	void		Render(HDC hDC);
//	void		Release();
//
//public:
//
//
//public:
//	// UI ������ ��� �̱����� ���� ���Ĵܿ� ���� ȣ���ϸ� �� ��?
//	// �� �� UI�� ���õ� ��ҵ��� ���⿡ ����
//	static CUIMgr* Get_Instance()
//	{
//		if (!m_pInstance)
//		{
//			m_pInstance = new CUIMgr;
//		}
//
//		return m_pInstance;
//	}
//	static void		Destroy_Instance()
//	{
//		if (m_pInstance)
//		{
//			delete m_pInstance;
//			m_pInstance = nullptr;
//		}
//	}
//
//private:
//	static CUIMgr*	m_pInstance;
//
//};
//

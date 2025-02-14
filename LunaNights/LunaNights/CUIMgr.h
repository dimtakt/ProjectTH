#pragma once
class CUIMgr
{
private:
	CUIMgr();
	virtual ~CUIMgr();

public:
	void		Initialize();
	void		Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();

public:


public:
	// UI 정보를 담는 싱글톤을 만들어서 최후단에 렌더 호출하면 될 듯?
	// 그 뒤 UI에 관련된 요소들은 여기에 구현




private:


};


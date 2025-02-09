#pragma once
#include "CObj.h"
class CTile : public CObj
{
public:
	CTile();
	virtual ~CTile();

public:
	const int& Get_DrawID() { return m_iDrawID; }
	const int& Get_Option() { return m_iOption; }

	void	Set_DrawID(int _iDrawID) { m_iDrawID = _iDrawID; }
	void	Set_Option(int _iOption) { m_iOption = _iOption; }

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	int			m_iDrawID;		// 0번 이미지를 출력할 것인가
	int			m_iOption;		// 옵션에 따라 타일 속성 변화
};


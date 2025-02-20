#pragma once
#include "CObj.h"
class CCollideRect :
    public CObj

// Rect 형태의 콜라이더 따로 구현을 위한 깡통 CObj 기반 객체
{
public:
    CCollideRect();
    virtual ~CCollideRect();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

};


#pragma once
#include "CObj.h"
class CCollideRect :
    public CObj

// Rect ������ �ݶ��̴� ���� ������ ���� ���� CObj ��� ��ü
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


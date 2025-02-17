#pragma once
#include "CObj.h"
class CWolf :
    public CObj
{
    CWolf();
    virtual ~CWolf();

    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};


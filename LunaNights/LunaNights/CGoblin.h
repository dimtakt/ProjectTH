#pragma once
#include "CObj.h"
class CGoblin :
    public CObj
{
    CGoblin();
    virtual ~CGoblin();

    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};


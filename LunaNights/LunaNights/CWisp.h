#pragma once
#include "CObj.h"
class CWisp :
    public CObj
{
    CWisp();
    virtual ~CWisp();


    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};


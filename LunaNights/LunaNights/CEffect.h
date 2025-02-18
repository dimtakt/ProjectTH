#pragma once
#include "CObj.h"
class CEffect :
    public CObj
{
public:
    CEffect();
    virtual ~CEffect();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

};


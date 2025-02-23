#pragma once
#include "CObj.h"


class CNpc :
    public CObj
{
public:
    CNpc();
    virtual ~CNpc();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:

};
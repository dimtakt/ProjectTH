#pragma once
#include "CObj.h"
class CWolf :
    public CObj
{
public:
    CWolf();
    virtual ~CWolf();

public:

    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    int m_iHp;
    int m_iMp;
    int m_fAtk;
};


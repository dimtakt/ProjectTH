#pragma once
#include "CObj.h"
class CWisp :
    public CObj
{
public:
    CWisp();
    virtual ~CWisp();

    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    int m_iHp;
    int m_iMp;
    float m_fAtk;

};


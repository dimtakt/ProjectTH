#pragma once
#include "CObj.h"

class CPlayer;

class CPlayerBullet :
    public CObj
{
public:
    CPlayerBullet();
    virtual ~CPlayerBullet();


public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;


private:
    CPlayer* pPlayer;
};


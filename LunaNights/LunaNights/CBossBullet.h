#pragma once
#include "CObj.h"

class CBoss_HonMeirin;

class CBossBullet :
    public CObj
{
public:
    CBossBullet();
    virtual ~CBossBullet();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    CBoss_HonMeirin* pBoss;

    FRAME tFrameBullet;
    FRAME tFrameBullet45;
    FRAME tFrameBulletBig;
};


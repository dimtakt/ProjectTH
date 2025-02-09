#pragma once
#include "CScene.h"
class CLogo :
    public CScene
{
public:
    CLogo();
    virtual ~CLogo();

public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _DC) override;
    void Release() override;

private:
};


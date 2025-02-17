#pragma once
#include "CScene.h"

class CObj;

class CEdit :
    public CScene
{
public:
    CEdit();
    virtual ~CEdit();

public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _DC) override;
    void Release() override;

private:
    void Key_Input();
    void LoadImages();

private:
    unsigned int iSelectedTileIndex;
    unsigned int iSelectedMonsterIndex; // 0 �� Wolf, 1 �� Wisp
    TCHAR tTileName;

    int iEditMode;  // 0 : Ÿ�� Ȯ�� ���, 1 : �ݶ��̴� ���� ���
    int iEditStage; // 0 : 1-1, 1 : 1-2, 2 : 1-3, 3 : 1-Boss

    int iTileX; // ������������ ���������� ����/���� ���̰� �ٸ�. �װ��� ����.
    int iTileY; // ������������ ���������� ����/���� ���̰� �ٸ�. �װ��� ����.

    std::list<CObj*> m_ObjList;
};


#include "pch.h"
#include "CCollisionMgr.h"
#include "CPlayer.h"
#include "CObjMgr.h"
#include "SoundMgr.h"

void CCollisionMgr::Collision_Rect_PlayerMonster(std::list<CObj*> DstList, std::list<CObj*> SrcList)
{
	RECT		rc{};

	for (auto& Dst : DstList)
	{
		for (auto& Src : SrcList)
		{
			INFO tDstInfo = *Dst->Get_CollideInfo();
			INFO tSrcInfo = *Src->Get_CollideInfo();

			RECT tDstRect = { tDstInfo.fX - tDstInfo.fCX / 2, tDstInfo.fY - tDstInfo.fCY / 2,
							tDstInfo.fX + tDstInfo.fCX / 2, tDstInfo.fY + tDstInfo.fCY / 2 };
			RECT tSrcRect = { tSrcInfo.fX - tSrcInfo.fCX / 2, tSrcInfo.fY - tSrcInfo.fCY / 2,
							tSrcInfo.fX + tSrcInfo.fCX / 2, tSrcInfo.fY + tSrcInfo.fCY / 2 };

			if (IntersectRect(&rc, &tDstRect, &tSrcRect))
			{
				//Dst->Set_Dead();
				//Src->Set_Dead();
				if (Dst->Get_State() != OBJST_DAMAGED && !Dst->Get_God())
				{
					Dst->Set_HP(Dst->Get_HP() - Src->Get_Atk());
					Dst->Set_State(OBJST_DAMAGED);
					Dst->Set_God();
					CSoundMgr::Get_Instance()->StopSound(SOUND_DESTROY);
					CSoundMgr::Get_Instance()->PlaySound(L"s15_destroy.wav", SOUND_DESTROY, 0.2f);
				}
				

				// 일단 죽지 않게
				if (Dst->Get_HP() < 0)
					Dst->Set_HP(0);
			}
		}
	}

}


void CCollisionMgr::Collision_Rect_BulletMonster(std::list<CObj*> DstList, std::list<CObj*> SrcList)
{
	RECT		rc{};

	for (auto& Dst : DstList)
	{
		for (auto& Src : SrcList)
		{
			INFO tDstInfo = *Dst->Get_CollideInfo();
			INFO tSrcInfo = *Src->Get_CollideInfo();

			RECT tDstRect = { tDstInfo.fX - tDstInfo.fCX / 2, tDstInfo.fY - tDstInfo.fCY / 2,
							tDstInfo.fX + tDstInfo.fCX / 2, tDstInfo.fY + tDstInfo.fCY / 2 };
			RECT tSrcRect = { tSrcInfo.fX - tSrcInfo.fCX / 2, tSrcInfo.fY - tSrcInfo.fCY / 2,
							tSrcInfo.fX + tSrcInfo.fCX / 2, tSrcInfo.fY + tSrcInfo.fCY / 2 };

			if (IntersectRect(&rc, &tDstRect, &tSrcRect))
			{
				Dst->Set_Dead();
				
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
				Src->Set_HP(Src->Get_HP() - pPlayer->Get_Atk());

				if (Src->Get_HP() <= 0)
				{
					Src->Set_Dead();
					CSoundMgr::Get_Instance()->StopSound(SOUND_DESTROY);
					CSoundMgr::Get_Instance()->PlaySound(L"s15_destroy.wav", SOUND_DESTROY, 0.2f);
				}
			}
		}
	}

}

void CCollisionMgr::Collision_RectEx(std::list<CObj*> DstList, std::list<CObj*> SrcList)
{
	float		fWidth(0.f), fHeight(0.f);

	for (auto& Dst : DstList)
	{
		for (auto& Src : SrcList)
		{
			if (Check_Rect(Dst, Src, &fWidth, &fHeight))
			{
				// 상, 하 충돌
				if (fWidth > fHeight)
				{
					// 상 충돌
					if (Dst->Get_Info()->fY < Src->Get_Info()->fY)
					{
						Dst->Set_PosY(-fHeight);
					}

					// 하 충돌
					else
					{
						Dst->Set_PosY(fHeight);
					}

				}
				// 좌 우 충돌
				else
				{
					// 좌 충돌
					if (Dst->Get_Info()->fX < Src->Get_Info()->fX)
					{
						Dst->Set_PosX(-fWidth);
					}

					// 우 충돌
					else
					{
						Dst->Set_PosX(fWidth);
					}
				}
			}
		}
	}
}

bool CCollisionMgr::Check_Rect(CObj* pDst, CObj* pSrc, float* pX, float* pY)
{
	float		fWidth = abs(pDst->Get_Info()->fX - pSrc->Get_Info()->fX);
	float		fHeight = abs(pDst->Get_Info()->fY - pSrc->Get_Info()->fY);

	float		fRadiusX = (pDst->Get_Info()->fCX + pSrc->Get_Info()->fCX) * 0.5f;
	float		fRadiusY = (pDst->Get_Info()->fCY + pSrc->Get_Info()->fCY) * 0.5f;

	if ((fRadiusX > fWidth) && (fRadiusY > fHeight))
	{
		if (pX && pY)
		{
			*pX = fRadiusX - fWidth;
			*pY = fRadiusY - fHeight;
		}
		return true;
	}
	
	return false;
}

void CCollisionMgr::Collision_Circle(std::list<CObj*> DstList, std::list<CObj*> SrcList)
{
	for (auto& Dst : DstList)
	{
		for (auto& Src : SrcList)
		{
			if (Intersect_Circle(Dst, Src))
			{
				Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}
}

bool CCollisionMgr::Intersect_Circle(CObj* pDst, CObj* pSrc)
{
	float	fRadius = (pDst->Get_Info()->fCX + pSrc->Get_Info()->fCX) * 0.5f;

	float	fWidth = abs(pDst->Get_Info()->fX - pSrc->Get_Info()->fX);
	float	fHeight = abs(pDst->Get_Info()->fY - pSrc->Get_Info()->fY);

	float	fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	return fRadius >= fDiagonal;
}

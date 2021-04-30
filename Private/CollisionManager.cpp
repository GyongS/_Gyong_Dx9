#include "stdafx.h"
#include "CollisionManager.h"
#include "GameObject.h"
#include "GameObject_Manager.h"
#include "Player.h"
#include "BrownWood.h"
#include "Warrior.h"
#include "GreenWood.h"
#include "GiantWood.h"
#include "Gold.h"
#include "PlayerSwitchIcon.h"
#include "SamuraiHead.h"
#include "PrisonerHead.h"
#include "Default_Head.h"
#include "Key_Manager.h"
#include "PlayerServeHPBar.h"
#include "Default_HitEffect.h"
#include "Prisoner_HitEffect.h"
#include "Samurai_HitEffect.h"
#include "Clegane.h"
#include "Cage.h"
#include "DamageFont.h"
#include "Hit_ILSEOM.h"
#include "Freeze_State.h"
#include "PlayerHit_UI.h"
#include "SoundManager.h"
#include "MonsterIcon.h"
#include "SkulHead.h"

CCollisionManager::CCollisionManager()
{
}


CCollisionManager::~CCollisionManager()
{
}

bool CCollisionManager::Check_Sphere(CGameObject* _Dst, CGameObject* _Src)
{
	float fX = abs(_Dst->Get_Info().vPos.x - _Src->Get_Info().vPos.x);
	float fY = _Dst->Get_Info().vPos.y - _Src->Get_Info().vPos.y;
	float fDia = sqrtf(fX * fX + fY * fY);

	float fDis = (float)((_Dst->Get_Info().iColCX + _Src->Get_Info().iColCX) * 0.5f);

	return fDia <= fDis;

	if (fDia <= fDis)
		return true;

	return false;
}

bool CCollisionManager::Check_ColRect(CGameObject * _Dst, CGameObject * _Src)
{
	float fDistX = abs(_Dst->Get_Info().vPos.x - _Src->Get_Info().vPos.x);
	float fDistY = abs(_Dst->Get_Info().vPos.y - _Src->Get_Info().vPos.y);

	float fCX = (float)((_Dst->Get_Info().iColCX + _Src->Get_Info().iColCX) >> 1);
	float fCY = (float)((_Dst->Get_Info().iColCY + _Src->Get_Info().iColCY) >> 1);

	if (fDistX < fCX && fDistY < fCY)
	{
		return true;
	}
	return false;
}

bool CCollisionManager::Check_ColRectEX(CGameObject * _Dst, CGameObject * _Src, float * _x, float * _y)
{
	float fDistX = abs(_Dst->Get_Info().vPos.x - _Src->Get_Info().vPos.x);
	float fDistY = abs(_Dst->Get_Info().vPos.y - _Src->Get_Info().vPos.y);

	float fCX = (float)((_Dst->Get_Info().iColCX + _Src->Get_Info().iColCX) >> 1);
	float fCY = (float)((_Dst->Get_Info().iColCY + _Src->Get_Info().iColCY) >> 1);

	if (fDistX < fCX && fDistY < fCY)
	{
		*_x = fCX - fDistX;
		*_y = fCY - fDistY;
		return true;
	}
	return false;
}

bool CCollisionManager::Check_TileRect(TILE_COLLISION* _Dst, CGameObject* _Src, float* _x, float* _y)
{
	float fDistX = abs(_Dst->vPos.x - _Src->Get_Info().vPos.x);
	float fDistY = abs(_Dst->vPos.y - _Src->Get_Info().vPos.y);

	float fCX = (float)((_Dst->vSize.x + _Src->Get_Info().vSize.x) * 0.5f);
	float fCY = (float)((_Dst->vSize.y + _Src->Get_Info().vSize.y) * 0.5f);

	if (fDistX < fCX && fDistY < fCY)
	{
		*_x = fCX - fDistX;
		*_y = fCY - fDistY;
		return true;
	}

	return false;
}

void CCollisionManager::Collision_Rect(list<CGameObject*>& _Dst, list<CGameObject*>& _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_ColRect(Dst, Src))
			{
				//////////////////////
				if (Src->Get_OBJID() == OBJ_ID::PORTAL)
				{
					static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_InPortal(true);
				}
				if (Src->Get_OBJID() == OBJ_ID::SHOP)
				{
					static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_InItemShop(true);
				}
				if (Src->Get_OBJID() == OBJ_ID::SKUL_HEAD && Dst->Get_OBJID() == OBJ_ID::PLAYER)
				{
					static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_PickUpHead(true);
					Src->Set_Dead();
				}
				else if (Src->Get_OBJID() == OBJ_ID::CAGE 
					&& static_cast<CMonsterIcon*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::MONSTER_ICON))->Get_MonsterCount() <= 0)
				{
					Dst->Set_Dead();
					static_cast<CCage*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::CAGE))->Set_Damage(1.f);
				}
				if (CKey_Manager::Get_Instance()->Key_Down(KEY_F))
				{
					if (Src->Get_OBJID() == OBJ_ID::DEFAULT_HEAD)
					{
						Src->Set_Dead();
					}
					else if (Src->Get_OBJID() == OBJ_ID::PRISONER_HEAD)
					{
						Src->Set_Dead();
					}
					else if (Src->Get_OBJID() == OBJ_ID::SAMURAI_HEAD)
					{
						Src->Set_Dead();
					}
				}
				else if (Src->Get_OBJID() == OBJ_ID::PLAYER && Dst->Get_OBJID() == OBJ_ID::MONSTER_ATTACK)
				{
					static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_Damage(5.f);
					CGameObject* pObj = CAbstractFactory<CPlayerHit_UI>::Create(400, 300);
					CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);
					Dst->Set_Dead();
				}

				else if (Src->Get_OBJID() == OBJ_ID::PLAYER && Dst->Get_OBJID() == OBJ_ID::GIANTWOOD_BALL)
				{
					static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_Damage(10.f);
					CGameObject* pObj = CAbstractFactory<CPlayerHit_UI>::Create(400, 300);
					CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);
					Dst->Set_Dead();
				}

				else if (Dst->Get_OBJID() == OBJ_ID::GIANTWOOD_ATTACK && Src->Get_OBJID() == OBJ_ID::PLAYER)
				{
					if (!static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_bHitCheck())
					{
						static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_Damage(15.f);
						CGameObject* pObj = CAbstractFactory<CPlayerHit_UI>::Create(400, 300);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);
						static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_HitCheck(true);
					}
				}

				else if (Src->Get_OBJID() == OBJ_ID::GOLD)
				{
					if (static_cast<CGold*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GOLD, Src))->Get_bPickUp())
						Src->Set_Dead();
				}

				else if (Dst->Get_OBJID() == OBJ_ID::GRENADE_COL && Src->Get_OBJID() == OBJ_ID::MONSTER)
				{
					int iDamage = (rand() % (int)static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Stat().fDamage) + 1;

					switch (Src->Get_OBJTYPE())
					{
					case OBJ_TYPE::BROWNWOOD:
						if (!static_cast<CBrownWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Get_HitGrenade())
						{
							static_cast<CBrownWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_Damage(iDamage + 20);

							CGameObject* pObj = CAbstractFactory<CDamageFont>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x,
								CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DAMAGEFONT, pObj);
							static_cast<CDamageFont*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::DAMAGEFONT))->Set_DamageFont(iDamage + 20);

							pObj = CAbstractFactory<CPrisoner_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x + rand() % 30,
								CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y + rand() % 30);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);

							static_cast<CBrownWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_HitGrenade(true);
						}
						break;
					case OBJ_TYPE::GREENWOOD:
						if (!static_cast<CGreenWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Get_HitGrenade())
						{
							static_cast<CGreenWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_Damage(iDamage + 20);

							CGameObject* pObj = CAbstractFactory<CDamageFont>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x,
								CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DAMAGEFONT, pObj);
							static_cast<CDamageFont*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::DAMAGEFONT))->Set_DamageFont(iDamage + 20);

							pObj = CAbstractFactory<CPrisoner_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x + rand() % 30,
								CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y + rand() % 30);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);

							static_cast<CGreenWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_HitGrenade(true);

						}
						break;
					case OBJ_TYPE::WARRIOR:
						if (!static_cast<CWarrior*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Get_HitGrenade())
						{
							static_cast<CWarrior*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_Damage(iDamage + 20);

							CGameObject* pObj = CAbstractFactory<CDamageFont>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x,
								CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DAMAGEFONT, pObj);
							static_cast<CDamageFont*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::DAMAGEFONT))->Set_DamageFont(iDamage + 20);

							pObj = CAbstractFactory<CPrisoner_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x + rand() % 30,
								CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y + rand() % 30);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);

							static_cast<CWarrior*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_HitGrenade(true);
						}
						break;
					default:
						break;
					}

				}

				else if (Dst->Get_OBJID() == OBJ_ID::GRENADE_COL && Src->Get_OBJID() == OBJ_ID::GIANTWOOD)
				{
					int iDamage = (rand() % (int)static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Stat().fDamage) + 1;


					if (!static_cast<CGiantWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src))->Get_HitGrenade())
					{
						static_cast<CGiantWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src))->Set_Damage(iDamage + 20);

						CGameObject* pObj = CAbstractFactory<CDamageFont>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.x,
							CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.y);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DAMAGEFONT, pObj);
						static_cast<CDamageFont*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::DAMAGEFONT, Src))->Set_DamageFont(iDamage + 20);

						pObj = CAbstractFactory<CPrisoner_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.x + rand() % 30,
							CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.y + rand() % 30);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);

						static_cast<CGiantWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src))->Set_HitGrenade(true);
					}
				}

				else if (Dst->Get_OBJID() == OBJ_ID::GRENADE_COL && Src->Get_OBJID() == OBJ_ID::BOSS)
				{
					int iDamage = (rand() % (int)static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Stat().fDamage) + 1;

					if (!static_cast<CClegane*>(CGameObject_Manager::Get_Instance()->Get_Boss())->Get_HitGrenade())
					{
						static_cast<CClegane*>(CGameObject_Manager::Get_Instance()->Get_Boss())->Set_Damage(iDamage + 23);

						CGameObject* pObj = CAbstractFactory<CDamageFont>::Create(CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.x,
							CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.y);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DAMAGEFONT, pObj);
						static_cast<CDamageFont*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::DAMAGEFONT))->Set_DamageFont(iDamage + 23);

						pObj = CAbstractFactory<CPrisoner_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.x + rand() % 30,
							CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.y + rand() % 30);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);

						static_cast<CClegane*>(CGameObject_Manager::Get_Instance()->Get_Boss())->Set_HitGrenade(true);
					}
				}

				else if (Dst->Get_OBJID() == OBJ_ID::BROADSWORD_COL && Src->Get_OBJID() == OBJ_ID::GIANTWOOD)
				{
					int iDamage = (rand() % (int)static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Stat().fDamage) + 1;

					if (!static_cast<CGiantWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src))->Get_HitBroadSword())
					{
						static_cast<CGiantWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src))->Set_Damage(iDamage + 20);

						CGameObject* pObj = CAbstractFactory<CDamageFont>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.x,
							CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.y);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DAMAGEFONT, pObj);
						static_cast<CDamageFont*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::DAMAGEFONT, Src))->Set_DamageFont(iDamage + 20);

						pObj = CAbstractFactory<CPrisoner_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.x + rand() % 30,
							CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.y + rand() % 30);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);
						static_cast<CGiantWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src))->Set_HitBroadSword(true);
					}
				}
				else if (Dst->Get_OBJID() == OBJ_ID::BROADSWORD_COL && Src->Get_OBJID() == OBJ_ID::MONSTER)
				{
					int iDamage = (rand() % (int)static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Stat().fDamage) + 1;

					switch (Src->Get_OBJTYPE())
					{
					case OBJ_TYPE::BROWNWOOD:
						if (!static_cast<CBrownWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Get_HitBroadSword())
						{
							static_cast<CBrownWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_Damage(iDamage + 15);

							CGameObject* pObj = CAbstractFactory<CDamageFont>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x,
								CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DAMAGEFONT, pObj);
							static_cast<CDamageFont*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::DAMAGEFONT))->Set_DamageFont(iDamage + 15);

							pObj = CAbstractFactory<CPrisoner_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x + rand() % 30,
								CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y + rand() % 30);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);
							static_cast<CBrownWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_HitBroadSword(true);
						}
						break;
					case OBJ_TYPE::GREENWOOD:
						if (!static_cast<CGreenWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Get_HitBroadSword())
						{
							static_cast<CGreenWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_Damage(iDamage + 15);

							CGameObject* pObj = CAbstractFactory<CDamageFont>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x,
								CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DAMAGEFONT, pObj);
							static_cast<CDamageFont*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::DAMAGEFONT))->Set_DamageFont(iDamage + 15);

							pObj = CAbstractFactory<CPrisoner_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x + rand() % 30,
								CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y + rand() % 30);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);
							static_cast<CGreenWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_HitBroadSword(true);

						}
						break;
					case OBJ_TYPE::WARRIOR:
						if (!static_cast<CWarrior*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Get_HitBroadSword())
						{
							static_cast<CWarrior*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_Damage(iDamage + 16);

							CGameObject* pObj = CAbstractFactory<CDamageFont>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x,
								CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DAMAGEFONT, pObj);
							static_cast<CDamageFont*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::DAMAGEFONT))->Set_DamageFont(iDamage + 16);

							pObj = CAbstractFactory<CPrisoner_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x + rand() % 30,
								CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y + rand() % 30);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);
							static_cast<CWarrior*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_HitBroadSword(true);
						}
						break;
					default:
						break;
					}
				}

				else if (Dst->Get_OBJID() == OBJ_ID::BROADSWORD_COL && Src->Get_OBJID() == OBJ_ID::BOSS)
				{
					if (!static_cast<CClegane*>(CGameObject_Manager::Get_Instance()->Get_Boss())->Get_HitBroadSword())
					{
						int iDamage = (rand() % (int)static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Stat().fDamage) + 1;

						static_cast<CClegane*>(CGameObject_Manager::Get_Instance()->Get_Boss())->Set_Damage(iDamage + 20);

						CGameObject* pObj = CAbstractFactory<CDamageFont>::Create(CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.x,
							CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.y);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DAMAGEFONT, pObj);
						static_cast<CDamageFont*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::DAMAGEFONT))->Set_DamageFont(iDamage + 20);

						pObj = CAbstractFactory<CPrisoner_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.x + rand() % 30,
							CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.y + rand() % 30);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);

						static_cast<CClegane*>(CGameObject_Manager::Get_Instance()->Get_Boss())->Set_HitBroadSword(true);
					}
				}

				if (Src->Get_OBJID() == OBJ_ID::MONSTER && Dst->Get_OBJID() == OBJ_ID::PLAYER_ATTACK)
				{
					CGameObject* pObj = nullptr;
					switch (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Type())
					{
					case CPlayer::TYPE_DEFAULT:
						pObj = CAbstractFactory<CDefault_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x,
							CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);
						CSoundManager::Get_Instance()->StopSound(CSoundManager::PLAYER_ATTACK_SFX);
						CSoundManager::Get_Instance()->PlaySound(L"Legacy_Hit [Unused].wav", CSoundManager::PLAYER_ATTACK_SFX);
						break;
					case CPlayer::TYPE_PRISONER:
						pObj = CAbstractFactory<CPrisoner_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x + rand() % 30,
							CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y + rand() % 30);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);
						CSoundManager::Get_Instance()->StopSound(CSoundManager::PLAYER_ATTACK_SFX);
						CSoundManager::Get_Instance()->PlaySound(L"Hit_Sword_Small.wav", CSoundManager::PLAYER_ATTACK_SFX);
						break;
					case CPlayer::TYPE_SAMURAI:
						pObj = CAbstractFactory<CSamurai_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x + rand() % 30,
							CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y + rand() % 30);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);
						CSoundManager::Get_Instance()->StopSound(CSoundManager::PLAYER_ATTACK_SFX);
						CSoundManager::Get_Instance()->PlaySound(L"Blast_Hit.wav", CSoundManager::PLAYER_ATTACK_SFX);
						break;
					}
					int iDamage = (rand() % (int)static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Stat().fDamage) + 1;

					switch (Src->Get_OBJTYPE())
					{
					case OBJ_TYPE::BROWNWOOD:
						static_cast<CBrownWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_Damage(iDamage);
						break;
					case OBJ_TYPE::GREENWOOD:
						static_cast<CGreenWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_Damage(iDamage);
						break;
					case OBJ_TYPE::GIANTWOOD:
						static_cast<CGiantWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_Damage(iDamage);
						break;
					case OBJ_TYPE::WARRIOR:
						static_cast<CWarrior*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_Damage(iDamage);
						break;
					default:
						break;
					}

					pObj = CAbstractFactory<CDamageFont>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x,
						CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y);
					CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DAMAGEFONT, pObj);
					static_cast<CDamageFont*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::DAMAGEFONT))->Set_DamageFont(iDamage);
					Dst->Set_Dead();
				}
				else if (Src->Get_OBJID() == OBJ_ID::GIANTWOOD && Dst->Get_OBJID() == OBJ_ID::PLAYER_ATTACK)
				{
					CGameObject* pObj = nullptr;
					switch (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Type())
					{
					case CPlayer::TYPE_DEFAULT:
						pObj = CAbstractFactory<CDefault_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.x,
							CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.y);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);
						CSoundManager::Get_Instance()->StopSound(CSoundManager::PLAYER_ATTACK_SFX);
						CSoundManager::Get_Instance()->PlaySound(L"Legacy_Hit [Unused].wav", CSoundManager::PLAYER_ATTACK_SFX);
						break;
					case CPlayer::TYPE_PRISONER:
						pObj = CAbstractFactory<CPrisoner_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.x + rand() % 30,
							CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.y + rand() % 30);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);
						CSoundManager::Get_Instance()->StopSound(CSoundManager::PLAYER_ATTACK_SFX);
						CSoundManager::Get_Instance()->PlaySound(L"Hit_Sword_Small.wav", CSoundManager::PLAYER_ATTACK_SFX);
						break;
					case CPlayer::TYPE_SAMURAI:
						pObj = CAbstractFactory<CSamurai_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.x + rand() % 30,
							CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.y + rand() % 30);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);
						CSoundManager::Get_Instance()->StopSound(CSoundManager::PLAYER_ATTACK_SFX);
						CSoundManager::Get_Instance()->PlaySound(L"Blast_Hit.wav", CSoundManager::PLAYER_ATTACK_SFX);
						break;
					}
					int iDamage = (rand() % (int)static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Stat().fDamage) + 1;

					static_cast<CGiantWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src))->Set_Damage(iDamage);

					pObj = CAbstractFactory<CDamageFont>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.x,
						CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.y);
					CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DAMAGEFONT, pObj);
					static_cast<CDamageFont*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::DAMAGEFONT))->Set_DamageFont(iDamage);
					Dst->Set_Dead();
				}
				else if (Src->Get_OBJID() == OBJ_ID::BOSS && Dst->Get_OBJID() == OBJ_ID::PLAYER_ATTACK)
				{
					CGameObject* pObj = nullptr;

					switch (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Type())
					{
					case CPlayer::TYPE_DEFAULT:
						pObj = CAbstractFactory<CDefault_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.x,
							CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.y);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);
						CSoundManager::Get_Instance()->StopSound(CSoundManager::PLAYER_ATTACK_SFX);
						CSoundManager::Get_Instance()->PlaySound(L"Legacy_Hit [Unused].wav", CSoundManager::PLAYER_ATTACK_SFX);
						break;
					case CPlayer::TYPE_PRISONER:
						pObj = CAbstractFactory<CPrisoner_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.x + rand() % 30,
							CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.y + rand() % 30);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);
						CSoundManager::Get_Instance()->StopSound(CSoundManager::PLAYER_ATTACK_SFX);
						CSoundManager::Get_Instance()->PlaySound(L"Hit_Sword_Small.wav", CSoundManager::PLAYER_ATTACK_SFX);
						break;
					case CPlayer::TYPE_SAMURAI:
						pObj = CAbstractFactory<CSamurai_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.x + rand() % 30,
							CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.y + rand() % 30);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);
						CSoundManager::Get_Instance()->StopSound(CSoundManager::PLAYER_ATTACK_SFX);
						CSoundManager::Get_Instance()->PlaySound(L"Blast_Hit.wav", CSoundManager::PLAYER_ATTACK_SFX);
						break;
					}

					int iDamage = (rand() % (int)static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Stat().fDamage) + 1;

					static_cast<CClegane*>(CGameObject_Manager::Get_Instance()->Get_Boss())->Set_Damage(iDamage);
					pObj = CAbstractFactory<CDamageFont>::Create(CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.x,
						CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.y);
					CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DAMAGEFONT, pObj);
					static_cast<CDamageFont*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::DAMAGEFONT))->Set_DamageFont(iDamage);
					Dst->Set_Dead();
				}

				if (Dst->Get_OBJID() == OBJ_ID::ILSEOM && Src->Get_OBJID() == OBJ_ID::MONSTER)
				{
					switch (Src->Get_OBJTYPE())
					{
					case OBJ_TYPE::BROWNWOOD:
						static_cast<CBrownWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_bColorBlack(true);
						break;
					case OBJ_TYPE::GREENWOOD:
						static_cast<CGreenWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_bColorBlack(true);
						break;
					case OBJ_TYPE::WARRIOR:
						static_cast<CWarrior*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_bColorBlack(true);
						break;
					default:
						break;
					}
					static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_HitILSeom(true);
					CGameObject* pObj = CAbstractFactory<CHit_ILSEOM>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x,
						CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y);
					CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);

				}

				else if (Dst->Get_OBJID() == OBJ_ID::ILSEOM && Src->Get_OBJID() == OBJ_ID::GIANTWOOD)
				{
					if (Src->Get_OBJTYPE() == OBJ_TYPE::GIANTWOOD)
						static_cast<CGiantWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src))->Set_bColorBlack(true);

					static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_HitILSeom(true);
					CGameObject* pObj = CAbstractFactory<CHit_ILSEOM>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.x,
						CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.y);
					CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);

				}

				if (Dst->Get_OBJID() == OBJ_ID::ILSEOM && Src->Get_OBJID() == OBJ_ID::BOSS &&
					static_cast<CClegane*>(CGameObject_Manager::Get_Instance()->Get_Boss())->Get_State() != CClegane::INTRO)
				{
					static_cast<CClegane*>(CGameObject_Manager::Get_Instance()->Get_Boss())->Set_bColorBlack(true);

					static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_HitILSeom(true);
					CGameObject* pObj = CAbstractFactory<CHit_ILSEOM>::Create(CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.x,
						CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.y);
					CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);

				}
				if (Dst->Get_OBJID() == OBJ_ID::ILSEOM_HIT && Src->Get_OBJID() == OBJ_ID::MONSTER)
				{
					int iDamage = (rand() % (int)static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Stat().fDamage) + 1;

					switch (Src->Get_OBJTYPE())
					{
					case OBJ_TYPE::BROWNWOOD:
						static_cast<CBrownWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_Damage(iDamage + 100);
						break;
					case OBJ_TYPE::GREENWOOD:
						static_cast<CGreenWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_Damage(iDamage + 100);
						break;
					case OBJ_TYPE::WARRIOR:
						static_cast<CWarrior*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_Damage(iDamage + 100);
						break;
					default:
						break;
					}
					CGameObject* pObj = CAbstractFactory<CDamageFont>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x,
						CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y);
					CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DAMAGEFONT, pObj);
					static_cast<CDamageFont*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::DAMAGEFONT))->Set_DamageFont(iDamage + 100);
					Dst->Set_Dead();
				}

				if (Dst->Get_OBJID() == OBJ_ID::ILSEOM_HIT && Src->Get_OBJID() == OBJ_ID::GIANTWOOD)
				{
					int iDamage = (rand() % (int)static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Stat().fDamage) + 1;

					static_cast<CGiantWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src))->Set_Damage(iDamage + 100);

					CGameObject* pObj = CAbstractFactory<CDamageFont>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.x,
						CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.y);
					CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DAMAGEFONT, pObj);
					static_cast<CDamageFont*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::DAMAGEFONT))->Set_DamageFont(iDamage + 100);
					Dst->Set_Dead();
				}

				if (Dst->Get_OBJID() == OBJ_ID::ILSEOM_HIT && Src->Get_OBJID() == OBJ_ID::BOSS &&
					static_cast<CClegane*>(CGameObject_Manager::Get_Instance()->Get_Boss())->Get_State() != CClegane::INTRO)
				{
					int iDamage = (rand() % (int)static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Stat().fDamage) + 1;

					static_cast<CClegane*>(CGameObject_Manager::Get_Instance()->Get_Boss())->Set_Damage(iDamage + 100);

					CGameObject* pObj = CAbstractFactory<CDamageFont>::Create(CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.x,
						CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.y);
					CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DAMAGEFONT, pObj);
					static_cast<CDamageFont*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::DAMAGEFONT))->Set_DamageFont(iDamage + 100);
					Dst->Set_Dead();

				}

				else if (Dst->Get_OBJID() == OBJ_ID::ICE_ALLOW && Src->Get_OBJID() == OBJ_ID::MONSTER ||
					Dst->Get_OBJID() == OBJ_ID::ICE_ALLOW && Src->Get_OBJID() == OBJ_ID::GIANTWOOD ||
					Dst->Get_OBJID() == OBJ_ID::ICE_ALLOW && Src->Get_OBJID() == OBJ_ID::BOSS)
				{
					int iDamage = (rand() % (int)static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Stat().fDamage) + 1;

					if (Dst->Get_OBJID() == OBJ_ID::ICE_ALLOW && Src->Get_OBJID() == OBJ_ID::BOSS &&
						static_cast<CClegane*>(CGameObject_Manager::Get_Instance()->Get_Boss())->Get_State() != CClegane::INTRO)
					{
						if (!static_cast<CClegane*>(CGameObject_Manager::Get_Instance()->Get_Boss())->Get_HitIceAllow())
						{
							static_cast<CClegane*>(CGameObject_Manager::Get_Instance()->Get_Boss())->Set_Damage(iDamage + 20);

							CGameObject* pObj = CAbstractFactory<CDamageFont>::Create(CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.x,
								CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.y);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DAMAGEFONT, pObj);
							static_cast<CDamageFont*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::DAMAGEFONT))->Set_DamageFont(iDamage + 20);

							CSoundManager::Get_Instance()->StopSound(CSoundManager::ICE_ARROW);
							CSoundManager::Get_Instance()->PlaySound(L"Atk_Ice_Short.wav", CSoundManager::ICE_ARROW);

							pObj = CAbstractFactory<CPrisoner_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.x + rand() % 30,
								CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Info().vPos.y + rand() % 30);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);

							static_cast<CClegane*>(CGameObject_Manager::Get_Instance()->Get_Boss())->Set_HitIceAllow(true);
						}
					}
					switch (Src->Get_OBJTYPE())
					{
					case OBJ_TYPE::BROWNWOOD:
						if (!static_cast<CBrownWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Get_HitIceAllow())
						{
							static_cast<CBrownWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_Damage(iDamage + 20);

							CGameObject* pObj = CAbstractFactory<CDamageFont>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x,
								CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DAMAGEFONT, pObj);
							static_cast<CDamageFont*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::DAMAGEFONT))->Set_DamageFont(iDamage + 20);

							CSoundManager::Get_Instance()->StopSound(CSoundManager::ICE_ARROW);
							CSoundManager::Get_Instance()->PlaySound(L"Atk_Ice_Short.wav", CSoundManager::ICE_ARROW);

							pObj = CAbstractFactory<CPrisoner_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x + rand() % 30,
								CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y + rand() % 30);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);

							static_cast<CBrownWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_HitIceAllow(true);
						}
						break;
					case OBJ_TYPE::GREENWOOD:
						if (!static_cast<CGreenWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Get_HitIceAllow())
						{
							static_cast<CGreenWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_Damage(iDamage + 20);

							CGameObject* pObj = CAbstractFactory<CDamageFont>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x,
								CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DAMAGEFONT, pObj);
							static_cast<CDamageFont*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::DAMAGEFONT))->Set_DamageFont(iDamage + 20);

							CSoundManager::Get_Instance()->StopSound(CSoundManager::ICE_ARROW);
							CSoundManager::Get_Instance()->PlaySound(L"Atk_Ice_Short.wav", CSoundManager::ICE_ARROW);

							pObj = CAbstractFactory<CPrisoner_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x + rand() % 30,
								CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y + rand() % 30);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);

							static_cast<CGreenWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_HitIceAllow(true);
						}
						break;
					case OBJ_TYPE::GIANTWOOD:
						if (!static_cast<CGiantWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src))->Get_HitIceAllow())
						{
							static_cast<CGiantWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src))->Set_Damage(iDamage + 20);

							CGameObject* pObj = CAbstractFactory<CDamageFont>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.x,
								CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.y);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DAMAGEFONT, pObj);
							static_cast<CDamageFont*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::DAMAGEFONT))->Set_DamageFont(iDamage + 20);

							CSoundManager::Get_Instance()->StopSound(CSoundManager::ICE_ARROW);
							CSoundManager::Get_Instance()->PlaySound(L"Atk_Ice_Short.wav", CSoundManager::ICE_ARROW);

							pObj = CAbstractFactory<CPrisoner_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.x + rand() % 30,
								CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src)->Get_Info().vPos.y + rand() % 30);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);

							static_cast<CGiantWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GIANTWOOD, Src))->Set_HitIceAllow(true);
						}
						break;
					case OBJ_TYPE::WARRIOR:
						if (!static_cast<CWarrior*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Get_HitIceAllow())
						{
							static_cast<CWarrior*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_Damage(iDamage + 20);

							CGameObject* pObj = CAbstractFactory<CDamageFont>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x,
								CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DAMAGEFONT, pObj);
							static_cast<CDamageFont*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::DAMAGEFONT))->Set_DamageFont(iDamage + 20);

							CSoundManager::Get_Instance()->StopSound(CSoundManager::ICE_ARROW);
							CSoundManager::Get_Instance()->PlaySound(L"Atk_Ice_Short.wav", CSoundManager::ICE_ARROW);

							pObj = CAbstractFactory<CPrisoner_HitEffect>::Create(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.x + rand() % 30,
								CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src)->Get_Info().vPos.y + rand() % 30);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);

							static_cast<CWarrior*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_HitIceAllow(true);
						}
						break;
					default:
						break;
					}
				}
				if (Dst->Get_OBJID() == OBJ_ID::BOSS_EARTHQUAKE && Src->Get_OBJID() == OBJ_ID::PLAYER)
				{
					if (!static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_bHitCheck())
					{
						static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_Damage(20.f);
						CGameObject* pObj = CAbstractFactory<CPlayerHit_UI>::Create(400, 300);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);
						static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_HitCheck(true);
					}
				}
				if (Dst->Get_OBJID() == OBJ_ID::BOSS && Src->Get_OBJID() == OBJ_ID::PLAYER)
				{
					if (static_cast<CClegane*>(CGameObject_Manager::Get_Instance()->Get_Boss())->Get_State() == CClegane::WHIRLWIND_LOOP
						&& !static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_bHitCheck())
					{
						static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_Damage(25.f);
						CGameObject* pObj = CAbstractFactory<CPlayerHit_UI>::Create(400, 300);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);
						static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_HitCheck(true);
					}
				}
				if (Dst->Get_OBJID() == OBJ_ID::STAMPING_WAVE && Src->Get_OBJID() == OBJ_ID::PLAYER)
				{
					if (!static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_bHitCheck())
					{
						static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_Damage(15.f);
						CGameObject* pObj = CAbstractFactory<CPlayerHit_UI>::Create(400, 300);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);
						static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_HitCheck(true);
					}
				}
				if (Dst->Get_OBJID() == OBJ_ID::STONE && Src->Get_OBJID() == OBJ_ID::PLAYER)
				{
					if (!static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_bHitCheck())
					{
						static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_Damage(15.f);
						CGameObject* pObj = CAbstractFactory<CPlayerHit_UI>::Create(400, 300);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);
						static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_HitCheck(true);
					}
				}
				//////////////////////
			}

			if (!Check_ColRect(Dst, Src))
			{
				if (Src->Get_OBJID() == OBJ_ID::PORTAL)
				{
					static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_InPortal(false);
				}
			}
			if (!Check_ColRect(Dst, Src))
			{
				if (Src->Get_OBJID() == OBJ_ID::SHOP)
				{
					static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_InItemShop(false);
				}
			}
		}
	}
}

void CCollisionManager::Collision_RectEx(list<CGameObject*>& _Dst, list<CGameObject*>& _Src)
{
	float fX = 0.f, fY = 0.f;
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_ColRectEX(Dst, Src, &fX, &fY))
			{
				if (fX < fY)
				{
					if (Dst->Get_Info().vPos.x < Src->Get_Info().vPos.x)
						Src->Set_PosX(fX);
					else
						Src->Set_PosX(-fX);
				}
				else
				{

					Src->Set_PosY(-fY);
				}
			}

		}
	}
}

void CCollisionManager::Collision_TIle(list<CGameObject*>& _Dst, list<CGameObject*>& _Src)
{
	float fX = 0.f, fY = 0.f;
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_ColRectEX(Dst, Src, &fX, &fY))
			{
				if (fX < fY && Dst->Get_OBJID() == OBJ_ID::TILE && Src->Get_OBJID() == OBJ_ID::PLAYER)
				{
					if (Dst->Get_Info().vPos.x < Src->Get_Info().vPos.x)
						Src->Set_PosX(fX);
					else
						Src->Set_PosX(-fX);
				}
				else
				{
					if (Src->Get_OBJID() == OBJ_ID::PLAYER)
					{
						static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_GroundCheck(true);
						static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_JumpCheck(false);
						static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_DoubleJumpCheck(false);
						static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_JumpTime(0.f);
						static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_JumpCount(0);
					}
					if (Src->Get_OBJID() == OBJ_ID::MONSTER)
					{
						static_cast<CBrownWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_CheckGround(true);
					}
					if (Src->Get_OBJID() == OBJ_ID::GOLD)
					{
						static_cast<CGold*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GOLD, Src ))->Set_GroundCheck(false);
						static_cast<CGold*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::GOLD, Src ))->Set_JumpTime();
					}
					if (Src->Get_OBJID() == OBJ_ID::SKUL_HEAD)
					{
						static_cast<CSkulHead*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::SKUL_HEAD))->Set_GroundCheck(false);
						static_cast<CSkulHead*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::SKUL_HEAD))->Set_JumpTime();
					}
					if (Src->Get_OBJID() == OBJ_ID::GRENADE)
					{
						Src->Set_Dead();
					}
					if (Src->Get_OBJID() == OBJ_ID::STONE)
					{
						Src->Set_Dead();
					}
					if (Src->Get_OBJID() == OBJ_ID::DEFAULT_HEAD)
					{
						static_cast<CDefault_Head*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::DEFAULT_HEAD, Src))->Set_JumpCheck(false);
						static_cast<CDefault_Head*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::DEFAULT_HEAD, Src))->Set_JumpTime();
					}
					if (Src->Get_OBJID() == OBJ_ID::SAMURAI_HEAD)
					{
						static_cast<CSamuraiHead*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::SAMURAI_HEAD, Src))->Set_JumpCheck(false);
						static_cast<CSamuraiHead*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::SAMURAI_HEAD, Src))->Set_JumpTime();
					}
					if (Src->Get_OBJID() == OBJ_ID::PRISONER_HEAD)
					{
						static_cast<CPrisonerHead*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::PRISONER_HEAD, Src))->Set_JumpCheck(false);
						static_cast<CPrisonerHead*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::PRISONER_HEAD, Src))->Set_JumpTime();
					}
					Src->Set_PosY(-fY);
				}
			}

			if (!Check_ColRect(Dst, Src))
			{
				if (Src->Get_OBJID() == OBJ_ID::MONSTER)
				{
					static_cast<CBrownWood*>(CGameObject_Manager::Get_Instance()->Get_NearTarget(OBJ_ID::MONSTER, Src))->Set_CheckGround(false);
				}
			}
			
			/*	if (Src->Get_OBJID() == OBJ_ID::PLAYER && Dst->Get_OBJID() == OBJ_ID::TILE || Src->Get_OBJID() == OBJ_ID::PLAYER && Dst->Get_OBJID() == OBJ_ID::TILE_2)
				{
				   ¼öÁ¤ÇØ¾ß´ï
					if (Check_ColRect(Dst, Src))
					{
						static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_GroundCheck(true);
					}
					else if (!Check_ColRect(Dst, Src))
					{
						static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_GroundCheck(false);
					}
				}*/
		}
	}
}
#include "stdafx.h"
#include "GameObject_Manager.h"
#include "CollisionManager.h"
#include "Terrain_Client.h"

IMPLEMENT_SINGLETON(CGameObject_Manager)

CGameObject_Manager::CGameObject_Manager()
{
}


CGameObject_Manager::~CGameObject_Manager()
{
	Release_GameObject();
}

CGameObject* CGameObject_Manager::Get_NearTarget(OBJ_ID::ID _eID, CGameObject* _pObj)
{
	CGameObject* pTarget = nullptr;
	float	fDis = 0.f;

	for (auto& pObj : m_listObject[_eID])
	{
		float fX = pObj->Get_Info().vPos.x - _pObj->Get_Info().vPos.x;
		float fY = pObj->Get_Info().vPos.y - _pObj->Get_Info().vPos.y;
		float fDia = sqrtf(fX * fX + fY * fY);

		if (fDis > fDia || !pTarget)
		{
			fDis = fDia;
			pTarget = pObj;
		}
	}
	return pTarget;
}

CGameObject * CGameObject_Manager::Get_Target(OBJ_ID::ID _eID)
{
	CGameObject* pTarget = nullptr;
	if (m_listObject[_eID].empty())
		return nullptr;

	for (auto& pObj : m_listObject[_eID])
		pTarget = pObj;

	return pTarget;
}

void CGameObject_Manager::Add_GameObject(OBJ_ID::ID eID, CGameObject* pGameObject)
{
	m_listObject[eID].emplace_back(pGameObject);
}

void CGameObject_Manager::DeleteID(OBJ_ID::ID _eID)
{
	if (m_listObject[_eID].empty())
		return;

	for_each(m_listObject[_eID].begin(), m_listObject[_eID].end(), Safe_Delete<CGameObject*>);
	m_listObject[_eID].clear();		
}

void CGameObject_Manager::Update_GameObject()
{
	for (int i = 0; i < OBJ_ID::END; ++i)
	{
		for (auto& iter = m_listObject[i].begin(); iter != m_listObject[i].end(); )
		{
			int iEvent = (*iter)->Update_GameObject();

			if (1 == iEvent)
			{
				Safe_Delete(*iter);
				iter = m_listObject[i].erase(iter);
			}
			else
				++iter;
		}
	}

	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::PLAYER], m_listObject[OBJ_ID::PORTAL]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::PLAYER], m_listObject[OBJ_ID::SHOP]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::PLAYER], m_listObject[OBJ_ID::GOLD]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::PLAYER], m_listObject[OBJ_ID::DEFAULT_HEAD]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::PLAYER], m_listObject[OBJ_ID::PRISONER_HEAD]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::PLAYER], m_listObject[OBJ_ID::SAMURAI_HEAD]);

	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::PLAYER_ATTACK], m_listObject[OBJ_ID::CAGE]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::PLAYER_ATTACK], m_listObject[OBJ_ID::MONSTER]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::PLAYER_ATTACK], m_listObject[OBJ_ID::GIANTWOOD]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::PLAYER_ATTACK], m_listObject[OBJ_ID::BOSS]);

	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::ICE_ALLOW], m_listObject[OBJ_ID::MONSTER]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::ICE_ALLOW], m_listObject[OBJ_ID::GIANTWOOD]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::ICE_ALLOW], m_listObject[OBJ_ID::BOSS]);
	
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::BROADSWORD_COL], m_listObject[OBJ_ID::MONSTER]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::BROADSWORD_COL], m_listObject[OBJ_ID::GIANTWOOD]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::BROADSWORD_COL], m_listObject[OBJ_ID::BOSS]);	

	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::GRENADE_COL], m_listObject[OBJ_ID::MONSTER]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::GRENADE_COL], m_listObject[OBJ_ID::GIANTWOOD]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::GRENADE_COL], m_listObject[OBJ_ID::BOSS]);

	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::BOSS_EARTHQUAKE], m_listObject[OBJ_ID::PLAYER]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::BOSS], m_listObject[OBJ_ID::PLAYER]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::STAMPING_WAVE], m_listObject[OBJ_ID::PLAYER]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::STONE], m_listObject[OBJ_ID::PLAYER]);


	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::ILSEOM], m_listObject[OBJ_ID::MONSTER]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::ILSEOM_HIT], m_listObject[OBJ_ID::MONSTER]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::ILSEOM], m_listObject[OBJ_ID::GIANTWOOD]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::ILSEOM_HIT], m_listObject[OBJ_ID::GIANTWOOD]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::ILSEOM], m_listObject[OBJ_ID::BOSS]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::ILSEOM_HIT], m_listObject[OBJ_ID::BOSS]);

	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::MONSTER_ATTACK], m_listObject[OBJ_ID::PLAYER]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::GIANTWOOD_BALL], m_listObject[OBJ_ID::PLAYER]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::GIANTWOOD_ATTACK], m_listObject[OBJ_ID::PLAYER]);
	CCollisionManager::Collision_Rect(m_listObject[OBJ_ID::PLAYER], m_listObject[OBJ_ID::SKUL_HEAD]);


	// 타일 충돌

	CCollisionManager::Collision_TIle(m_listObject[OBJ_ID::TILE], m_listObject[OBJ_ID::PLAYER]);
	CCollisionManager::Collision_TIle(m_listObject[OBJ_ID::TILE], m_listObject[OBJ_ID::MONSTER]);
	CCollisionManager::Collision_TIle(m_listObject[OBJ_ID::TILE], m_listObject[OBJ_ID::GOLD]);
	CCollisionManager::Collision_TIle(m_listObject[OBJ_ID::TILE], m_listObject[OBJ_ID::GRENADE]);
	CCollisionManager::Collision_TIle(m_listObject[OBJ_ID::TILE], m_listObject[OBJ_ID::DEFAULT_HEAD]);
	CCollisionManager::Collision_TIle(m_listObject[OBJ_ID::TILE], m_listObject[OBJ_ID::PRISONER_HEAD]);
	CCollisionManager::Collision_TIle(m_listObject[OBJ_ID::TILE], m_listObject[OBJ_ID::SAMURAI_HEAD]);
	CCollisionManager::Collision_TIle(m_listObject[OBJ_ID::TILE], m_listObject[OBJ_ID::STONE]);
	CCollisionManager::Collision_TIle(m_listObject[OBJ_ID::TILE], m_listObject[OBJ_ID::SKUL_HEAD]);

	CCollisionManager::Collision_TIle(m_listObject[OBJ_ID::TILE_2], m_listObject[OBJ_ID::PLAYER]);
	CCollisionManager::Collision_TIle(m_listObject[OBJ_ID::TILE_2], m_listObject[OBJ_ID::MONSTER]);
	CCollisionManager::Collision_TIle(m_listObject[OBJ_ID::TILE_2], m_listObject[OBJ_ID::GRENADE]);
	CCollisionManager::Collision_TIle(m_listObject[OBJ_ID::TILE_2], m_listObject[OBJ_ID::GOLD]);
	CCollisionManager::Collision_TIle(m_listObject[OBJ_ID::TILE_2], m_listObject[OBJ_ID::SKUL_HEAD]);



}

void CGameObject_Manager::Late_Update_GameObject()
{
	for (int i = 0; i < OBJ_ID::END; ++i)
	{
		for (auto& pObject : m_listObject[i])
		{
			pObject->Late_Update_GameObject();
		}
	}
}

void CGameObject_Manager::Render_GameObject()
{
	for (int i = 0; i < OBJ_ID::END; ++i)
	{
		for (auto& pObject : m_listObject[i])
		{
			pObject->Render_GameObject();
		}
	}
}

void CGameObject_Manager::Release_GameObject()
{
	for (int i = 0; i < OBJ_ID::END; ++i)
	{
		for (auto& pObject : m_listObject[i])
		{
			Safe_Delete(pObject);
		}
		m_listObject[i].clear();
	}
}

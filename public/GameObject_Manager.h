#pragma once
#include "GameObject.h"
class CGameObject_Manager
{
	DECLARE_SINGLETON(CGameObject_Manager)
private:
	CGameObject_Manager();
	~CGameObject_Manager();

public:
	CGameObject* Get_Player() const { return m_listObject[OBJ_ID::PLAYER].front();}
	CGameObject* Get_Boss() const { return m_listObject[OBJ_ID::BOSS].front(); }
	CGameObject* Get_NearTarget(OBJ_ID::ID _eID, CGameObject* _pObj);
	CGameObject* Get_Target(OBJ_ID::ID _eID);

public:
	void Add_GameObject(OBJ_ID::ID eID, CGameObject* pGameObject);
	void DeleteID(OBJ_ID::ID _eID);
public:
	void Update_GameObject();
	void Late_Update_GameObject();
	void Render_GameObject();
	void Release_GameObject();

private:
	list<CGameObject*> m_listObject[OBJ_ID::END];
	vector<TILE_COLLISION*> m_vecTile_Col;
};


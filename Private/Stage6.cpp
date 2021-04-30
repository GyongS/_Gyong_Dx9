#include "stdafx.h"
#include "Stage6.h"
#include "GameObject_Manager.h"
#include "Terrain_Client.h"
#include "Player.h"
#include "SoundManager.h"
#include "Scene_Manager.h"
#include "Key_Manager.h"
#include "Portal.h"
#include "GiantWood.h"
#include "BrownWood.h"
#include "Player.h"
#include "MinimapPortal.h"
#include "SoundManager.h"

CStage6::CStage6()
{
}


CStage6::~CStage6()
{
	Release_Scene();
}

HRESULT CStage6::Ready_Scene()
{	
	CSoundManager::Get_Instance()->PlayBGM(L"Chapter1.wav");

	static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_GroundCheck(false);

	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::TERRAIN, CTerrain_Client::Create(L"../Data/Stage6.dat"));

	CGameObject_Manager::Get_Instance()->Get_Player()->Set_Pos(150.f, 100.f);

	CGameObject* pObj = CAbstractFactory<CPortal>::Create(1772, 474);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::PORTAL, pObj);

	pObj = CAbstractFactory<CMinimapPortal>::Create(0, 0);
	pObj->Set_Target(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::PORTAL));
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MINIMAP_PORTAL, pObj);

	pObj = CAbstractFactory<CGiantWood>::Create(873, 330);
	pObj->Set_Target(CGameObject_Manager::Get_Instance()->Get_Player());
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::GIANTWOOD, pObj);

	float fX = 0.f;
	for (size_t i = 0; i < 3; i++)
	{
		pObj = CAbstractFactory<CBrownWood>::Create(1016 + fX, 200);
		pObj->Set_Target(CGameObject_Manager::Get_Instance()->Get_Player());
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MONSTER, pObj);

		fX += 200.f;
	}

	fX = 0.f;
	for (size_t i = 0; i < 5; i++)
	{
		pObj = CAbstractFactory<CBrownWood>::Create(760 + fX, 518);
		pObj->Set_Target(CGameObject_Manager::Get_Instance()->Get_Player());
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MONSTER, pObj);

		fX += 200.f;
	}

	return S_OK;
}

int CStage6::Update_Scene()
{
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_Q))
	{
		CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_STAGE7);
	}
	CGameObject_Manager::Get_Instance()->Update_GameObject();
	return OBJ_NOEVENT;
}

void CStage6::Late_Update_Scene()
{
	CGameObject_Manager::Get_Instance()->Late_Update_GameObject();
}

void CStage6::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject();
}

void CStage6::Release_Scene()
{
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::TILE);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::TILE_2);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::TERRAIN);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::GIANTWOOD);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::GIANTWOOD_ATTACK);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::GIANTWOOD_BALL);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::MONSTER);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::MONSTER_UI);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::PORTAL);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::MINIMAP_MONSTER);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::MINIMAP_PORTAL);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::GOLD);
	CSoundManager::Get_Instance()->StopBGM();

}

CStage6 * CStage6::Create()
{
	CStage6* pInstance = new CStage6;

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		ERR_MSG(L"CreateFailed Stage6");
		return nullptr;
	}
	return pInstance;
}

#include "stdafx.h"
#include "Stage2.h"
#include "GameObject_Manager.h"
#include "Terrain_Client.h"
#include "Key_Manager.h"
#include "Scene_Manager.h"
#include "Cage.h"
#include "Cage_Human.h"
#include "EventManager.h"
#include "Portal.h"
#include "Player.h"
#include "MinimapPortal.h"
#include "SoundManager.h"
#include "Warrior.h"
#include "MonsterIcon.h"

CStage2::CStage2()
{
}


CStage2::~CStage2()
{
	Release_Scene();
}

HRESULT CStage2::Ready_Scene()
{
	static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_GroundCheck(false);

	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::TERRAIN, CTerrain_Client::Create(L"../Data/Stage2.dat"));

	CGameObject_Manager::Get_Instance()->Get_Player()->Set_Pos(150.f, 100.f);

	CGameObject* pObj = CAbstractFactory<CCage>::Create(546, 435);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::CAGE, pObj);

	

	pObj = CAbstractFactory<CCage_Human>::Create(546, 455);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::TUTORIAL, pObj);

	pObj = CAbstractFactory<CWarrior>::Create(746, 455);
	pObj->Set_Target(CGameObject_Manager::Get_Instance()->Get_Player());
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MONSTER, pObj);

	pObj = CAbstractFactory<CWarrior>::Create(646, 455);
	pObj->Set_Target(CGameObject_Manager::Get_Instance()->Get_Player());
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MONSTER, pObj);

	pObj = CAbstractFactory<CPortal>::Create(1772, 444);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::PORTAL, pObj);

	pObj = CAbstractFactory<CMinimapPortal>::Create(0, 0);
	pObj->Set_Target(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::PORTAL));
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MINIMAP_PORTAL, pObj);

	return S_OK;
}

int CStage2::Update_Scene()
{
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_Q))
	{
		CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_STAGE3);
	}
	Stage_Event();
	CGameObject_Manager::Get_Instance()->Update_GameObject();
	return OBJ_NOEVENT;
}

void CStage2::Late_Update_Scene()
{
	CGameObject_Manager::Get_Instance()->Late_Update_GameObject();
}

void CStage2::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject();	
}

void CStage2::Release_Scene()
{
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::TILE);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::TILE_2);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::TERRAIN);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::CAGE);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::TUTORIAL);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::MONSTER);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::PORTAL);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::MINIMAP_PORTAL);
	CSoundManager::Get_Instance()->StopBGM();
}

CStage2 * CStage2::Create()
{
	CStage2* pInstance = new CStage2;
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		ERR_MSG(L"CreateFailed Stage");
		return nullptr;
	}
	return pInstance;
}

void CStage2::Stage_Event()
{
	if (CScene_Manager::Get_Instance()->Get_Scene() == CScene_Manager::SCENE_STAGE2)
	{
		if (static_cast<CCage*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::CAGE))->Get_DestroyCheck()
			&& CEventManager::Get_Instance()->Get_EventCnt() < 3)
		{
			CEventManager::Get_Instance()->Set_EventCnt();
			CEventManager::Get_Instance()->Set_StartEvent(true);
		}
	}
}

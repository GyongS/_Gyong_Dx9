#include "stdafx.h"
#include "Stage4.h"
#include "GameObject_Manager.h"
#include "Terrain_Client.h"
#include "Key_Manager.h"
#include "Scene_Manager.h"
#include "PrisonerHead.h"
#include "Portal.h"
#include "Player.h"
#include "MinimapPortal.h"

CStage4::CStage4()
{
}


CStage4::~CStage4()
{
	Release_Scene();
}

HRESULT CStage4::Ready_Scene()
{
	static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_GroundCheck(false);

	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::TERRAIN, CTerrain_Client::Create(L"../Data/Stage4.dat"));

	CGameObject_Manager::Get_Instance()->Get_Player()->Set_Pos(150.f, 100.f);

	CGameObject* pObj = CAbstractFactory<CPrisonerHead>::Create(906, 517);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::PRISONER_HEAD, pObj);

	pObj = CAbstractFactory<CPortal>::Create(1687, 280);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::PORTAL, pObj);

	pObj = CAbstractFactory<CMinimapPortal>::Create(0, 0);
	pObj->Set_Target(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::PORTAL));
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MINIMAP_PORTAL, pObj);
	return S_OK;
}

int CStage4::Update_Scene()
{
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_Q))
	{
		CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_STAGE5);
	}
	CGameObject_Manager::Get_Instance()->Update_GameObject();
	return OBJ_NOEVENT;
}

void CStage4::Late_Update_Scene()
{
	CGameObject_Manager::Get_Instance()->Late_Update_GameObject();

}

void CStage4::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject();
}

void CStage4::Release_Scene()
{
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::TILE);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::TILE_2);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::TERRAIN);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::SAMURAI_HEAD);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::DEFAULT_HEAD);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::PRISONER_HEAD);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::PORTAL);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::MINIMAP_PORTAL);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::MINIMAP_MONSTER);
}

CStage4 * CStage4::Create()
{
	CStage4* pInstance = new CStage4;
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		ERR_MSG(L"CreateFailed Stage");
		return nullptr;
	}
	return pInstance;
}

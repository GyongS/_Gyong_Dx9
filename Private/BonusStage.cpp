#include "stdafx.h"
#include "BonusStage.h"
#include "GameObject_Manager.h"
#include "Terrain_Client.h"
#include "Player.h"
#include "Portal.h"
#include "MinimapPortal.h"
#include "Scene_Manager.h"
#include "Key_Manager.h"
#include "Item_Shop.h"
#include "Portal.h"
#include "SoundManager.h"

CBonusStage::CBonusStage()
{
}


CBonusStage::~CBonusStage()
{
	Release_Scene();
}

HRESULT CBonusStage::Ready_Scene()
{
	CSoundManager::Get_Instance()->PlayBGM(L"DemonCastle.wav");

	static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_GroundCheck(false);

	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::TERRAIN, CTerrain_Client::Create(L"../Data/BonusStage.dat"));

	CGameObject_Manager::Get_Instance()->Get_Player()->Set_Pos(150.f, 100.f);

	CGameObject* pObj = CAbstractFactory<CItem_Shop>::Create(513.f, 361.f);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::SHOP, pObj);

	pObj = CAbstractFactory<CPortal>::Create(898.f, 341.f);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::PORTAL, pObj);
	return S_OK;
}

int CBonusStage::Update_Scene()
{
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_Q))
	{
		CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_STAGE6);
	}

	CGameObject_Manager::Get_Instance()->Update_GameObject();
	return OBJ_NOEVENT;
}

void CBonusStage::Late_Update_Scene()
{
	CGameObject_Manager::Get_Instance()->Late_Update_GameObject();
}

void CBonusStage::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject();
}

void CBonusStage::Release_Scene()
{
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::TILE);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::TILE_2);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::TERRAIN);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::PORTAL);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::SHOP);
	CSoundManager::Get_Instance()->StopBGM();
}

CBonusStage * CBonusStage::Create()
{
	CBonusStage* pInstance = new CBonusStage;

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		ERR_MSG(L"CreateFailed BonusStage");
		return nullptr;
	}
	return pInstance;
}

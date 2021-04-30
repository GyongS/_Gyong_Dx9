#include "stdafx.h"
#include "Stage7.h"
#include "GameObject_Manager.h"
#include "Terrain_Client.h"
#include "Player.h"
#include "BossHPBack.h"
#include "BossHPBar.h"
#include "BossHPDamage.h"
#include "Clegane.h"
#include "SoundManager.h"


CStage7::CStage7()
{
}


CStage7::~CStage7()
{	
	Release_Scene();
}

HRESULT CStage7::Ready_Scene()
{
	CSoundManager::Get_Instance()->PlayBGM(L"Chapter1_Boss.wav");

	static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_GroundCheck(false);

	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::TERRAIN, CTerrain_Client::Create(L"../Data/Stage7.dat"));

	CGameObject_Manager::Get_Instance()->Get_Player()->Set_Pos(150.f, 100.f);

	CGameObject* pObj = CAbstractFactory<CClegane>::Create(800, 460);
	pObj->Set_Target(CGameObject_Manager::Get_Instance()->Get_Player());
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::BOSS, pObj);

	pObj = CAbstractFactory<CBossHPBack>::Create(400, 100);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::BOSS_UI, pObj);

	pObj = CAbstractFactory<CBossHPBar>::Create(397, 108);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::BOSS_UI, pObj);

	pObj = CAbstractFactory<CBossHPDamage>::Create(397, 108);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::BOSS_UI, pObj);

	return S_OK;
}

int CStage7::Update_Scene()
{
	CGameObject_Manager::Get_Instance()->Update_GameObject();
	return OBJ_NOEVENT;
}

void CStage7::Late_Update_Scene()
{
	CGameObject_Manager::Get_Instance()->Late_Update_GameObject();

}

void CStage7::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject();

}

void CStage7::Release_Scene()
{
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::TILE);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::TILE_2);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::TERRAIN);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::BOSS);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::BOSS_UI);
}

CStage7 * CStage7::Create()
{
	CStage7* pInstance = new CStage7;

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		ERR_MSG(L"CreateFailed Stage7");
		return nullptr;
	}
	return pInstance;
}

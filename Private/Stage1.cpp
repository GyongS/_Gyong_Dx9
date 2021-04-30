#include "stdafx.h"
#include "Stage1.h"
#include "GameObject_Manager.h"
#include "Player.h"
#include "PlayerInfoUI.h"
#include "PlayerMainIcon.h"
#include "PlayerSwitchIcon.h"
#include "Skill_Icon_A.h"
#include "Skill_Icon_S.h"
#include "Skill_Icon_D.h"
#include "Terrain_Client.h"
#include "Minimap.h"
#include "Scroll_Manager.h"
#include "Gold_Icon.h"
#include "PlayerHPbar.h"
#include "PlayerServeHPBar.h"
#include "PlayerSwitchIcon.h"
#include "Event_UI_Top.h"
#include "Event_UI_Bottom.h"
#include "DarkQuartz_Icon.h"
#include "Key_Manager.h"
#include "Scene_Manager.h"
#include "EventManager.h"
#include "Stage1Helper.h"
#include "Portal.h"
#include "TutorialSkeleton.h"
#include "MinimapPlayer.h"
#include "MinimapPortal.h"
#include "Warrior.h"
#include "SamuraiHead.h"
#include "SoundManager.h"
#include "MonsterIcon.h"


CStage1::CStage1()
{
}


CStage1::~CStage1()
{
	Release_Scene();
}

HRESULT CStage1::Ready_Scene()
{
	CSoundManager::Get_Instance()->PlayBGM(L"Tutorial.wav");
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::TERRAIN, CTerrain_Client::Create(L"../Data/Stage1.dat"));
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::PLAYER, CPlayer::Create());	
	

	//CGameObject* pObj = CAbstractFactory<CBrownWood>::Create(300, 300);
	//pObj->Set_Target(CGameObject_Manager::Get_Instance()->Get_Player());
	//CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MONSTER, pObj);

	//pObj = CAbstractFactory<CGreenWood>::Create(500, 300);
	//pObj->Set_Target(CGameObject_Manager::Get_Instance()->Get_Player());
	//CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MONSTER, pObj);

	//pObj = CAbstractFactory<CWarrior>::Create(700, 300);
	//pObj->Set_Target(CGameObject_Manager::Get_Instance()->Get_Player());
	//CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MONSTER, pObj);

	//pObj = CAbstractFactory<CGiantWood>::Create(500, 400);
	//pObj->Set_Target(CGameObject_Manager::Get_Instance()->Get_Player());
	//CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::GIANTWOOD, pObj);
	

	Start_Create_UI();
	Tutorial_Start();
	
	return S_OK;
}

int CStage1::Update_Scene()
{
	StageEvent();	

	if (CKey_Manager::Get_Instance()->Key_Down(KEY_Q))
	{
		CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_STAGE2);
	}
	CGameObject_Manager::Get_Instance()->Update_GameObject();
	return OBJ_NOEVENT;
}

void CStage1::Late_Update_Scene()
{
	CGameObject_Manager::Get_Instance()->Late_Update_GameObject();

}

void CStage1::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject();

}

void CStage1::Release_Scene()
{
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::TUTORIAL);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::PORTAL);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::TILE);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::TILE_2);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::TERRAIN);
	CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::MINIMAP_PORTAL);
}

CStage1 * CStage1::Create()
{
	CStage1* pInstance = new CStage1;
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		ERR_MSG(L"CreateFailed Stage");
		return nullptr;
	}
	return pInstance;
}

void CStage1::Start_Create_UI()
{
	CGameObject* pObj = CAbstractFactory<CPlayerInfoUI>::Create(120.f, 560.f);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);

	pObj = CAbstractFactory<CPlayerMainIcon>::Create(50.f, 553.f);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);

	pObj = CAbstractFactory<CPlayerSwitchIcon>::Create(25.f, 575.f);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::SWITCH_ICON, pObj);

	pObj = CAbstractFactory<CSkill_Icon_A>::Create(97.f, 552.f);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);

	pObj = CAbstractFactory<CSkill_Icon_S>::Create(138.f, 552.f);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);

	pObj = CAbstractFactory<CSkill_Icon_D>::Create(179.f, 552.f);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);

	pObj = CAbstractFactory<CMinimap>::Create(710.f, 544.f);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MINIMAP, pObj);

	pObj = CAbstractFactory<CMinimapPlayer>::Create(0.f, 0.f);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MINIMAP, pObj);

	pObj = CAbstractFactory<CGold_Icon>::Create(607.f, 540.f);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);

	pObj = CAbstractFactory<CDarkQuartz_Icon>::Create(608.f, 565.f);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);
	
	pObj = CAbstractFactory<CPlayerServeHPBar> ::Create(140.f, 577.f);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);

	pObj = CAbstractFactory<CPlayerHPbar>::Create(140.f, 577.f);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);
	
	pObj = CAbstractFactory<CEvent_UI_Top>::Create(400.f, -200.f);
	CGameObject_Manager::Get_Instance()-> Add_GameObject(OBJ_ID::EVENT_UI, pObj);

	pObj = CAbstractFactory<CEvent_UI_Bottom>::Create(400.f, 800.f);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EVENT_UI, pObj);

	pObj = CAbstractFactory<CMonsterIcon>::Create(758.f, 470);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MONSTER_ICON, pObj);
}

void CStage1::Tutorial_Start()
{
	CGameObject* pObj = CAbstractFactory<CStage1Helper>::Create(1766.f, 30.f);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::TUTORIAL, pObj);

	pObj = CAbstractFactory<CPortal>::Create(1664.f, 417.f);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::PORTAL, pObj);

	pObj = CAbstractFactory<CMinimapPortal>::Create(0, 0);
	pObj->Set_Target(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::PORTAL));
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MINIMAP_PORTAL, pObj);

	pObj = CAbstractFactory<CTutorialSkeleton>::Create(1432.f, 133.f);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::TUTORIAL, pObj);
}

void CStage1::StageEvent()
{
	if (CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos.x >= 741
		&& CEventManager::Get_Instance()->Get_EventCnt() < 1)
	{
		CEventManager::Get_Instance()->Set_EventCnt();
		CEventManager::Get_Instance()->Set_StartEvent(true);
	}
	else if (CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos.x >= 1385
		&& CEventManager::Get_Instance()->Get_EventCnt() < 2)
	{
		CEventManager::Get_Instance()->Set_EventCnt();
		CEventManager::Get_Instance()->Set_StartEvent(true);
	}

}

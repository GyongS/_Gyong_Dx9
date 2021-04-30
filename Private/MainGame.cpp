#include "stdafx.h"
#include "MainGame.h"
#include "GraphicDevice.h"
#include "Time_Manager.h"
#include "Frame_Manager.h"
#include "Texture_Manager_Client.h"
#include "Scene_Manager.h"
#include "GameObject_Manager.h"
#include "Key_Manager.h"
#include "Scroll_Manager.h"
#include "SoundManager.h"

//#define _CRTDBG_MAP_ALLOC
//#include <cstdlib>
//#include <crtdbg.h>
//
//#ifdef _DEBUG
//#define new new ( _NORMAL_BLOCK, __FILE__, __LINE__)
//#endif

CMainGame::CMainGame() : m_pGrphic_Device(CGraphicDevice::Get_Instance())
{
}


CMainGame::~CMainGame()
{
	Release();
}

HRESULT CMainGame::Ready_MainGame()
{
	srand((unsigned)time(NULL));

	CTime_Manager::Get_Instance()->Ready_TimeManager();
	if (m_pGrphic_Device->Ready_Graphic_Device(CGraphicDevice::MODE_WIN))
	{
		ERR_MSG(L"Creating Failed Graphic_Device ");
		return E_FAIL;
	}	

	CSoundManager::Get_Instance()->Initialize();
	Single_Resources_Load();
	CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_LOADING);
	return S_OK; // S_OK ,NOERROR; - 성공시 반환하는 반환 타입. 
}

void CMainGame::Update()
{	
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CKey_Manager::Get_Instance()->Update_KeyManager();
	CTime_Manager::Get_Instance()->Update_TimeManager();
	CScene_Manager::Get_Instance()->Update_Scene();
}

void CMainGame::Late_Update()
{
	CScene_Manager::Get_Instance()->Late_Update_Scene();
	CScroll_Manager::Scroll_Lock();

}

void CMainGame::Render(CFrame_Manager* pFrameManager)
{
	m_pGrphic_Device->Render_Begin();
	CScene_Manager::Get_Instance()->Render_Scene();
	pFrameManager->RenderFPS_TimeManager();
	m_pGrphic_Device->Render_End();
}

void CMainGame::Release()
{	
	CKey_Manager::Destroy_Instance();
	CTime_Manager::Destroy_Instance();
	CScene_Manager::Destroy_Instance();
	CGameObject_Manager::Destroy_Instance();
	CScene_Manager::Destroy_Instance();
	CTexture_Manager_Client::Destroy_Instance();
	CSoundManager::Destroy_Instance();
	m_pGrphic_Device->Destroy_Instance();
}

HRESULT CMainGame::Single_Resources_Load()
{

	// 타이틀
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Logo/Title_Art2.png", L"Logo")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Logo/Title_Logo2.png", L"Logo2")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/HealthBar/Info_UI.png", L"INFO")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/HealthBar/Info_UI.png", L"INFO")))
		return E_FAIL;
	
	// 인게임 UI
	Load_Single_UI_Texture();	

	// 미니맵
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Minimap/Minimap.png", L"Minimap")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Minimap/MiniPlayer.png", L"MiniPlayer")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Minimap/MiniMonster.png", L"MiniMonster")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Minimap/MiniPortal.png", L"MiniPortal")))
		return E_FAIL;

	// 타일
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::MULTI_TEX, L"../Resources/MAP/Tile/Tile_%02d.png", L"Terrain", L"Tile", 42)))
		return E_FAIL;

	// 장식
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::MULTI_TEX, L"../Resources/MAP/DECORATION/Deco_%02d.png", L"Decoration", L"Deco", 74)))
		return E_FAIL;

	// 타일 콜리전
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/MAP/COLLISION/TileCol_01.png", L"Tile_Col")))
		return E_FAIL;

	// 오브젝트 콜리전
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/MAP/COLLISION/TileCol_02.png", L"Object_Col")))
		return E_FAIL;

	// 머리
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/SWITCH_OBJ/Prisoner.png", L"Prisoner_Head")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/SWITCH_OBJ/Samurai.png", L"Samurai_Head")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/SWITCH_OBJ/Skul.png", L"Skul_Head")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/SWITCH_OBJ/GrimReaper_Head.png", L"GrimReaper_Head")))
		return E_FAIL;


	// 프리즈너 스킬 오브젝트
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/SKILL_OBJ/PRISONER_SKILL_OBJ/IceCross_Shot/IceCrossbow_Projectile.png", L"IceCross_Shot")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/SKILL_OBJ/PRISONER_SKILL_OBJ/GRENADE/PowerfulGrenade_Projectile_0.png", L"GRENADE")))
		return E_FAIL;


	// 돌맹이 리소스
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/SKILL_OBJ/StoneShower/Stone_0.png", L"Stone_0")))
		return E_FAIL;
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/SKILL_OBJ/StoneShower/Stone_1.png", L"Stone_1")))
		return E_FAIL;
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/SKILL_OBJ/StoneShower/Stone_2.png", L"Stone_2")))
		return E_FAIL;
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/SKILL_OBJ/StoneShower/Stone_3.png", L"Stone_3")))
		return E_FAIL;
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/SKILL_OBJ/StoneShower/Stone_4.png", L"Stone_4")))
		return E_FAIL;

	// 튜토리얼
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/Tutorial/Stage1Tutorial.png", L"Stage1Tutorial")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/SKELETON_NOWEAPON/Noweapon_0.png", L"Noweapon_0")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/Tutorial/InPortalPopup.png", L"InPortalPopup")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/Tutorial/InShopPopup.png", L"InShopPopup")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/MAP/Cage/Cage_Front.png", L"Cage_Front")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/MAP/Cage/Cage_Front_Destroyed.png", L"Cage_Front_Destroyed")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/Cage_Sit/Idle_Cage_0.png", L"Idle_Cage_0")))
		return E_FAIL;

	// 빙결 상태
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/Debuff/freeze_large.png", L"freeze_large")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/Debuff/freeze_medium1.png", L"freeze_medium1")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/Debuff/freeze_medium2.png", L"freeze_medium2")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/Debuff/freeze_small.png", L"freeze_small")))
		return E_FAIL;

	// 피격 UI
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/HIT_UI.png", L"HIT_UI")))
		return E_FAIL;

	// 스컬 머리
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/SKiLL_OBJ/PARTS/Skul.png", L"SkulSkillHead")))
		return E_FAIL;

}

HRESULT CMainGame::Load_Single_UI_Texture()
{
	// 기본 아이콘
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Face/Skul_Face.png", L"Skul_Face")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Face/Prisoner_Face.png", L"Prisoner_Face")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Face/Samurai_Face.png", L"Samurai_Face")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Face/GrimReaper _Face.png", L"GrimReaper _Face")))
		return E_FAIL;


	// 스위치 아이콘
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Face/Skul_Switch_Face.png", L"Skul_Switch_Face")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Face/Prisoner_Switch_Face.png", L"Prisoner_Switch_Face")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Face/Samurai_Switch_Face.png", L"Samurai_Switch_Face")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Face/GrimReaper_Switch_Face.png", L"GrimReaper_Switch_Face")))
		return E_FAIL;

	// 기본 모드 스킬
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Skill_ICON/Default/SkullThrowing.png", L"Skill_SkullThrowing")))
		return E_FAIL;


	// 프리즈너 모드 스킬
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Skill_ICON/Prisoner/Broadsword.png", L"Skill_Broadsword")))
		return E_FAIL;
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Skill_ICON/Prisoner/IceCrossbow.png", L"Skill_IceCrossbow")))
		return E_FAIL;
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Skill_ICON/Prisoner/PowerfulGrenade.png", L"Skill_PowerfulGrenade")))
		return E_FAIL;

	// 사무라이 모드 스킬
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Skill_ICON/Samurai/Ilseom.png", L"Skill_Ilseom")))
		return E_FAIL;

	// 그림리퍼 모드 스킬
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Skill_ICON/GrimReaper/TheStake.png", L"TheStake")))
		return E_FAIL;
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Skill_ICON/GrimReaper/Harvest.png", L"Harvest")))
		return E_FAIL;
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Skill_ICON/GrimReaper/DarkCharging.png", L"DarkCharging")))
		return E_FAIL;

	// 골드 아이콘
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/GOLD_ICON/GOLD_ICON.png", L"GOLD_ICON")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/DARKQUARTZ_ICON/DarkQuartz_Icon.png", L"DARKQUARTZ_ICON")))
		return E_FAIL;

	// 체력바
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/HealthBar/Player_HPBar.png", L"Player_HPBar")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/HealthBar/Player_HPBar_Serve.png", L"Player_HPBar_Serve")))
		return E_FAIL;

	// 몬스터 체력바
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Monster_UI/EnemyHealthBar_Back.png", L"EnemyHealthBar_Back")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Monster_UI/EnemyHealthBar.png", L"EnemyHealthBar")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Monster_UI/EnemyHealthBar_Damage.png", L"EnemyHealthBar_Damage")))
		return E_FAIL;

	// 이벤트 UI 
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Event_UI/Event_UI_0.png", L"Event_UI")))
		return E_FAIL;

	// 보스 UI
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/BOSS_UI/BOSS_HP_BACK_2.png", L"BOSS_HP_BACK_2")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/BOSS_UI/BOSS_HP_BAR.png", L"BOSS_HP_BAR")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/BOSS_UI/BOSS_HP_DAMAGE.png", L"BOSS_HP_DAMAGE")))
		return E_FAIL;

	// 다이얼로그
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Dialogue/Dialogue_Body.png", L"Dialogue_Body")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Dialogue/Dialogue_Enter.png", L"Dialogue_Enter")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Face/Skul_Tutorial.png", L"Skul_Tutorial")))
		return E_FAIL;

	// 쿨타임
	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Face/Skul_Tutorial.png", L"Skul_Tutorial")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/Swap_CoolTime/Swap_CoolTime.png", L"Swap_CoolTime")))
		return E_FAIL;

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/CoolTime/CoolTime.png", L"CoolTime")))
		return E_FAIL;

	// 몬스터 아이콘

	if (FAILED(CTexture_Manager_Client::Get_Instance()->Insert_Texture_Manager(TEX_ID::SINGLE_TEX, L"../Resources/UI/EnemyIcon/Enemy_Icon.png", L"Enemy_Icon")))
		return E_FAIL;
}

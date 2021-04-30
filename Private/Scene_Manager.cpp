#include "stdafx.h"
#include "Scene_Manager.h"
#include "Scene.h"
#include "LoadingScene.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "Stage4.h"
#include "Stage5.h"
#include "Stage6.h"
#include "Stage7.h"
#include "BonusStage.h"


IMPLEMENT_SINGLETON(CScene_Manager)
CScene_Manager::CScene_Manager()
	:m_eCurScene(SCENE_END)
	, m_eNextScene(SCENE_END)
{
}


CScene_Manager::~CScene_Manager()
{
	Release_Scene();
}

void CScene_Manager::Scene_Change_SceneManager(ID eNextScene)
{
	m_eNextScene = eNextScene;
	if (m_eNextScene != m_eCurScene)
	{
		Safe_Delete(m_pScene);
		switch (m_eNextScene)
		{
		case CScene_Manager::SCENE_LOADING:
			m_pScene = new CLoadingScene;
			m_pScene->Ready_Scene();
			break;
		case CScene_Manager::SCENE_STAGE1:
			m_pScene = CStage1::Create();
			break;
		case CScene_Manager::SCENE_STAGE2:
			m_pScene = CStage2::Create();
			break;
		case CScene_Manager::SCENE_STAGE3:
			m_pScene = CStage3::Create();
			break;
		case CScene_Manager::SCENE_STAGE4:
			m_pScene = CStage4::Create();
			break;
		case CScene_Manager::SCENE_STAGE5:
			m_pScene = CStage5::Create();
			break;
		case CScene_Manager::SCENE_STAGE6:
			m_pScene = CStage6::Create();
			break;
		case CScene_Manager::SCENE_STAGE7:
			m_pScene = CStage7::Create();
			break;
		case CScene_Manager::BONUS_STAGE:
			m_pScene = CBonusStage::Create();
			break;
		case CScene_Manager::SCENE_END:
			break;
		default:
			break;
		}
		m_eCurScene = m_eNextScene;
	}
}

void CScene_Manager::Update_Scene()
{
	m_pScene->Update_Scene();
}

void CScene_Manager::Late_Update_Scene()
{
	m_pScene->Late_Update_Scene();
}

void CScene_Manager::Render_Scene()
{
	m_pScene->Render_Scene();
}

void CScene_Manager::Release_Scene()
{
	m_pScene->Release_Scene();
}
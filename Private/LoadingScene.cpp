#include "stdafx.h"
#include "Scene_Manager.h"
#include "LoadingScene.h"
#include "Key_Manager.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "SoundManager.h"

CLoadingScene::CLoadingScene() : m_bLoading(false), m_fLogoAlpha(0.f)
{
}


CLoadingScene::~CLoadingScene()
{
	Release_Scene();
}

HRESULT CLoadingScene::Ready_Scene()
{
	CSoundManager::Get_Instance()->PlayBGM(L"Adventurer.wav");

	InitializeCriticalSection(&m_CriticalSection);
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, CLoadingScene::LoadImageFunc, this, 0, nullptr);

	return S_OK;
}

int CLoadingScene::Update_Scene()
{
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_RETURN) && m_fLogoAlpha >= 255.f)
	{
		WaitForSingleObject(m_hThread, INFINITE);

		CloseHandle(m_hThread);

		DeleteCriticalSection(&m_CriticalSection);

		CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_STAGE1);
		return 0;
	}
	return 0;
}

void CLoadingScene::Late_Update_Scene()
{
}

void CLoadingScene::Render_Scene()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(L"Logo");
	if (nullptr == pTexInfo)
		return;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1), 0.f);

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	wstring wstrFullPath = CTexture_Manager_Client::Get_Instance()->Get_String();
	D3DXMatrixTranslation(&matTrans, 100.f, 500.f, 0.f);
	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphicDevice::Get_Instance()->Get_Font()->DrawTextW(CGraphicDevice::Get_Instance()->Get_Sprite(), wstrFullPath.c_str(), wstrFullPath.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_bLoading)
	{
		if (m_fLogoAlpha < 255.f)
			m_fLogoAlpha += 1.f;

		const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(L"Logo2");
		D3DXMatrixTranslation(&matTrans, 550.f, 600.f, 0.f);
		CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB((DWORD)m_fLogoAlpha, 255, 255, 255));
		
	}
}

void CLoadingScene::Release_Scene()
{
	CSoundManager::Get_Instance()->StopBGM();
}

unsigned CLoadingScene::LoadImageFunc(LPVOID pArg)
{
	CLoadingScene* pLoad = static_cast<CLoadingScene*>(pArg);
	EnterCriticalSection(&pLoad->m_CriticalSection);

	if (FAILED(CTexture_Manager_Client::Get_Instance()->ReadImageFile(L"../Data/PathInfo.txt")))
		return E_FAIL;

	pLoad->m_bLoading = true;
	LeaveCriticalSection(&pLoad->m_CriticalSection);
	return 0;
}

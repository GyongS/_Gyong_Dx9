#include "stdafx.h"
#include "Terrain_Client.h"
#include "GraphicDevice.h"
#include "Time_Manager.h"
#include "Scroll_Manager.h"
#include "Texture_Manager_Client.h"
#include "GameObject_Manager.h"
#include "CollisionManager.h"
#include "Tile_Collision.h"
#include "Collision.h"
#include "Scene_Manager.h"
#include "Minimap.h"

CTerrain_Client::CTerrain_Client()
{
}


CTerrain_Client::~CTerrain_Client()
{
	Release_GameObject();
}

HRESULT CTerrain_Client::Ready_GameObject()
{
	if (CScene_Manager::Get_Instance()->Get_Scene() == CScene_Manager::SCENE_STAGE1)
	{
		LoadTileData(L"../Data/Stage1.dat");
	}	
	else if (CScene_Manager::Get_Instance()->Get_Scene() == CScene_Manager::SCENE_STAGE2)
	{
		LoadTileData(L"../Data/Stage2.dat");
	}
	else if (CScene_Manager::Get_Instance()->Get_Scene() == CScene_Manager::SCENE_STAGE3)
	{ 
		LoadTileData(L"../Data/Stage3.dat");
	}
	else if (CScene_Manager::Get_Instance()->Get_Scene() == CScene_Manager::SCENE_STAGE4)
	{
		LoadTileData(L"../Data/Stage4.dat");
	}
	else if (CScene_Manager::Get_Instance()->Get_Scene() == CScene_Manager::SCENE_STAGE5)
	{
		LoadTileData(L"../Data/Stage5.dat");
	}
	else if (CScene_Manager::Get_Instance()->Get_Scene() == CScene_Manager::BONUS_STAGE)
	{
		LoadTileData(L"../Data/BonusStage.dat");
	}
	else if (CScene_Manager::Get_Instance()->Get_Scene() == CScene_Manager::SCENE_STAGE6)
	{
		LoadTileData(L"../Data/Stage6.dat");
	}
	else if (CScene_Manager::Get_Instance()->Get_Scene() == CScene_Manager::SCENE_STAGE7)
	{
		LoadTileData(L"../Data/Stage7.dat");
	}

	return S_OK;
}

int CTerrain_Client::Update_GameObject()
{
	/*POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);

	D3DXVECTOR3 vSpeed = {};
	vSpeed.x = 300.f * CTime_Manager::Get_Instance()->Get_DeltaTime();
	vSpeed.y = 300.f * CTime_Manager::Get_Instance()->Get_DeltaTime();*/

	/*if (0 > pt.x)
		CScroll_Manager::Set_ScrollPos({ vSpeed.x, 0.f, 0.f });
	if (WINCX < pt.x)
		CScroll_Manager::Set_ScrollPos({ -vSpeed.x, 0.f, 0.f });
	if (0 > pt.y)
		CScroll_Manager::Set_ScrollPos({ 0.f, vSpeed.y,0.f });
	if (WINCY < pt.y)
		CScroll_Manager::Set_ScrollPos({ 0.f, -vSpeed.y,0.f });*/	

	return 0;
}

void CTerrain_Client::Late_Update_GameObject()
{
}

void CTerrain_Client::Render_GameObject()
{
	Render_Decoration();

	int iScrollX = abs((int)CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X));
	int iIndex = 0;

	int iCullX = iScrollX / TILECX;
	int iCullY = 0;
	int iCullEndX = iCullX + (WINCX / TILECX) + 2;
	int iCullEndY = iCullY + (WINCY / TILECY) + 2;

	for (int i = iCullY; i < iCullEndY; ++i)
	{
		for (int j = iCullX; j < iCullEndX; ++j)
		{
			iIndex = j + (i * TILEX);

			if (m_vecTile[iIndex]->byDrawID == 36 || 0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
				continue;			

			const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(L"Terrain", L"Tile", m_vecTile[iIndex]->byDrawID);

			if (nullptr == pTexInfo)
				return;

			float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
			float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

			D3DXMATRIX matScale, matTrans, matWorld;
			D3DXMatrixScaling(&matScale, m_vecTile[iIndex]->vSize.x , m_vecTile[iIndex]->vSize.y, 0.f);
			D3DXMatrixTranslation(&matTrans, m_vecTile[iIndex]->vPos.x + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X), m_vecTile[iIndex]->vPos.y + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y), 0.f);
			matWorld = matScale * matTrans;

   			CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
			CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));			
						
		}
	}

	Render_MiniMap();

}

void CTerrain_Client::Release_GameObject()
{
	for (auto& pTile : m_vecTile)
		Safe_Delete(pTile);
	m_vecTile.clear();
	m_vecTile.swap(vector<TILE*>());

	for (auto& pMapObject : m_vecMapObject)
		Safe_Delete(pMapObject);
	m_vecMapObject.clear();
	m_vecMapObject.swap(vector<MAPOBJECT*>());

	for (auto& pTile_Col : m_vecCollision)
		Safe_Delete(pTile_Col);
	m_vecCollision.clear();
	m_vecCollision.swap(vector<TILE_COLLISION*>());
}

void CTerrain_Client::LoadTileData(const wstring & wstrFilePath)
{
	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return;
	}

	DWORD dwByte = 0;
	TILE* pTile = nullptr;
	MAPOBJECT* pMapObject = nullptr;
	TILE_COLLISION* pTile_Col = nullptr;

	// 타일
	int iTileSize = 0;
	ReadFile(hFile, &iTileSize, sizeof(int), &dwByte, nullptr);
	for (size_t i = 0; i < iTileSize; ++i)
	{
		pTile = new TILE;
		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);
		m_vecTile.emplace_back(pTile);
	}

	// 맵 오브젝트
	int iMapSize = 0;
	ReadFile(hFile, &iMapSize, sizeof(int), &dwByte, nullptr);
	for (size_t i = 0; i < iMapSize; ++i)
	{
		pMapObject = new MAPOBJECT;
		ReadFile(hFile, pMapObject, sizeof(MAPOBJECT), &dwByte, nullptr);
		m_vecMapObject.emplace_back(pMapObject);
	}

	// 타일 콜리전
	int iColSize = 0;
	ReadFile(hFile, &iColSize, sizeof(int), &dwByte, nullptr);
	for (size_t i = 0; i < iColSize; ++i)
	{
		pTile_Col = new TILE_COLLISION;
		ReadFile(hFile, pTile_Col, sizeof(TILE_COLLISION), &dwByte, nullptr);
		m_vecCollision.emplace_back(pTile_Col);		
		if (pTile_Col->byDrawID == 2)
		{
			CGameObject* pObj = CAbstractFactory<CTile_Collision>::CreateTileCol(pTile_Col->vPos);
			pObj->Set_OBJID(OBJ_ID::TILE_2);				
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::TILE_2, pObj);
		}
		else
		{
			CGameObject* pObj = CAbstractFactory<CTile_Collision>::CreateTileCol(pTile_Col->vPos);
			pObj->Set_OBJID(OBJ_ID::TILE);
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::TILE, pObj);
		}
		
	}

	CloseHandle(hFile);
}

void CTerrain_Client::Render_Decoration()
{
	for (auto& pObj : m_vecMapObject)
	{
		const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(L"Decoration", L"Deco", (*pObj).byDrawID);

		if (nullptr == pTexInfo)
			return;

		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, (*pObj).vSize.x, (*pObj).vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, (*pObj).vPos.x + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X), (*pObj).vPos.y + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y), 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));		
	}
}

void CTerrain_Client::Render_Tile_Collision()
{
	for (auto& pObj : m_vecCollision)
	{
		const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(L"Tile_Col");

		if (nullptr == pTexInfo)
			return;

		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, (*pObj).vSize.x, (*pObj).vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, (*pObj).vPos.x, (*pObj).vPos.y, 0.f);
		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CTerrain_Client::Render_MiniMap()
{
	int iScrollX = abs((int)CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X));
	int iIndex = 0;

	int iCullX = iScrollX / TILECX;
	int iCullY = 0;
	int iCullEndX = iCullX + (WINCX / TILECX) + 2;
	int iCullEndY = iCullY + (WINCY / TILECY) + 2;

	for (int i = iCullY; i < iCullEndY; ++i)
	{
		for (int j = iCullX; j < iCullEndX; ++j)
		{
			iIndex = j + (i * TILEX);

			if (m_vecTile[iIndex]->byDrawID == 36 || 0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
				continue;

			const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(L"Terrain", L"Tile", m_vecTile[iIndex]->byDrawID);

			if (nullptr == pTexInfo)
				return;

			float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
			float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

			D3DXMATRIX matScale, matTrans, matWorld;
			D3DXMatrixScaling(&matScale, m_vecTile[iIndex]->vSize.x, m_vecTile[iIndex]->vSize.y, 0.f);
			D3DXMatrixTranslation(&matTrans, m_vecTile[iIndex]->vPos.x + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X) + 3150, m_vecTile[iIndex]->vPos.y + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y) + 2350, 0.f);

			matWorld = matScale * matTrans;
			Set_Ratio(matWorld, 0.2f, 0.2f);
			CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
			CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 0, 0, 0));

		}
	}
}

void CTerrain_Client::Set_Ratio(D3DXMATRIX & matWorld, float fRatioX, float fRatioY)
{
	/*
	x	y	z	w
	x	11	12	13	14
	y	21	22	23	24
	z	31	32	33	34
	w	41	42	43	44
	*/
	matWorld._11 *= fRatioX;
	matWorld._21 *= fRatioX;
	matWorld._31 *= fRatioX;
	matWorld._41 *= fRatioX;

	matWorld._12 *= fRatioY;
	matWorld._22 *= fRatioY;
	matWorld._32 *= fRatioY;
	matWorld._42 *= fRatioY;
}

CTerrain_Client* CTerrain_Client::Create(const wstring & wstrFilePath)
{
	CTerrain_Client* pInstance = new CTerrain_Client;
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Delete(pInstance);
		return nullptr;
	}
	return pInstance;
}

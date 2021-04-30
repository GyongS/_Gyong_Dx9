#include "stdafx.h"
#include "Tile_Collision.h"
#include "Texture_Manager_Client.h"
#include "GraphicDevice.h"
#include "Scroll_Manager.h"


CTile_Collision::CTile_Collision()
{
}


CTile_Collision::~CTile_Collision()
{
}

HRESULT CTile_Collision::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.iColCX = 32;
	m_tInfo.iColCY = 28;
	m_pObjectKey = L"Tile_Col";
	return S_OK;
}

int CTile_Collision::Update_GameObject()
{
	CGameObject::Late_Ready_GameObject();
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CTile_Collision::Late_Update_GameObject()
{
}

void CTile_Collision::Render_GameObject()
{
	/*const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

	if (nullptr == pTexInfo)
		return;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXMatrixScaling(&matScale,m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X), m_tInfo.vPos.y + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y), 0.f);

	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));*/
}

void CTile_Collision::Release_GameObject()
{
}

HRESULT CTile_Collision::Late_Ready_GameObject()
{
	if (m_eOBJID == OBJ_ID::TILE_2)
	{
		m_tInfo.iColCX = 15;
		m_tInfo.iColCY = 15;
	}
	else
	{
		m_tInfo.iColCX = 32;
		m_tInfo.iColCY = 28;
	}
	return S_OK;
}

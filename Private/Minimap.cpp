#include "stdafx.h"
#include "Minimap.h"
#include "Texture_Manager_Client.h"
#include "GraphicDevice.h"
#include "Player.h"

CMinimap::CMinimap()
{
}


CMinimap::~CMinimap()
{
}

HRESULT CMinimap::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f ,1.f, 0.f };
	m_pObjectKey = L"Minimap";
	return S_OK;
}

HRESULT CMinimap::Late_Ready_GameObject()
{
	return E_NOTIMPL;
}

int CMinimap::Update_GameObject()
{
	return OBJ_NOEVENT;
}

void CMinimap::Late_Update_GameObject()
{
}

void CMinimap::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

	if (nullptr == pTexInfo)
		return;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CMinimap::Release_GameObject()
{
}

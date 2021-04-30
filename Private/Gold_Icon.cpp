#include "stdafx.h"
#include "Gold_Icon.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Player.h"
#include "GameObject_Manager.h"

CGold_Icon::CGold_Icon()
{
}


CGold_Icon::~CGold_Icon()
{
}

HRESULT CGold_Icon::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f ,1.f, 0.f };
	m_pObjectKey = L"GOLD_ICON";
	return S_OK;
}

int CGold_Icon::Update_GameObject()
{
	return OBJ_NOEVENT;
}

void CGold_Icon::Late_Update_GameObject()
{

}

void CGold_Icon::Render_GameObject()
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


	swprintf_s(m_szbuff, L"%d", static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Gold());

	D3DXMATRIX FontmatTrans;
	if (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Gold() <= 100)
		D3DXMatrixTranslation(&FontmatTrans, 583.f, 533.f, 0.f);		

	else if(static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Gold() >= 100 &&
		static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Gold() < 1000)
		D3DXMatrixTranslation(&FontmatTrans, 563.f, 533.f, 0.f);

	else if(static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Gold() >= 1000 &&
		static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Gold() < 10000)
		D3DXMatrixTranslation(&FontmatTrans, 556.f, 533.f, 0.f);

	else if (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Gold() >= 10000)
		D3DXMatrixTranslation(&FontmatTrans, 549.f, 533.f, 0.f);
	
	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&FontmatTrans);
	CGraphicDevice::Get_Instance()->Get_Font()->DrawTextW(CGraphicDevice::Get_Instance()->Get_Sprite(), m_szbuff, lstrlen(m_szbuff), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CGold_Icon::Release_GameObject()
{
}

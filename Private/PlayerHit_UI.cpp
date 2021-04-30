#include "stdafx.h"
#include "PlayerHit_UI.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"

CPlayerHit_UI::CPlayerHit_UI() : m_iAlpha(255)
{
}


CPlayerHit_UI::~CPlayerHit_UI()
{
}

HRESULT CPlayerHit_UI::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"HIT_UI";
	return S_OK;
}

int CPlayerHit_UI::Update_GameObject()
{
	m_iAlpha -= 2.f;

	if (m_iAlpha <= 0)
		return OBJ_DEAD;
	

	return OBJ_NOEVENT;
}

void CPlayerHit_UI::Late_Update_GameObject()
{
}

void CPlayerHit_UI::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

	if (pTexInfo == nullptr)
		return;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 0, 0));
}

void CPlayerHit_UI::Release_GameObject()
{
}

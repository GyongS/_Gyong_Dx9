#include "stdafx.h"
#include "PlayerServeHPBar.h"
#include "Texture_Manager_Client.h"
#include "GraphicDevice.h"
#include "GameObject_Manager.h"
#include "Player.h"
#include "Key_Manager.h"
#include "Time_Manager.h"
#include "Math_Manager.h"
#include "PlayerHPbar.h"

CPlayerServeHPBar::CPlayerServeHPBar()
{
}


CPlayerServeHPBar::~CPlayerServeHPBar()
{
}

HRESULT CPlayerServeHPBar::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f ,1.f, 0.f };
	m_pObjectKey = L"Player_HPBar_Serve";
	return S_OK;
}

HRESULT CPlayerServeHPBar::Late_Ready_GameObject()
{
	return S_OK;
}

int CPlayerServeHPBar::Update_GameObject()
{	
	return OBJ_NOEVENT;
}

void CPlayerServeHPBar::Late_Update_GameObject()
{
}

void CPlayerServeHPBar::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

	if (nullptr == pTexInfo)
		return;

	m_fFillAmount = static_cast<CPlayerHPbar*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::UI))->Get_BackFillAmount();

	if (0.f > m_fFillAmount)
		m_fFillAmount = 0.f;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	RECT rcTemp = { 0, 0,pTexInfo->tImageInfo.Width * m_fFillAmount, pTexInfo->tImageInfo.Height };
	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rcTemp, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CPlayerServeHPBar::Release_GameObject()
{
}

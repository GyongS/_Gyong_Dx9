#include "stdafx.h"
#include "Ghost_Effect.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Player.h"
#include "GameObject_Manager.h"
#include "Scroll_Manager.h"

CGhost_Effect::CGhost_Effect() : m_iAlpha(0)
{
}


CGhost_Effect::~CGhost_Effect()
{
}

HRESULT CGhost_Effect::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f ,1.f, 0.f };	
	m_iAlpha = 255;
	return S_OK;
}

int CGhost_Effect::Update_GameObject()
{
	m_iAlpha -= 10;

	if (m_iAlpha < 0)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CGhost_Effect::Late_Update_GameObject()
{

}

void CGhost_Effect::Render_GameObject()
{	
	const TEXINFO* pTexInfo = nullptr;

	if (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Type() == CPlayer::TYPE_DEFAULT)
	{
		pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(L"DEFAULT", L"DEFAULT_DASH", 1.f);	
	}
	else if (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Type() == CPlayer::TYPE_PRISONER)
	{
		pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(L"PRISONER_MODE", L"PRISONER_MODE_DASH", 1.f);
	}
	else if (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Type() == CPlayer::TYPE_SAMURAI)
	{
		pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(L"SAMURAI_MODE", L"SAMURAI_MODE_DASH", 1.f);
	}

	if (nullptr == pTexInfo)
		return;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;


	D3DXMATRIX matTrans, matScale, matWorld;
	if (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_bLeftRight())
	{
		D3DXMatrixScaling(&matScale, -m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	}
	else
	{
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	}
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X), m_tInfo.vPos.y + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y), 0.f);

	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 0, 0, 0));
}

void CGhost_Effect::Release_GameObject()
{
}
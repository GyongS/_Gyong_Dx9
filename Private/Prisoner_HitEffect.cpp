#include "stdafx.h"
#include "Prisoner_HitEffect.h"
#include "GraphicDevice.h"
#include "Scroll_Manager.h"
#include "Texture_Manager_Client.h"
#include "Player.h"
#include "GameObject_Manager.h"

CPrisoner_HitEffect::CPrisoner_HitEffect()
{
}


CPrisoner_HitEffect::~CPrisoner_HitEffect()
{
}

HRESULT CPrisoner_HitEffect::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"HIT_EFFECT";
	m_pStateKey = L"HIT_PRISONERMODE";
	m_tFrame.fStartFrame = 0.f;
	m_tFrame.fEndFrame = 7.f;
	return S_OK;
}

HRESULT CPrisoner_HitEffect::Late_Ready_GameObject()
{
	m_bLeftRight = CGameObject_Manager::Get_Instance()->Get_Player()->Get_bLeftRight();
	return S_OK;
}

int CPrisoner_HitEffect::Update_GameObject()
{
	CGameObject::Late_Ready_GameObject();

	if (m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
		return OBJ_DEAD;


	return OBJ_NOEVENT;
}

void CPrisoner_HitEffect::Late_Update_GameObject()
{
	FrameMove(1.5f);
}

void CPrisoner_HitEffect::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey, m_pStateKey, (int)m_tFrame.fStartFrame);

	if (nullptr == pTexInfo)
		return;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans, matScale, matWorld;

	if (m_bLeftRight)
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
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CPrisoner_HitEffect::Release_GameObject()
{
}

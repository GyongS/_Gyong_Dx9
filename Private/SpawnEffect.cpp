#include "stdafx.h"
#include "SpawnEffect.h"
#include "Texture_Manager_Client.h"
#include "GraphicDevice.h"
#include "BrownWood.h"
#include "GameObject_Manager.h"
#include "Scroll_Manager.h"

CSpawnEffect::CSpawnEffect()
{
}


CSpawnEffect::~CSpawnEffect()
{
}

HRESULT CSpawnEffect::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f ,1.f , 0.f };
	m_pObjectKey = L"EFFECT";
	m_pStateKey = L"Enemy_Appearance";
	m_tFrame.fStartFrame = 0.f;
	m_tFrame.fEndFrame = 11.f;
	return S_OK;
}

HRESULT CSpawnEffect::Late_Ready_GameObject()
{
	return E_NOTIMPL;
}

int CSpawnEffect::Update_GameObject()
{
	if (m_bDead)
	{
		CGameObject* pObj = CAbstractFactory<CBrownWood>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
		pObj->Set_Target(CGameObject_Manager::Get_Instance()->Get_Player());
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MONSTER, pObj);
		return OBJ_DEAD;
	}


	return OBJ_NOEVENT;
}

void CSpawnEffect::Late_Update_GameObject()
{
	FrameMove(0.7f);

	if (m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1)
		m_bDead = true;
}

void CSpawnEffect::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey, m_pStateKey, (int)m_tFrame.fStartFrame);

	if (nullptr == pTexInfo)
		return;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X), m_tInfo.vPos.y + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y), 0.f);

	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CSpawnEffect::Release_GameObject()
{
}

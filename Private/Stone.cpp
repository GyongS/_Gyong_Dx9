#include "stdafx.h"
#include "Stone.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Scroll_Manager.h"
#include "Time_Manager.h"
#include "Stone_HitEffect.h"
#include "GameObject_Manager.h"
#include "SoundManager.h"

CStone::CStone() : m_fDelay(0.f)
{
}


CStone::~CStone()
{
}

HRESULT CStone::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.iColCX = 25;
	m_tInfo.iColCY = 25;
	m_fSpeed = 200.f;
	m_eOBJID = OBJ_ID::STONE;
	CSoundManager::Get_Instance()->PlaySound(L"Trap_RockImpact.wav", CSoundManager::STONE_EFFECT);
	return S_OK;
}

int CStone::Update_GameObject()
{
	if (m_bDead)
	{
		CGameObject* pObj = CAbstractFactory<CStone_HitEffect>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);

		CSoundManager::Get_Instance()->PlaySound(L"Trap_RockImpact.wav", CSoundManager::STONE_END);

		return OBJ_DEAD;
	}
	m_fDelay += CTime_Manager::Get_Instance()->Get_DeltaTime();
	if(0.1f <= m_fDelay)
		m_fSpeed += 5;

	m_tInfo.vPos.y += m_fSpeed * CTime_Manager::Get_Instance()->Get_DeltaTime();

	return OBJ_NOEVENT;
}

void CStone::Late_Update_GameObject()
{
}

void CStone::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

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

void CStone::Release_GameObject()
{
}

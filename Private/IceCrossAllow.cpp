#include "stdafx.h"
#include "IceCrossAllow.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Scroll_Manager.h"
#include "GameObject_Manager.h"

CIceCrossAllow::CIceCrossAllow()
{
}


CIceCrossAllow::~CIceCrossAllow()
{	
}

HRESULT CIceCrossAllow::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.iColCX = 60;
	m_tInfo.iColCY = 15;
	m_eOBJID = OBJ_ID::ICE_ALLOW;
	m_fSpeed = 7.f;
	m_pObjectKey = L"IceCross_Shot";

	return S_OK;
}

HRESULT CIceCrossAllow::Late_Ready_GameObject()
{
	m_bLeftRight = CGameObject_Manager::Get_Instance()->Get_Player()->Get_bLeftRight();
	return S_OK;
}

int CIceCrossAllow::Update_GameObject()
{
	CGameObject::Late_Ready_GameObject();
	if (m_bDead)
	{
		return OBJ_DEAD;
	}

	if (m_bLeftRight)
	{		
		if (CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos.x - 300.f >= m_tInfo.vPos.x)
		{
			m_bDead = true;
		}

		m_tInfo.vPos.x -= cosf(m_fRotation * PI / 180.f) * m_fSpeed;
		m_tInfo.vPos.y += sinf(m_fRotation * PI / 180.f) * m_fSpeed;
	}
	else
	{
		if (CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos.x + 300.f <= m_tInfo.vPos.x)
		{
			m_bDead = true;
		}
		m_tInfo.vPos.x += cosf(m_fRotation * PI / 180.f) * m_fSpeed;
		m_tInfo.vPos.y -= sinf(m_fRotation * PI / 180.f) * m_fSpeed;
	}	


	
	return OBJ_NOEVENT;
}

void CIceCrossAllow::Late_Update_GameObject()
{
}

void CIceCrossAllow::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

	if (pTexInfo == nullptr)
		return;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans, matScale, matRotZ, matWorld;
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

void CIceCrossAllow::Release_GameObject()
{
}

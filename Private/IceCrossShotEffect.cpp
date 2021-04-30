#include "stdafx.h"
#include "IceCrossShotEffect.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Scroll_Manager.h"
#include "GameObject_Manager.h"

CIceCrossShotEffect::CIceCrossShotEffect()
{
}


CIceCrossShotEffect::~CIceCrossShotEffect()
{
}

HRESULT CIceCrossShotEffect::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"HIT_EFFECT";
	m_pStateKey  = L"IceCross";
	m_tFrame.fStartFrame = 0.f;
	m_tFrame.fEndFrame = 11.f;
	return S_OK;
}

int CIceCrossShotEffect::Update_GameObject()
{
	if (m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CIceCrossShotEffect::Late_Update_GameObject()
{
	FrameMove(1.f);
}

void CIceCrossShotEffect::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey, m_pStateKey, (int)m_tFrame.fStartFrame);

	if (nullptr == pTexInfo)
		return;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans, matScale, matWorld;
	if (CGameObject_Manager::Get_Instance()->Get_Player()->Get_bLeftRight())
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

void CIceCrossShotEffect::Release_GameObject()
{
}

#include "stdafx.h"
#include "Critical_Effect.h"
#include "GraphicDevice.h"
#include "Scroll_Manager.h"
#include "Texture_Manager_Client.h"
#include "Player.h"
#include "GameObject_Manager.h"
CCritical_Effect::CCritical_Effect()
{
}


CCritical_Effect::~CCritical_Effect()
{
}

HRESULT CCritical_Effect::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"HIT_CRITICAL";
	m_pStateKey = L"CRITICAL_EFFECT";
	m_tFrame.fStartFrame = 0.f;
	m_tFrame.fEndFrame = 11.f;
	return S_OK;
}

HRESULT CCritical_Effect::Late_Ready_GameObject()
{
	m_bLeftRight = CGameObject_Manager::Get_Instance()->Get_Player()->Get_bLeftRight();
	return S_OK;
}

int CCritical_Effect::Update_GameObject()
{
	if (m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
	{
		return OBJ_DEAD;
	}
	return OBJ_NOEVENT;

}

void CCritical_Effect::Late_Update_GameObject()
{
	FrameMove();
}

void CCritical_Effect::Render_GameObject()
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

void CCritical_Effect::Release_GameObject()
{
}

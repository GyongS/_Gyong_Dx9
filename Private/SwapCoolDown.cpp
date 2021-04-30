#include "stdafx.h"
#include "SwapCoolDown.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "GameObject_Manager.h"
#include "SwapCoolDownEffect.h"

CSwapCoolDown::CSwapCoolDown() : m_fSwap_CoolTime(0.f)
{
}


CSwapCoolDown::~CSwapCoolDown()
{
}

HRESULT CSwapCoolDown::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"Swap_CoolTime";
	return S_OK;
}

int CSwapCoolDown::Update_GameObject()
{
	if (m_bDead)
	{
		CGameObject* pObj = CAbstractFactory<CSwapCoolDownEffect>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);
		return OBJ_DEAD;
	}
	m_fSwap_CoolTime = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Stat().fSwap_Cool / CGameObject_Manager::Get_Instance()->Get_Player()->Get_Stat().fSwap_MaxCool;
	return OBJ_NOEVENT;
}

void CSwapCoolDown::Late_Update_GameObject()
{
	if (m_fSwap_CoolTime <= 0)
	{
		m_bDead = true;
	}
}

void CSwapCoolDown::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

	if (pTexInfo == nullptr)
		return;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, -m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	RECT rcTemp = { 0, 0 ,(LONG)pTexInfo->tImageInfo.Width, (LONG)(pTexInfo->tImageInfo.Height * m_fSwap_CoolTime) };

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rcTemp, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(150, 255, 255, 255));
}

void CSwapCoolDown::Release_GameObject()
{
}

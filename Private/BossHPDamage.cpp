#include "stdafx.h"
#include "BossHPDamage.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "GameObject_Manager.h"
#include "Time_Manager.h"
#include "Math_Manager.h"
#include "BossHPBar.h"

CBossHPDamage::CBossHPDamage()
{
}


CBossHPDamage::~CBossHPDamage()
{
}

HRESULT CBossHPDamage::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f ,1.f, 0.f };
	m_pObjectKey = L"BOSS_HP_DAMAGE";
	return S_OK;
}

int CBossHPDamage::Update_GameObject()
{

	return OBJ_NOEVENT;
}

void CBossHPDamage::Late_Update_GameObject()
{
}

void CBossHPDamage::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

	if (nullptr == pTexInfo)
		return;

	m_fFillAmount = static_cast<CBossHPBar*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::BOSS_UI))->Get_BackFillAmount();

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

void CBossHPDamage::Release_GameObject()
{
}

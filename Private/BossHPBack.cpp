#include "stdafx.h"
#include "BossHPBack.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"


CBossHPBack::CBossHPBack()
{
}


CBossHPBack::~CBossHPBack()
{
}

HRESULT CBossHPBack::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"BOSS_HP_BACK_2";
	return S_OK;
}

int CBossHPBack::Update_GameObject()
{

	return OBJ_NOEVENT;
}

void CBossHPBack::Late_Update_GameObject()
{
}

void CBossHPBack::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;


	D3DXMATRIX matTrans, matScale, matWorld;


	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMATRIX FontmatScale, FontmatTrans, FontmatWorld;

	swprintf_s(m_szbuff, L"병거니우스");

	D3DXMatrixScaling(&FontmatScale, 0.7f, 0.7f, 0.7f);
	D3DXMatrixTranslation(&FontmatTrans, 370.f, 75.f, 0.f);

	FontmatWorld = FontmatScale * FontmatTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&FontmatWorld);
	CGraphicDevice::Get_Instance()->Get_Font()->DrawTextW(CGraphicDevice::Get_Instance()->Get_Sprite(), m_szbuff, lstrlen(m_szbuff), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CBossHPBack::Release_GameObject()
{
}



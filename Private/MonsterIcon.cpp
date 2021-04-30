#include "stdafx.h"
#include "MonsterIcon.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "GameObject_Manager.h"


CMonsterIcon::CMonsterIcon()
{
}


CMonsterIcon::~CMonsterIcon()
{
}

HRESULT CMonsterIcon::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"Enemy_Icon";

	return S_OK;
}

HRESULT CMonsterIcon::Late_Ready_GameObject()
{	
	return S_OK;
}

int CMonsterIcon::Update_GameObject()
{
	return OBJ_NOEVENT;
}

void CMonsterIcon::Late_Update_GameObject()
{
}

void CMonsterIcon::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

	if (pTexInfo == nullptr)
		return;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMATRIX FontmatScale, FontmatTrans, FontmatWorld;

	TCHAR szbuff[64], szTemp[64];

	swprintf_s(szbuff, L"%d", m_iMonsterCnt);

	D3DXMatrixScaling(&FontmatScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&FontmatTrans, m_tInfo.vPos.x + 15, m_tInfo.vPos.y, 0.f);

	FontmatWorld = FontmatScale * FontmatTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&FontmatWorld);
	CGraphicDevice::Get_Instance()->Get_Font()->DrawTextW(CGraphicDevice::Get_Instance()->Get_Sprite(), szbuff, lstrlen(szbuff), nullptr, 0, D3DCOLOR_ARGB(255, 255, 0, 0));
}

void CMonsterIcon::Release_GameObject()
{
}

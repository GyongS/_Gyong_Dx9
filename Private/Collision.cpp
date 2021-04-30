#include "stdafx.h"
#include "Collision.h"
#include "Texture_Manager_Client.h"
#include "GraphicDevice.h"
#include "Time_Manager.h"
#include "Scroll_Manager.h"

CCollision::CCollision() : m_fLifeTime(0.f)
{
}


CCollision::~CCollision()
{
}

HRESULT CCollision::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };	
	m_pObjectKey = L"Object_Col";
	return S_OK;
}

HRESULT CCollision::Late_Ready_GameObject()
{
	return E_NOTIMPL;
}

int CCollision::Update_GameObject()
{
	m_fLifeTime += CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fLifeTime >= 0.01f || m_bDead)
	{
		return OBJ_DEAD;
	}

	return OBJ_NOEVENT;
}

void CCollision::Late_Update_GameObject()
{
}

void CCollision::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

	if (nullptr == pTexInfo)
		return;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXMatrixScaling(&matScale, (m_tInfo.iColCX / 32.f) * m_tInfo.vSize.x, (m_tInfo.iColCY / 32.f) * m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X), m_tInfo.vPos.y + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y), 0.f);

	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CCollision::Release_GameObject()
{
}

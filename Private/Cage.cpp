#include "stdafx.h"
#include "Cage.h"
#include "Texture_Manager_Client.h"
#include "GraphicDevice.h"
#include "Scroll_Manager.h"
#include "Player.h"
#include "GameObject_Manager.h"
#include "Key_Manager.h"

CCage::CCage() : m_bDestroy(false)
{
}


CCage::~CCage()
{
}

HRESULT CCage::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"Cage_Front";
	m_tInfo.iColCX = 200;
	m_tInfo.iColCY = 200;
	m_tStat.fMaxHP = 5;
	m_tStat.fCurHP = m_tStat.fMaxHP;
	m_eOBJID = OBJ_ID::CAGE;
	return S_OK;
}

int CCage::Update_GameObject()
{
	if (m_tStat.fCurHP <= 0)
	{
		m_pObjectKey = L"Cage_Front_Destroyed";
		m_tInfo.vPos.y = 445;
		m_bDestroy = true;
	}
	

	return OBJ_NOEVENT;
}

void CCage::Late_Update_GameObject()
{
}

void CCage::Render_GameObject()
{
	const TEXINFO* pTexInfo = nullptr;

	pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

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

void CCage::Release_GameObject()
{
}

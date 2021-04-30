#include "stdafx.h"
#include "Grenade.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Scroll_Manager.h"
#include "Time_Manager.h"
#include "GameObject_Manager.h"
#include "Key_Manager.h"
#include "Grenade_Effect.h"

CGrenade::CGrenade() : m_fGravity(0.f), m_fTime(0.f)
{
}


CGrenade::~CGrenade()
{
}

HRESULT CGrenade::Ready_GameObject()
{
	m_pObjectKey = L"GRENADE";
	m_tInfo.vSize = { 1.f, 1.f, 0.f };	
	m_tInfo.iColCX = 26;
	m_tInfo.iColCY = 26;
	m_fSpeed = 5.f;
	m_fRotation = 45.f;		
	m_fGravity = 9.f;
	m_eOBJID = OBJ_ID::GRENADE;
	return S_OK;
}

HRESULT CGrenade::Late_Ready_GameObject()
{
	m_bLeftRight = CGameObject_Manager::Get_Instance()->Get_Player()->Get_bLeftRight();
	return S_OK;
}

int CGrenade::Update_GameObject()
{
	CGameObject::Late_Ready_GameObject();
	if (m_bDead)
	{
		CGameObject* pObj = CAbstractFactory<CGrenade_Effect>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::GRENADE_EFFECT, pObj);
		return OBJ_DEAD;
	}

	m_fTime += 3.f * CTime_Manager::Get_Instance()->Get_DeltaTime();

	if (m_bLeftRight)
	{
		m_tInfo.vPos.x -= m_fSpeed * cosf(m_fRotation * PI / 180.f) * m_fTime;
		m_tInfo.vPos.y -= m_fSpeed * sinf(m_fRotation * PI / 180.f) * m_fTime - 0.5f * m_fGravity * m_fTime * m_fTime;
	}
	else
	{
		m_tInfo.vPos.x += m_fSpeed * cosf(m_fRotation * PI / 180.f) * m_fTime;
		m_tInfo.vPos.y -= m_fSpeed * sinf(m_fRotation * PI / 180.f) * m_fTime - 0.5f * m_fGravity * m_fTime * m_fTime;
	}

	return OBJ_NOEVENT;
}

void CGrenade::Late_Update_GameObject()
{
}

void CGrenade::Render_GameObject()
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

void CGrenade::Release_GameObject()
{
}


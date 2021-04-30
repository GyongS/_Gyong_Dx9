#include "stdafx.h"
#include "IceParticle.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Scroll_Manager.h"
#include "GameObject_Manager.h"
#include "Time_Manager.h"
#include "SoundManager.h"


CIceParticle::CIceParticle() : m_fGravity(0.f), m_fTime(0.f)
{
}


CIceParticle::~CIceParticle()
{
}

HRESULT CIceParticle::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"Particle";
	m_pStateKey = L"Ice_Particle";
	m_fSpeed = 5.f;
	m_fGravity = 10.f;

	CSoundManager::Get_Instance()->StopSound(CSoundManager::ICE_PARTICLE);
	CSoundManager::Get_Instance()->PlaySound(L"Hit_Ice_Small.wav", CSoundManager::ICE_PARTICLE);
	return S_OK;
}

HRESULT CIceParticle::Late_Ready_GameObject()
{
	m_bLeftRight = CGameObject_Manager::Get_Instance()->Get_Player()->Get_bLeftRight();
	return S_OK;
}

int CIceParticle::Update_GameObject()
{
	m_fLifeTime += CTime_Manager::Get_Instance()->Get_DeltaTime();

	if (m_fLifeTime > 1.f)
		return OBJ_DEAD;


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

void CIceParticle::Late_Update_GameObject()
{
}

void CIceParticle::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey, m_pStateKey, m_iAnimNum);

	if (pTexInfo == nullptr)
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

void CIceParticle::Release_GameObject()
{
}

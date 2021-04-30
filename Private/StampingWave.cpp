#include "stdafx.h"
#include "StampingWave.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Scroll_Manager.h"
#include "GameObject_Manager.h"
#include "Time_Manager.h"

CStampingWave::CStampingWave() : m_fLifeTime(0.f)
{
}


CStampingWave::~CStampingWave()
{
}

HRESULT CStampingWave::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"EFFECT";
	m_pStateKey = L"BOSS_STAMPING_WAVE";
	m_tFrame.fStartFrame = 0.f;
	m_tFrame.fEndFrame = 16.f;	
	m_tInfo.iColCX = 200;
	m_tInfo.iColCY = 100;
	m_eOBJID = OBJ_ID::STAMPING_WAVE;
	return S_OK;
}

HRESULT CStampingWave::Late_Ready_GameObject()
{
	m_bLeftRight = CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::BOSS)->Get_bLeftRight();
	return S_OK;
}

int CStampingWave::Update_GameObject()
{
	CGameObject::Late_Ready_GameObject();
	m_fLifeTime += CTime_Manager::Get_Instance()->Get_DeltaTime();

	if (m_fLifeTime >= 3.f)
		return OBJ_DEAD;
	if (m_bLeftRight)
	{
		m_tInfo.vPos.x -= 600.f * CTime_Manager::Get_Instance()->Get_DeltaTime();
	}
	else
	{
		m_tInfo.vPos.x += 600.f * CTime_Manager::Get_Instance()->Get_DeltaTime();
	}

	return OBJ_NOEVENT;
}

void CStampingWave::Late_Update_GameObject()
{
}

void CStampingWave::Render_GameObject()
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

void CStampingWave::Release_GameObject()
{
}

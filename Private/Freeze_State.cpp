#include "stdafx.h"
#include "Freeze_State.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Scroll_Manager.h"
#include "Time_Manager.h"
#include "GameObject_Manager.h"
#include "IceParticle.h"

CFreeze_State::CFreeze_State() : m_fLifeTime(0.f)
{
}


CFreeze_State::~CFreeze_State()
{
}

HRESULT CFreeze_State::Ready_GameObject()
{	
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	return S_OK;
}

int CFreeze_State::Update_GameObject()
{
	m_fLifeTime += CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_fLifeTime > 2.5f || m_bDead)
	{
		int iCnt = 0;
		for (int i = 0; i < 16; ++i)
		{
			if (iCnt > 8)
				iCnt = 0;
			CGameObject* pObj = CAbstractFactory<CIceParticle>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y, (rand() % 50) + 65);
			pObj->Set_AnimNum(iCnt);
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::PARTICLE, pObj);
			++iCnt;
		}
		return OBJ_DEAD;
	}

	return OBJ_NOEVENT;
}

void CFreeze_State::Late_Update_GameObject()
{
	if (m_pTarget == nullptr)
	{
		m_bDead = true;
		return;
	}
}

void CFreeze_State::Render_GameObject()
{
	if (m_pTarget == nullptr)
	{
		return;
	}

	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

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

void CFreeze_State::Release_GameObject()
{
}

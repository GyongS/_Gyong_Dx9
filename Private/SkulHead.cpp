#include "stdafx.h"
#include "SkulHead.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Scroll_Manager.h"
#include "Time_Manager.h"
#include "GameObject_Manager.h"

CSkulHead::CSkulHead() : m_fGravity(0.f), m_fTime(0.f)
{
}


CSkulHead::~CSkulHead()
{
}

HRESULT CSkulHead::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"SkulSkillHead";
	m_tInfo.iColCX = 19;
	m_tInfo.iColCY = 17;
	m_fRotation = 0.f;
	m_fGravity = 7.f;
	m_fSpeed = 10.f;
	m_eOBJID = OBJ_ID::SKUL_HEAD;
	return S_OK;
}

HRESULT CSkulHead::Late_Ready_GameObject()
{
	m_bLeftRight = CGameObject_Manager::Get_Instance()->Get_Player()->Get_bLeftRight();

	return S_OK;
}

int CSkulHead::Update_GameObject()
{
	CGameObject::Late_Ready_GameObject();

	if (m_bDead)
		return OBJ_DEAD;

	m_fTime += 3.f * CTime_Manager::Get_Instance()->Get_DeltaTime();

	if (!m_bGround)
	{
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
	}

	return OBJ_NOEVENT;
}

void CSkulHead::Late_Update_GameObject()
{
	if (m_tInfo.vPos.x <  m_tInfo.vPos.x - 500)
	{
		m_tInfo.vPos.x = 0;
	}
	else if(m_tInfo.vPos.x > m_tInfo.vPos.x + 500)
	{
		m_tInfo.vPos.x = 800;
	}

}

void CSkulHead::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

	if (pTexInfo == nullptr)
		return;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans, matScale, matWorld;
	if (!m_bLeftRight)
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

void CSkulHead::Release_GameObject()
{
}

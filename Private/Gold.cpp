#include "stdafx.h"
#include "Gold.h"
#include "Texture_Manager_Client.h"
#include "GraphicDevice.h"
#include "Scroll_Manager.h"
#include "Player.h"
#include "GameObject_Manager.h"
#include "Time_Manager.h"

CGold::CGold() : m_fGravity(0.f), m_fJumpTime(0.f), m_bGround(false), m_bGet_Gold(false)
, m_bPIckUp(false), m_fTimer(0.f)
{
}


CGold::~CGold()
{
}

HRESULT CGold::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"ITEM";
	m_pStateKey = L"GOLD";
	m_tFrame.fStartFrame = 0.f;
	m_tFrame.fEndFrame = 9.f;
	m_fGravity = 7.f;
	m_fSpeed = 2.f;
	m_tInfo.iColCX = 5;
	m_tInfo.iColCY = 5;
	m_eOBJID = OBJ_ID::GOLD;
	m_bGround = true;

	return S_OK;
}

HRESULT CGold::Late_Ready_GameObject()
{
	m_bLeftRight = CGameObject_Manager::Get_Instance()->Get_Player()->Get_bLeftRight();
	return S_OK;
}

int CGold::Update_GameObject()
{
	CGameObject::Late_Ready_GameObject();

	if (m_bDead)
	{
		if (!m_bGet_Gold)
		{
			m_pStateKey = L"GET_GOLD";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 26.f;
			static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_Gold((rand() % 50) + 235);
			m_bGround = false;
			m_bPIckUp = false;
			m_bGet_Gold = true;
		}

		if (m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
		{
			return OBJ_DEAD;
		}
	}

	if (!m_bGround)
	{
		m_fSpeed = 0.f;
		m_fJumpTime = 0.f;
		m_fTimer += CTime_Manager::Get_Instance()->Get_DeltaTime();

		if (m_fTimer >= 0.7f)
		{
			m_bPIckUp = true;
			m_fTimer = 0.f;
		}
	}

	m_fJumpTime += 3.f * CTime_Manager::Get_Instance()->Get_DeltaTime();
	
	if (m_bGround)
	{
		if (m_bLeftRight)
		{
			m_tInfo.vPos.x -= m_fSpeed * cosf(m_fRotation * PI / 180.f) * m_fJumpTime;
			m_tInfo.vPos.y -= m_fSpeed * sinf(m_fRotation * PI / 180.f) * m_fJumpTime - 0.5f * m_fGravity * m_fJumpTime * m_fJumpTime;
		}
		else if (!m_bLeftRight)
		{
			m_tInfo.vPos.x += m_fSpeed * cosf(m_fRotation * PI / 180.f) * m_fJumpTime;
			m_tInfo.vPos.y -= m_fSpeed * sinf(m_fRotation * PI / 180.f) * m_fJumpTime - 0.5f * m_fGravity * m_fJumpTime * m_fJumpTime;
		}
	}
	

	return OBJ_NOEVENT;
}

void CGold::Late_Update_GameObject()
{
	FrameMove();
}

void CGold::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey, m_pStateKey, int(m_tFrame.fStartFrame));
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

void CGold::Release_GameObject()
{
}
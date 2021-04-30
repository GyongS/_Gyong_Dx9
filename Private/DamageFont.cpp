#include "stdafx.h"
#include "DamageFont.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "GameObject_Manager.h"
#include "Player.h"
#include "Time_Manager.h"
#include "Scroll_Manager.h"

CDamageFont::CDamageFont() :
	m_fGravity(0.f)
	, m_fJumpForce(0.f)
	, m_fJumpTime(0.f)
	, m_fJumpY(0.f)
	, m_fLifeTime(0.f)
	, m_iDamage(0)
{
}


CDamageFont::~CDamageFont()
{
}

HRESULT CDamageFont::Ready_GameObject()
{
	m_pObjectKey = L"";
	m_fJumpForce = 45.f;
	m_fGravity = 7.5f;
	m_eOBJID = OBJ_ID::DAMAGEFONT;
	return S_OK;
}

HRESULT CDamageFont::Late_Ready_GameObject()
{
	m_fJumpY = m_tInfo.vPos.y;
	return S_OK;
}

int CDamageFont::Update_GameObject()
{
	CGameObject::Late_Ready_GameObject();

	m_fLifeTime += CTime_Manager::Get_Instance()->Get_DeltaTime();

	if (m_fLifeTime >= 0.5f)
		return OBJ_DEAD;
	
	m_tInfo.vPos.y = m_fJumpY - (m_fJumpForce * m_fJumpTime - m_fGravity * m_fJumpTime * m_fJumpTime * 0.5f);
	m_fJumpTime += 0.15f;
	return OBJ_NOEVENT;
}

void CDamageFont::Late_Update_GameObject()
{
}

void CDamageFont::Render_GameObject()
{
	Render_Damage(m_iDamage);
}

void CDamageFont::Render_Damage(int InDamage)
{
	TEXINFO* pTextrue = nullptr;

	D3DXMATRIX FontmatScale, FontmatTrans, FontmatWorld;
	
	TCHAR szbuff[64];

	swprintf_s(szbuff, L"%d", InDamage);

	D3DXMatrixScaling(&FontmatScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&FontmatTrans, m_tInfo.vPos.x+ CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X), m_tInfo.vPos.y + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y), 0.f);

	FontmatWorld = FontmatScale * FontmatTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&FontmatWorld);
	CGraphicDevice::Get_Instance()->Get_Font()->DrawTextW(CGraphicDevice::Get_Instance()->Get_Sprite(), szbuff, lstrlen(szbuff), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
}

CDamageFont * CDamageFont::Create()
{
	CDamageFont* pInstance = new CDamageFont;

	return nullptr;
}

void CDamageFont::Release_GameObject()
{
}


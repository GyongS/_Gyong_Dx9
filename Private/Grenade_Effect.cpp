#include "stdafx.h"
#include "Grenade_Effect.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Scroll_Manager.h"
#include "GameObject_Manager.h"
#include "Collision.h"
#include "SoundManager.h"

CGrenade_Effect::CGrenade_Effect()
{
}


CGrenade_Effect::~CGrenade_Effect()
{
}

HRESULT CGrenade_Effect::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"HIT_EFFECT";
	m_pStateKey = L"PowerfulGrenade";
	m_tFrame.fStartFrame = 0.f;
	m_tFrame.fEndFrame = 11.f;
	m_eOBJID = OBJ_ID::GRENADE_EFFECT;
	
	CSoundManager::Get_Instance()->StopSound(CSoundManager::GRENADE_BOMB);
	CSoundManager::Get_Instance()->PlaySound(L"Grenade_Bomb.wav", CSoundManager::GRENADE_BOMB);
	return S_OK;
}

HRESULT CGrenade_Effect::Late_Ready_GameObject()
{
	CGameObject* pObj = CAbstractFactory<CCollision>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
	pObj->Set_ColScale(180, 180);
	pObj->Set_OBJID(OBJ_ID::GRENADE_COL);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::GRENADE_COL, pObj);
	return S_OK;
}

int CGrenade_Effect::Update_GameObject()
{
	CGameObject::Late_Ready_GameObject();

	if (m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
		return OBJ_DEAD;

	if (m_tFrame.fStartFrame <= 3.f)
	{
		CScroll_Manager::Set_bScrollShake(true);
	}
	if (m_tFrame.fStartFrame > 3.f)
	{
		CScroll_Manager::Set_bScrollShake(false);
	}

	return OBJ_NOEVENT;
}

void CGrenade_Effect::Late_Update_GameObject()
{
	FrameMove();
}

void CGrenade_Effect::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey, m_pStateKey, (int)m_tFrame.fStartFrame);

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

void CGrenade_Effect::Release_GameObject()
{
}

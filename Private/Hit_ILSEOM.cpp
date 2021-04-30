#include "stdafx.h"
#include "Hit_ILSEOM.h"
#include "GraphicDevice.h"
#include "Scroll_Manager.h"
#include "Texture_Manager_Client.h"
#include "Player.h"
#include "GameObject_Manager.h"
#include "Warrior.h"
#include "Critical_Effect.h"
#include "Collision.h"
#include "SoundManager.h"

CHit_ILSEOM::CHit_ILSEOM()
{
}


CHit_ILSEOM::~CHit_ILSEOM()
{
}

HRESULT CHit_ILSEOM::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"HIT_ILSEOM";
	m_pStateKey = L"ILSEOM";
	m_tFrame.fStartFrame = 0.f;
	m_tFrame.fEndFrame = 8.f;
	
	return S_OK;
}

HRESULT CHit_ILSEOM::Late_Ready_GameObject()
{
	m_bLeftRight = CGameObject_Manager::Get_Instance()->Get_Player()->Get_bLeftRight();
	return S_OK;
}

int CHit_ILSEOM::Update_GameObject()
{
	CGameObject::Late_Ready_GameObject();

	if (m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
	{
		CGameObject* pObj = CAbstractFactory<CCritical_Effect>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);

		pObj = CAbstractFactory<CCollision>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
		pObj->Set_ColScale(1, 1);
		pObj->Set_OBJID(OBJ_ID::ILSEOM_HIT);		
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::ILSEOM_HIT, pObj);
		CSoundManager::Get_Instance()->StopSound(CSoundManager::SAMURAI_ATTACK_0_SFX);
		CSoundManager::Get_Instance()->PlaySound(L"Samurai_IlSeomHit.wav", CSoundManager::SAMURAI_ILSEOM_END);
		static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_HitILSeom(false);		
		return OBJ_DEAD;
	}


	return OBJ_NOEVENT;
}

void CHit_ILSEOM::Late_Update_GameObject()
{
	FrameMove();
}

void CHit_ILSEOM::Render_GameObject()
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

void CHit_ILSEOM::Release_GameObject()
{
}

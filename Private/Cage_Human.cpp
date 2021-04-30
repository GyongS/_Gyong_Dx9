#include "stdafx.h"
#include "Cage_Human.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Scroll_Manager.h"
#include "GameObject_Manager.h"
#include "Cage.h"
#include "EventManager.h"
#include "SamuraiHead.h"
#include "Player.h"

CCage_Human::CCage_Human() : m_bDropItem(false)
{
}


CCage_Human::~CCage_Human()
{
}

HRESULT CCage_Human::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"Idle_Cage_0";
	m_pStateKey = L"Cage_IDLE";
	m_tFrame.fStartFrame = 0.f;
	m_tFrame.fEndFrame = 6.f;
	m_fFrameSpeed = 0.f;
	return S_OK;
}

int CCage_Human::Update_GameObject()
{
	if (static_cast<CCage*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::CAGE))->Get_DestroyCheck())
	{
		m_pObjectKey = L"Cage";
		m_fFrameSpeed = 1.f;
		if (CEventManager::Get_Instance()->Get_EventCnt() < 4 &&
			CEventManager::Get_Instance()->Get_DialogueCount() >= 7)
		{
			m_bDropItem = true;
		}
		else if (m_bDropItem)
		{
			CGameObject* pObj = CAbstractFactory<CSamuraiHead>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::SAMURAI_HEAD, pObj);
			static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_DropItem(true);
			CEventManager::Get_Instance()->Set_DialogueCount(0);
			m_bDropItem = false;
		}
	}
	
	return OBJ_NOEVENT;
}

void CCage_Human::Late_Update_GameObject()
{
	FrameMove(m_fFrameSpeed);
}

void CCage_Human::Render_GameObject()
{
	const TEXINFO* pTexInfo = nullptr;

	if (m_pObjectKey == L"Cage")
	{
		pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey, m_pStateKey, (int)m_tFrame.fStartFrame);
	}
	if (m_pObjectKey == L"Idle_Cage_0")
	{
		pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);
	}

	if (nullptr == pTexInfo)
		return;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans, matScale, matWorld;
	D3DXMatrixScaling(&matScale, -m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X), m_tInfo.vPos.y + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y), 0.f);

	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CCage_Human::Release_GameObject()
{
}



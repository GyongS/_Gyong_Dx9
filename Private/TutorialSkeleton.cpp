#include "stdafx.h"
#include "TutorialSkeleton.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Scroll_Manager.h"
#include "EventManager.h"
#include "GameObject_Manager.h"
#include "Player.h"

CTutorialSkeleton::CTutorialSkeleton(): m_eCurState(END), m_ePreState(END)
{
}


CTutorialSkeleton::~CTutorialSkeleton()
{
}

HRESULT CTutorialSkeleton::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"SKELETON";
	m_pStateKey = L"SKELETON_GIVE";
	m_eCurState = GIVE;
	m_tFrame.fStartFrame = 0.f;
	m_tFrame.fEndFrame = 6.f;
	m_fFrameSpeed = 0.f;
	return S_OK;
}

int CTutorialSkeleton::Update_GameObject()
{
	if (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_bGetBone()
		&& CEventManager::Get_Instance()->Get_DialogueCount() >= 7)
	{
		m_pObjectKey = L"Noweapon_0";
	}

	else if (CEventManager::Get_Instance()->Get_EventCnt() >= 2 &&
		CEventManager::Get_Instance()->Get_DialogueCount() >= 6 &&
		!static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_bGetBone())
	{
		m_fFrameSpeed = 0.6f;
		if (m_tFrame.fStartFrame >= 4.f)
		{
			m_fFrameSpeed = 0.f;
		}
	}	

	if (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_bGetBone()
		&& !CEventManager::Get_Instance()->Get_bEvent())
	{
		m_pObjectKey = L"SKELETON";
		m_eCurState = DEAD;
		m_fFrameSpeed = 0.8f;
		if (m_eCurState == DEAD && m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
		{
			m_fFrameSpeed = 0.f;
		}
	}
	
	Change_State();
	return OBJ_NOEVENT;
}

void CTutorialSkeleton::Late_Update_GameObject()
{
	FrameMove(m_fFrameSpeed);
}

void CTutorialSkeleton::Render_GameObject()
{
	const TEXINFO* pTexInfo = nullptr;

	if (m_pObjectKey == L"SKELETON")
	{
		pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey, m_pStateKey, (int)m_tFrame.fStartFrame);
	}
	if (m_pObjectKey == L"Noweapon_0")
	{
		pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);
	}

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

void CTutorialSkeleton::Release_GameObject()
{
}

void CTutorialSkeleton::Change_State()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CTutorialSkeleton::GIVE:
			m_pStateKey = L"SKELETON_GIVE";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 6.f;
			break;
		case CTutorialSkeleton::DEAD:
			m_pStateKey = L"SKELETON_DEAD";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 10.f;
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

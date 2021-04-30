#include "stdafx.h"
#include "Portal.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Scroll_Manager.h"
#include "GameObject_Manager.h"
#include "Scene_Manager.h"
#include "Player.h"

CPortal::CPortal()
{
}


CPortal::~CPortal()
{
}

HRESULT CPortal::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"Resources";
	m_pStateKey = L"DOOR";
	m_tFrame.fStartFrame = 0.f;
	m_tFrame.fEndFrame = 8.f;
	m_fFrameSpeed = 1.f;
	m_tInfo.iColCX = 50;
	m_tInfo.iColCY = 150;
	m_eOBJID = OBJ_ID::PORTAL;
	return S_OK;
}

int CPortal::Update_GameObject()
{
	switch (CScene_Manager::Get_Instance()->Get_Scene())
	{
		case CScene_Manager::SCENE_STAGE1:
			break;
		case CScene_Manager::SCENE_STAGE2:
			break;
		case CScene_Manager::SCENE_STAGE3:
			m_tFrame.fStartFrame = 9.f;
			m_tFrame.fEndFrame = 9.f;
			m_fFrameSpeed = 0.f;
			break;
		case CScene_Manager::SCENE_STAGE4:
			break;
		case CScene_Manager::SCENE_STAGE5:
			break;
	default:
		break;
	}
	return OBJ_NOEVENT;
}

void CPortal::Late_Update_GameObject()
{
	FrameMove(m_fFrameSpeed);
}

void CPortal::Render_GameObject()
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

	if (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_InPortal())
	{
		pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(L"InPortalPopup");

		D3DXMATRIX matTrans, matScale, matWorld;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, (m_tInfo.vPos.x + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X)) + 60, (m_tInfo.vPos.y + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y)) + 130, 0.f);

		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CPortal::Release_GameObject()
{
}

#include "stdafx.h"
#include "PrisonerHead.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Scroll_Manager.h"
#include "Player.h"
#include "GameObject_Manager.h"
#include "SamuraiHead.h"
#include "Default_Head.h"

CPrisonerHead::CPrisonerHead() : m_bJump(false), m_fGravity(0.f), m_fJumpForce(0.f), m_fJumpTime(0.f), m_fJumpY(0.f)
{
}


CPrisonerHead::~CPrisonerHead()
{
}

HRESULT CPrisonerHead::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.iColCX = 10;
	m_tInfo.iColCY = 10;
	m_fJumpForce = 40.f;
	m_fGravity = 9.8f;
	m_eOBJID = OBJ_ID::PRISONER_HEAD;
	m_pObjectKey = L"Prisoner_Head";
	return S_OK;
}

HRESULT CPrisonerHead::Late_Ready_GameObject()
{
	m_bJump = static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_DropItem();
	m_fJumpY = m_tInfo.vPos.y;
	return S_OK;
}

int CPrisonerHead::Update_GameObject()
{
	CGameObject::Late_Ready_GameObject();

	if (m_bDead)
	{
		if (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_HeadCnt() < 1)
		{
			static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_HeadCnt(1);

			switch (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Type())
			{
			case CPlayer::TYPE_DEFAULT:
				static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_Type(CPlayer::TYPE_PRISONER);
				static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_SaveType(CPlayer::TYPE_DEFAULT);
				static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_DropItem(true);
				break;
			case CPlayer::TYPE_PRISONER:
				break;
			case CPlayer::TYPE_SAMURAI:
				static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_Type(CPlayer::TYPE_PRISONER);
				static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_SaveType(CPlayer::TYPE_SAMURAI);
				static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_DropItem(true);
				break;
			default:
				break;
			}
			return OBJ_DEAD;
		}

		CGameObject* pObj = nullptr;
		
		switch (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_SaveType())
		{
		case CPlayer::TYPE_DEFAULT:
			pObj = CAbstractFactory<CDefault_Head>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::DEFAULT_HEAD, pObj);
			break;
		case CPlayer::TYPE_PRISONER:
			break;
		case CPlayer::TYPE_SAMURAI:
			pObj = CAbstractFactory<CSamuraiHead>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::SAMURAI_HEAD, pObj);
			static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_SaveType(CPlayer::TYPE_SAMURAI);
			break;
		default:
			break;
		}

		switch (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Type())
		{
		case CPlayer::TYPE_DEFAULT:
			static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_Type(CPlayer::TYPE_PRISONER);
			static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_SaveType(CPlayer::TYPE_DEFAULT);
			static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_DropItem(true);
			break;
		case CPlayer::TYPE_PRISONER:
			break;
		case CPlayer::TYPE_SAMURAI:
			static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_Type(CPlayer::TYPE_PRISONER);
			static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_SaveType(CPlayer::TYPE_SAMURAI);
			static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_DropItem(true);
			break;
		default:
			break;
		}

		return OBJ_DEAD;
	}

	if (m_bJump)
	{
		m_tInfo.vPos.y = m_fJumpY - (m_fJumpForce * m_fJumpTime - m_fGravity * m_fJumpTime * m_fJumpTime * 0.5f);
		m_fJumpTime += 0.15f;
	}

	return OBJ_NOEVENT;
}

void CPrisonerHead::Late_Update_GameObject()
{
}

void CPrisonerHead::Render_GameObject()
{
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

void CPrisonerHead::Release_GameObject()
{
}

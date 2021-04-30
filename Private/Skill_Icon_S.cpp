#include "stdafx.h"
#include "Skill_Icon_S.h"
#include "Texture_Manager_Client.h"
#include "GraphicDevice.h"
#include "Player.h"
#include "GameObject_Manager.h"

CSkill_Icon_S::CSkill_Icon_S()
{
}


CSkill_Icon_S::~CSkill_Icon_S()
{
}

HRESULT CSkill_Icon_S::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"";

	return S_OK;
}

HRESULT CSkill_Icon_S::Late_Ready_GameObject()
{
	return E_NOTIMPL;
}

int CSkill_Icon_S::Update_GameObject()
{
	Change_Skill_Icon_S();
	return OBJ_NOEVENT;
}

void CSkill_Icon_S::Late_Update_GameObject()
{
}

void CSkill_Icon_S::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

	if (nullptr == pTexInfo)
		return;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CSkill_Icon_S::Release_GameObject()
{
}

void CSkill_Icon_S::Change_Skill_Icon_S()
{
	switch (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Type())
	{
	case CPlayer::TYPE_DEFAULT:
		m_pObjectKey = L"";
		break;
	case CPlayer::TYPE_PRISONER:
		m_pObjectKey = L"Skill_Broadsword";
		break;
	case CPlayer::TYPE_SAMURAI:
		m_pObjectKey = L"";
		break;
	default:
		break;
	}
}

#include "stdafx.h"
#include "SkillCoolDown.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "GameObject_Manager.h"
#include "CoolDownEffect.h"
#include "Player.h"

CSkillCoolDown::CSkillCoolDown() :
	m_fSkill_ACoolDown(0.f),
	m_fSkill_SCoolDown(0.f),
	m_fSkill_DCoolDown(0.f),
	m_eCurSkill_ID(END)
{
}


CSkillCoolDown::~CSkillCoolDown()
{
}

HRESULT CSkillCoolDown::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"CoolTime";
	return S_OK;
}

HRESULT CSkillCoolDown::Late_Ready_GameObject()
{
	switch (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_SkillCnt())
	{
	case 0:
		m_eCurSkill_ID = SKILL_A;
		break;
	case 1:
		m_eCurSkill_ID = SKILL_S;
		break;
	case 2:
		m_eCurSkill_ID = SKILL_D;
		break;
	}
	return S_OK;
}

int CSkillCoolDown::Update_GameObject()
{
	CGameObject::Late_Ready_GameObject();

	if (m_bDead)
	{
		CGameObject* pObj = nullptr;

		switch (m_eCurSkill_ID)
		{
		case CSkillCoolDown::SKILL_A:
			pObj = CAbstractFactory<CCoolDownEffect>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);
			break;
		case CSkillCoolDown::SKILL_S:
			pObj = CAbstractFactory<CCoolDownEffect>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);
			break;
		case CSkillCoolDown::SKILL_D:
			pObj = CAbstractFactory<CCoolDownEffect>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);
			break;
		default:
			break;
		}
		return OBJ_DEAD;
	}

	m_fSkill_ACoolDown = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Stat().fSKill_A_Cool / CGameObject_Manager::Get_Instance()->Get_Player()->Get_Stat().fSKill_A_MaxCool;
	m_fSkill_SCoolDown = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Stat().fSKill_S_Cool / CGameObject_Manager::Get_Instance()->Get_Player()->Get_Stat().fSKill_S_MaxCool;
	m_fSkill_DCoolDown = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Stat().fSKill_D_Cool / CGameObject_Manager::Get_Instance()->Get_Player()->Get_Stat().fSKill_D_MaxCool;


	return OBJ_NOEVENT;
}

void CSkillCoolDown::Late_Update_GameObject()
{
	switch (m_eCurSkill_ID)
	{
	case CSkillCoolDown::SKILL_A:
		if (m_fSkill_ACoolDown <= 0)
			m_bDead = true;
		break;
	case CSkillCoolDown::SKILL_S:
		if (m_fSkill_SCoolDown <= 0)
			m_bDead = true;
		break;
	case CSkillCoolDown::SKILL_D:
		if (m_fSkill_DCoolDown <= 0)
			m_bDead = true;
		break;
	default:
		break;
	}
}

void CSkillCoolDown::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

	if (pTexInfo == nullptr)
		return;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, -m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	RECT rcTemp = {};

	switch (m_eCurSkill_ID)
	{
	case CSkillCoolDown::SKILL_A:
		rcTemp = { 0, 0 ,(LONG)pTexInfo->tImageInfo.Width, (LONG)(pTexInfo->tImageInfo.Height * m_fSkill_ACoolDown) };
		break;
	case CSkillCoolDown::SKILL_S:
		rcTemp = { 0, 0,(LONG)pTexInfo->tImageInfo.Width, (LONG)(pTexInfo->tImageInfo.Height * m_fSkill_SCoolDown) };
		break;
	case CSkillCoolDown::SKILL_D:
		rcTemp = { 0, 0,(LONG)pTexInfo->tImageInfo.Width, (LONG)(pTexInfo->tImageInfo.Height * m_fSkill_DCoolDown) };
		break;
	default:
		break;
	}

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rcTemp, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(150, 255, 255, 255));
}

void CSkillCoolDown::Release_GameObject()
{
}

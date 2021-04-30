#include "stdafx.h"
#include "GiantWood.h"
#include "GraphicDevice.h"
#include "Scroll_Manager.h"
#include "Texture_Manager_Client.h"
#include "Time_Manager.h"
#include "GiantWoodBall.h"
#include "GameObject_Manager.h"
#include "GiantWoodAttack_Effect.h"
#include "MonsterBackHP.h"
#include "MonsterServeHPbar.h"
#include "MonsterHPbar.h"
#include "Gold.h"
#include "Player.h"
#include "MinimapMonster.h"
#include "Freeze_State.h"
#include "MonsterDeadEffect.h"
#include "SoundManager.h"
#include "MonsterIcon.h"

CGiantWood::CGiantWood() : 
	m_eCurState(END),
	m_ePreState(END),
	m_fDelay(0.f),
	m_bAttack(false),
	m_bHitIceAllow(false),
	m_bFreezeState(false),
	m_bColorBlack(false),
	m_bHitBroadSword(false),
	m_bHitGrenade(false),
	m_pMainHp(nullptr),
	m_pServeHP(nullptr),
	m_pHPBack(nullptr),
	m_pMiniMapObj(nullptr),
	m_pFreezeState(nullptr)

{
}


CGiantWood::~CGiantWood()
{
}

HRESULT CGiantWood::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.iColCX = 115;
	m_tInfo.iColCY = 112;
	m_pObjectKey = L"WOODMONSTER";
	m_pStateKey = L"WOODMONSTER_IDLE";
	m_eCurState = IDLE;
	m_fRotation = 0.f;
	m_fFrameSpeed = 1.f;
	m_tStat.fMaxHP = 100.f;
	m_tStat.fCurHP = m_tStat.fMaxHP;
	m_eOBJID = OBJ_ID::GIANTWOOD;
	m_eOBJ_TYPE = OBJ_TYPE::GIANTWOOD;
	static_cast<CMonsterIcon*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::MONSTER_ICON))->Set_MonsterCnt(1);
	return S_OK;
}

HRESULT CGiantWood::Late_Ready_GameObject()
{
	m_pHPBack = CAbstractFactory<CMonsterBackHP>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y + 100);
	m_pHPBack->Set_Target(this);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MONSTER_UI, m_pHPBack);

	m_pServeHP = CAbstractFactory<CMonsterServeHPbar>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y + 100);
	m_pServeHP->Set_Target(this);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MONSTER_UI, m_pServeHP);

	m_pMainHp = CAbstractFactory<CMonsterHPbar>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y + 100);
	m_pMainHp->Set_Target(this);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MONSTER_UI, m_pMainHp);

	m_pMiniMapObj = CAbstractFactory<CMinimapMonster>::Create();
	m_pMiniMapObj->Set_Target(this);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MINIMAP_MONSTER, m_pMiniMapObj);
	return S_OK;
}

int CGiantWood::Update_GameObject()
{
	CGameObject::Late_Ready_GameObject();	

	if (m_tStat.fCurHP <= 0)
	{
		if (m_pMainHp != nullptr)
			m_pMainHp->Set_Target(nullptr);

		if (m_pServeHP != nullptr)
			m_pServeHP->Set_Target(nullptr);

		if (m_pHPBack != nullptr)
			m_pHPBack->Set_Target(nullptr);

		if (m_pMiniMapObj != nullptr)
			m_pMiniMapObj->Set_Target(nullptr);

		if (m_pFreezeState != nullptr)
			m_pFreezeState->Set_Target(nullptr);

		CGameObject* pObj = CAbstractFactory<CMonsterDeadEffect>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);

		int irand = (rand() % 5) + 1;
		float fRot = 60.f;
		for (size_t i = 0; i < irand; i++)
		{
			pObj = CAbstractFactory<CGold>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y, fRot);
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::GOLD, pObj);
			fRot += 15.f;
		}

		CSoundManager::Get_Instance()->StopSound(CSoundManager::BIG_MONSTER_DEAD);
		CSoundManager::Get_Instance()->PlaySound(L"Enemy_Big_Dead.wav", CSoundManager::BIG_MONSTER_DEAD);
		static_cast<CMonsterIcon*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::MONSTER_ICON))->Set_MonsterCnt(-1);

		return OBJ_DEAD;
	}

	FREEZE_STATE();
	HIT_ILSEOM_STATE();
	HIT_BROAD_SWORD();
	HIT_GRENADE();

	if (!m_bColorBlack && !m_bFreezeState)
	{
		m_fFrameSpeed = 0.7f;

		Change_State();
		Monster_Pattern();
	}

	return OBJ_NOEVENT;
}

void CGiantWood::Late_Update_GameObject()
{
	
	FrameMove(m_fFrameSpeed);
}

void CGiantWood::Render_GameObject()
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
	if (m_bColorBlack)
	{
		CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 0, 0, 0));
	}
	else
	{
		CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CGiantWood::Release_GameObject()
{
}

void CGiantWood::Set_Damage(int _InDamage)
{
	m_tStat.fCurHP -= _InDamage;
}

void CGiantWood::Change_State()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CGiantWood::IDLE:
			m_pStateKey = L"WOODMONSTER_IDLE";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 5.f;
			m_fFrameSpeed = 1.f;
			break;
		case CGiantWood::ATTACK1:
			m_pStateKey = L"WOODMONSTER_ATTACK1";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 7.f;
			m_fFrameSpeed = 1.f;
			break;
		case CGiantWood::ATTACK2:
			m_pStateKey = L"WOODMONSTER_ATTACK2";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 4.f;
			m_fFrameSpeed = 1.f;
			break;
		case CGiantWood::DEAD:
			m_pStateKey = L"WOODMONSTER_DEAD|";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 1.f;
			m_fFrameSpeed = 0.5f;
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CGiantWood::Monster_Pattern()
{	
	switch (m_eCurState)
	{
	case CGiantWood::IDLE:
		STATE_IDLE();
		break;
	case CGiantWood::ATTACK1:
		STATE_ATTACK1();
		break;
	case CGiantWood::ATTACK2:
		STATE_ATTACK2();
		break;
	}
}

void CGiantWood::STATE_IDLE()
{
	float fX = m_pTarget->Get_Info().vPos.x - m_tInfo.vPos.x;
	float fY = m_pTarget->Get_Info().vPos.y - m_tInfo.vPos.y;
	float fDist = sqrtf(fX * fX + fY * fY);
	m_fDelay += CTime_Manager::Get_Instance()->Get_DeltaTime();

	if (fDist <= 100.f && m_tInfo.vPos.y <= m_pTarget->Get_Info().vPos.y && m_fDelay >= 1.f)
	{
		m_eCurState = ATTACK1;
		m_fDelay = 0.f;
	}
	else
	{
		if (m_fDelay >= 2.f)
		{
			m_eCurState = ATTACK2;
			m_fDelay = 0.f;
		}
	}

}

void CGiantWood::STATE_ATTACK1()
{
	m_eCurState = ATTACK1;

	if (m_tFrame.fStartFrame >= 1.f)
		m_fFrameSpeed = 0.3f;

	if (m_tFrame.fStartFrame >= 4.f && !m_bAttack)
	{
		CGameObject* pObj = CAbstractFactory<CGiantWoodAttack_Effect>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y + 20.f);
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::GIANTWOOD_ATTACK, pObj);
		m_bAttack = true;
	}

	if (m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
	{
		m_eCurState = IDLE;
		m_bAttack = false;
	}

}

void CGiantWood::STATE_ATTACK2()
{
	m_eCurState = ATTACK2;

	if (m_tFrame.fStartFrame >= 1.f && m_tFrame.fStartFrame < 2.f)
	{
		m_fFrameSpeed = 0.3f;
	}

	if (m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 2.f)
	{
		if (!m_bAttack)
		{
			for (int i = 0; i < 8; ++i)
			{
				CGameObject* pObj = CAbstractFactory<CGiantWoodBall>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y, m_fRotation);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::GIANTWOOD_BALL, pObj);
				m_fRotation += 15.f;
			}
			m_fRotation = 0.f;
			m_bAttack = true;
		}
	}
	if (m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
	{
		m_eCurState = IDLE;
		m_bAttack = false;
	}
}

void CGiantWood::HIT_ILSEOM_STATE()
{
	if (!static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_bHit_ILSeom())
	{
		m_bColorBlack = false;
	}

	if (m_bColorBlack)
	{
		m_fFrameSpeed = 0.f;
		m_fDelay = 0.f;
	}
}

void CGiantWood::FREEZE_STATE()
{
	if (CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::ICE_ALLOW) == nullptr)
	{
		m_bHitIceAllow = false;
	}

	if (CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::FREEZE) == nullptr)
	{
		m_bFreezeState = false;
	}

	if (!m_bFreezeState && m_bHitGrenade || !m_bFreezeState && m_bHitIceAllow)
	{
		m_pFreezeState = CAbstractFactory<CFreeze_State>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
		m_pFreezeState->Set_ObjectKey(L"freeze_large");
		m_pFreezeState->Set_Target(this);
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::FREEZE, m_pFreezeState);

		CSoundManager::Get_Instance()->StopSound(CSoundManager::FREEZE_STATE);
		CSoundManager::Get_Instance()->PlaySound(L"State_Freeze.wav", CSoundManager::FREEZE_STATE);
		m_bFreezeState = true;
	}

	if (m_bFreezeState)
	{
		m_fFrameSpeed = 0.f;		
		m_fDelay = 0.f;
	}
}

void CGiantWood::HIT_BROAD_SWORD()
{
	if (CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::BROADSWORD_COL) == nullptr)
	{
		m_bHitBroadSword = false;
	}
}

void CGiantWood::HIT_GRENADE()
{
	if (CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::GRENADE) == nullptr)
	{
		m_bHitGrenade = false;
	}
}

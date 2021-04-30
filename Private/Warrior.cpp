#include "stdafx.h"
#include "Warrior.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "GameObject_Manager.h"
#include "Collision.h"
#include "Time_Manager.h"
#include "Scroll_Manager.h"
#include "MonsterBackHP.h"
#include "MonsterServeHPbar.h"
#include "MonsterHPbar.h"
#include "Gold.h"
#include "Player.h"
#include "IceCrossAllow.h"
#include "MinimapMonster.h"
#include "Freeze_State.h"
#include "MonsterDeadEffect.h"
#include "SoundManager.h"
#include "MonsterIcon.h"

CWarrior::CWarrior() : 
	m_bCheckGround(false),
	m_iDir(0),
	m_fDelay(0.f),
	m_bAnimisPlaying(false),
	m_bCreateCol(false),
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


CWarrior::~CWarrior()
{
}

HRESULT CWarrior::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.iColCX = 34;
	m_tInfo.iColCY = 55;
	m_tStat.fMaxHP = 100.f;
	m_tStat.fCurHP = m_tStat.fMaxHP;
	m_pObjectKey = L"WARRIORMONSTER";
	m_pStateKey = L"WARRIORMONSTER_IDLE";
	m_eCurState = IDLE;
	m_fSpeed = 50.f;
	m_eOBJID = OBJ_ID::MONSTER;
	m_eOBJ_TYPE = OBJ_TYPE::WARRIOR;
	static_cast<CMonsterIcon*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::MONSTER_ICON))->Set_MonsterCnt(1);
	return S_OK;
}

HRESULT CWarrior::Late_Ready_GameObject()
{
	m_pHPBack = CAbstractFactory<CMonsterBackHP>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y + 40);
	m_pHPBack->Set_Target(this);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MONSTER_UI, m_pHPBack);

	m_pServeHP = CAbstractFactory<CMonsterServeHPbar>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y + 40);
	m_pServeHP->Set_Target(this);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MONSTER_UI, m_pServeHP);

	m_pMainHp = CAbstractFactory<CMonsterHPbar>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y + 40);
	m_pMainHp->Set_Target(this);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MONSTER_UI, m_pMainHp);

	m_pMiniMapObj = CAbstractFactory<CMinimapMonster>::Create();
	m_pMiniMapObj->Set_Target(this);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MINIMAP_MONSTER, m_pMiniMapObj);
	return S_OK;
}

int CWarrior::Update_GameObject()
{
	CGameObject::Late_Ready_GameObject();

	if (m_bDead || m_tStat.fCurHP <= 0)
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
		

		CSoundManager::Get_Instance()->StopSound(CSoundManager::MONSTER_DEAD);
		CSoundManager::Get_Instance()->PlaySound(L"Enemy_Dead.wav", CSoundManager::MONSTER_DEAD);

		static_cast<CMonsterIcon*>(CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::MONSTER_ICON))->Set_MonsterCnt(-1);
		return OBJ_DEAD;
	}


	if (!m_bCheckGround)
	{
		m_tInfo.vPos.y += 5.f;
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
		Check_LeftRight();
	}
	return OBJ_NOEVENT;
}

void CWarrior::Late_Update_GameObject()
{
	FrameMove(m_fFrameSpeed);
}

void CWarrior::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey, m_pStateKey, int(m_tFrame.fStartFrame));
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

	if (m_bColorBlack)
	{
		CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 0, 0, 0));
	}
	else
	{
		CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CWarrior::Release_GameObject()
{
}

void CWarrior::Set_Damage(int _InDamage)
{
	m_tStat.fCurHP -= _InDamage;
}

void CWarrior::Change_State()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CWarrior::IDLE:
			m_pStateKey = L"WARRIORMONSTER_IDLE";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 6.f;
			m_fFrameSpeed = 1.f;
			break;
		case CWarrior::WALK:
			m_pStateKey = L"WARRIORMONSTER_WALK";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 7.f;
			m_fFrameSpeed = 1.f;
			break;
		case CWarrior::ATTACK:
			m_pStateKey = L"WARRIORMONSTER_ATTACK";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 5.f;
			m_fFrameSpeed = 1.f;
			break;
		case CWarrior::HIT:
			m_pStateKey = L"WARRIORMONSTER_HIT";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 1.f;
			m_fFrameSpeed = 1.f;
			break;
		case CWarrior::HIT2:
			m_pStateKey = L"WARRIORMONSTER_HIT2";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 1.f;
			m_fFrameSpeed = 1.f;
			break;
		case CWarrior::DEAD:
			m_pStateKey = L"WARRIORMONSTER_DEAD";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 1.f;
			m_fFrameSpeed = 1.f;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CWarrior::Check_LeftRight()
{
	if (!m_bAnimisPlaying)
	{
		if (m_pTarget->Get_Info().vPos.y < m_tInfo.vPos.y)
			return;

		if (m_pTarget->Get_Info().vPos.x < m_tInfo.vPos.x)
		{
			m_bLeftRight = true;
		}
		else if (m_pTarget->Get_Info().vPos.x > m_tInfo.vPos.x)
		{
			m_bLeftRight = false;
		}
	}
}

void CWarrior::Monster_Pattern()
{
	switch (m_eCurState)
	{
	case CWarrior::IDLE:
		IDLE_STATE();
		break;
	case CWarrior::WALK:
		WALK_STATE();
		break;
	case CWarrior::ATTACK:
		ATTACK_STATE();
		break;
	}
}

void CWarrior::IDLE_STATE()
{
	m_fDelay += 1.f;
	m_iDir = (rand() % 2) + 1;

	if (m_fDelay >= 100.f)
	{
		switch (m_iDir)
		{
		case 1:
			m_fDelay = 0.f;
			m_bLeftRight = false;
			break;
		case 2:
			m_fDelay = 0.f;
			m_bLeftRight = true;
			break;
		}
		Set_State(WALK);
	}
}

void CWarrior::WALK_STATE()
{
	m_fDelay += 1.f;
	m_fSpeed = 50.f;


	// 플레이어가 몬스터랑 같은 층에 있을 때 플레이어 추적하게하기.

	float fX = m_pTarget->Get_Info().vPos.x - m_tInfo.vPos.x;
	float fY = m_pTarget->Get_Info().vPos.y - m_tInfo.vPos.y;
	float fDist = sqrtf(fX * fX + fY * fY);

	m_fRotation = acosf(fX / fDist);

	if (m_tInfo.vPos.x < m_pTarget->Get_Info().vPos.x)
		m_fRotation *= -1.f;

	m_tInfo.vPos.x += cosf(m_fRotation) * CTime_Manager::Get_Instance()->Get_DeltaTime() * m_fSpeed;

	if (fDist <= 50.f && m_tInfo.vPos.y <= m_pTarget->Get_Info().vPos.y)
	{
		Set_State(ATTACK);
	}	
	else
	{
		if (m_fDelay >= 150.f)
		{
			Set_State(IDLE);

			m_fDelay = 0.f;
			m_fSpeed = 0.f;
		}
		/*if (m_tInfo.vPos.x <= 0)
		{
		m_tInfo.vPos.x = m_tInfo.vPos.x += 1.f * CTime_Manager::Get_Instance()->Get_DeltaTime() * m_fSpeed;
		}*/
	}
}

void CWarrior::ATTACK_STATE()
{
	m_fSpeed = 0.f;

	m_bAnimisPlaying = true;
	if (m_tFrame.fStartFrame >= 1.f && !m_bCreateCol)
	{
		if (!m_bCreateCol)
		{
			if (m_bLeftRight)
			{
				CGameObject* pObJ = CAbstractFactory<CCollision>::CreateCollision(m_tInfo.vPos.x - 20, m_tInfo.vPos.y, 32, 32);
				pObJ->Set_OBJID(OBJ_ID::MONSTER_ATTACK);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MONSTER_ATTACK, pObJ);
				m_bCreateCol = true;
			}
			else if (!m_bLeftRight)
			{
				CGameObject* pObJ = CAbstractFactory<CCollision>::CreateCollision(m_tInfo.vPos.x + 20, m_tInfo.vPos.y, 32, 32);
				pObJ->Set_OBJID(OBJ_ID::MONSTER_ATTACK);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::MONSTER_ATTACK, pObJ);
				m_bCreateCol = true;
			}
		}
		else if (m_bCreateCol)
		{
			CGameObject_Manager::Get_Instance()->DeleteID(OBJ_ID::MONSTER_ATTACK);
		}
	}

	if (m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1)
	{
		m_bAnimisPlaying = false;
		Set_State(IDLE);
		m_fDelay = 0.f;
		m_bCreateCol = false;
	}
}

void CWarrior::HIT_ILSEOM_STATE()
{
	if (!static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_bHit_ILSeom())
	{
		m_bColorBlack = false;
	}

	if (m_bColorBlack)
	{
		m_fFrameSpeed = 0.f;
		m_fSpeed = 0.f;
		m_fDelay = 0.f;
	}
}

void CWarrior::FREEZE_STATE()
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
		m_pFreezeState->Set_ObjectKey(L"freeze_medium1");
		m_pFreezeState->Set_Target(this);
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::FREEZE, m_pFreezeState);
		CSoundManager::Get_Instance()->StopSound(CSoundManager::FREEZE_STATE);
		CSoundManager::Get_Instance()->PlaySound(L"State_Freeze.wav", CSoundManager::FREEZE_STATE);
		m_bFreezeState = true;
	}

	if (m_bFreezeState)
	{
		m_fFrameSpeed = 0.f;
		m_fSpeed = 0.f;
		m_fDelay = 0.f;
	}
}

void CWarrior::HIT_BROAD_SWORD()
{
	if (CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::BROADSWORD_COL) == nullptr)
	{
		m_bHitBroadSword = false;
	}
}

void CWarrior::HIT_GRENADE()
{
	if (CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::GRENADE_COL) == nullptr)
	{
		m_bHitGrenade = false;
	}
}

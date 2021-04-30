#include "stdafx.h"
#include "Clegane.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Scroll_Manager.h"
#include "GameObject_Manager.h"
#include "Time_Manager.h"
#include "StampingWave.h"
#include "Stamping_Hit_Effect.h"
#include "Stone.h"
#include "AttackSign.h"
#include "Player.h"
#include "Freeze_State.h"
#include "SoundManager.h"
#include "Collision.h"
#include "EventManager.h"

CClegane::CClegane() :
	m_iSelect(0)
	, m_fDelay(0.f)
	, m_fTimer(0.f)
	, m_fDist(0.f)
	, m_bCreateCol(false)
	, m_bAnimisPlaying(false)
	, m_bHitCheck(false)
	, m_bCreateWave(false)
	, m_bNormalStamping(false)
	, m_bJumpStamping(false)
	, m_bAttack(false)
	, m_eCurState(END)
	, m_ePreState(END)
	, m_fXSpeed(0.f)
	, m_fYSpeed(0.f)
	, m_fHeight(0.f)
	, m_fSoundDelay(0.f)
	, m_fEndHeight(0.f)
	, m_iSpawnCnt(0)
	, m_iPreSelect(-1)
	, m_bHitIceAllow(false)
	, m_bFreezeState(false)
	, m_bColorBlack(false)
	, m_bHitBroadSword(false)
	, m_bHitGrenade(false)
	, m_pMainHp(nullptr)
	, m_pServeHP(nullptr)
	, m_pHPBack(nullptr)
	, m_pMiniMapObj(nullptr)
	, m_pFreezeState(nullptr)
{
}


CClegane::~CClegane()
{
}

HRESULT CClegane::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"BOSSMONSTER";
	m_pStateKey = L"BOSS_INTRO";
	m_tFrame.fStartFrame = 0.f;
	m_tFrame.fEndFrame = 33.f;
	m_eCurState = INTRO;
	m_fGravity = 9.8f;
	m_fMaxHeight = 200.f; // 최대 높이
	m_fMaxTime = 0.7f; // 최대 높이까지 가는 시간

	//m_tStat.fMaxHP = 1000.f;
	m_tStat.fMaxHP = 100.f;
	m_tStat.fCurHP = m_tStat.fMaxHP;
	m_tInfo.iColCX = 200;
	m_tInfo.iColCY = 200;
	m_eOBJID = OBJ_ID::BOSS;
	return S_OK;
}

int CClegane::Update_GameObject()
{
	if (m_bDead)
	{
		/*m_pMainHp->Set_Target(nullptr);
		m_pServeHP->Set_Target(nullptr);
		m_pHPBack->Set_Target(nullptr);
		m_pMiniMapObj->Set_Target(nullptr);*/		
		
		//m_bFreezeState = false;

		DEAD_STATE();
	}

	float fX = m_pTarget->Get_Info().vPos.x - m_tInfo.vPos.x;
	float fY = m_pTarget->Get_Info().vPos.y - m_tInfo.vPos.y;
	m_fDist = sqrtf(fX * fX + fY * fY);

	if (m_tInfo.vPos.y <= m_pTarget->Get_Info().vPos.y)
		m_fRotation = acosf(fX / m_fDist);

	if (m_eCurState != INTRO)
	{
		FREEZE_STATE();
		HIT_ILSEOM_STATE();
		HIT_BROAD_SWORD();
		HIT_GRENADE();
	}

	if (!m_bColorBlack && !m_bFreezeState)
	{
		if (m_eCurState != INTRO && m_eCurState != WHIRLWIND_LOOP)
		{
		    m_fFrameSpeed = 1.f;
		}

		Change_State();
		Boss_Pattern();
		Check_LeftRight();
	}
	return OBJ_NOEVENT;
}

void CClegane::Late_Update_GameObject()
{
	if (m_tStat.fCurHP <= 0)
	{	
		m_eCurState = DEAD;
		m_bDead = true;
	}
	Wall_Check();
	FrameMove(m_fFrameSpeed);
}

void CClegane::Render_GameObject()
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

void CClegane::Release_GameObject()
{
}

void CClegane::Set_Damage(int _InDamage)
{
	m_tStat.fCurHP -= _InDamage;
}

void CClegane::Change_State()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CClegane::INTRO:
			m_pStateKey = L"BOSS_INTRO";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 33.f;
			m_fFrameSpeed = 0.3f;
			break;
		case CClegane::IDLE:
			m_pStateKey = L"BOSS_IDLE";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 6.f;
			m_fFrameSpeed = 1.f;
			break;
		case CClegane::EARTHQUAKE_READY:
			m_pStateKey = L"BOSS_EARTHQUAKE_READY";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 11.f;
			m_fFrameSpeed = 0.5f;
			break;
		case CClegane::EARTHQUAKE_JUMP:
			m_pStateKey = L"BOSS_EARTHQUAKE_JUMP";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 7.f;
			m_fFrameSpeed = 1.f;
			break;
		case CClegane::EARTHQUAKE_ATTACK:
			m_pStateKey = L"BOSS_EARTHQUAKE_ATTACK";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 7.f;
			m_fFrameSpeed = 0.7f;
			break;
		case CClegane::EARTHQUAKE_HIT:
			m_pStateKey = L"BOSS_EARTHQUAKE_HIT";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 8.f;
			m_fFrameSpeed = 0.4f;
			break;
		case CClegane::STAMPING_NORMAL:
			m_pStateKey = L"BOSS_STAMPING";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 7.f;
			m_fFrameSpeed = 1.f;
			break;
		case CClegane::STAMPING_READY:
			m_pStateKey = L"BOSS_STAMPING_READY";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 7.f;
			m_fFrameSpeed = 1.f;
			break;
		case CClegane::STAMPING_JUMP:
			m_pStateKey = L"BOSS_STAMPING_JUMP";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 7.f;
			m_fFrameSpeed = 1.f;
			break;
		case CClegane::STAMPING_HIT:
			m_pStateKey = L"BOSS_STAMPING_HIT";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 8.f;
			m_fFrameSpeed = 1.f;
			break;
		case CClegane::WHIRLWIND_READY:
			m_pStateKey = L"BOSS_WHIRLWIND_READY";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 1.f;
			m_fFrameSpeed = 1.f;
			break;
		case CClegane::WHIRLWIND_LOOP:
			m_pStateKey = L"BOSS_WHIRLWIND_LOOP";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 7.f;
			m_fFrameSpeed = 2.f;
			break;
		case CClegane::DEAD:
			m_pStateKey = L"BOSS_DEAD";
			m_tFrame.fStartFrame = 0.f;
			m_tFrame.fEndFrame = 5.f;
			m_fFrameSpeed = 1.f;
			break;
		default:
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CClegane::Check_LeftRight()
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

void CClegane::Boss_Pattern()
{
	switch (m_eCurState)
	{
	case CClegane::INTRO:
		INTRO_STATE();
		break;
	case CClegane::IDLE:
		IDLE_STATE();
		break;
	case CClegane::EARTHQUAKE_READY:
		EARTHQUAKE_READY_STATE();
		break;
	case CClegane::EARTHQUAKE_JUMP:
		EARTHQUAKE_JUMP_STATE();
		break;
	case CClegane::EARTHQUAKE_ATTACK:
		EARTHQUAKE_ATTACK_STATE();
		break;
	case CClegane::EARTHQUAKE_HIT:
		EARTHQUAKE_HIT_STATE();
		break;
	case CClegane::STAMPING_NORMAL:
		STAMPING_NORMAL_STATE();
		break;
	case CClegane::STAMPING_READY:
		STAMPING_READY_STATE();
		break;
	case CClegane::STAMPING_JUMP:
		STAMPING_JUMP_STATE();
		break;
	case CClegane::STAMPING_HIT:
		STAMPING_HIT_STATE();
		break;
	case CClegane::WHIRLWIND_READY:
		WHIRLWIND_READY_STATE();
		break;
	case CClegane::WHIRLWIND_LOOP:
		WHIRLWIND_LOOP_STATE();
		break;
	case CClegane::DEAD:
		DEAD_STATE();
		break;
	default:
		break;
	}
}

void CClegane::Wall_Check()
{
	if (m_tInfo.vPos.x < 50.)
	{
		m_tInfo.vPos.x = 50.f;
	}

	else if (m_tInfo.vPos.x > TILECX * TILEX >> 1)
	{
		m_tInfo.vPos.x = TILECX * TILEX >> 1;
	}
}

void CClegane::INTRO_STATE()
{
	if (m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
	{
		m_eCurState = IDLE;
	}
}

void CClegane::IDLE_STATE()
{
	m_bNormalStamping = false;
	m_bCreateWave = false;
	m_fDelay += CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_iSelect = rand() % 3;

	if (m_fDelay >= 1.5f)
	{
		if (m_iSelect != m_iPreSelect)
		{
			switch (m_iSelect)
			{
			case 0:
				m_eCurState = STAMPING_READY;
				break;
			case 1:
				m_eCurState = EARTHQUAKE_READY;
				break;
			case 2:
				m_eCurState = WHIRLWIND_READY;
				break;
			default:
				break;
			}
			m_fDelay = 0.f;
			m_iPreSelect = m_iSelect;
		}
	}

}

void CClegane::EARTHQUAKE_READY_STATE()
{
	m_bAnimisPlaying = true;

	if (!m_bAttack)
	{
		CGameObject* pObj = CAbstractFactory<CAttackSign>::Create(m_pTarget->Get_Info().vPos.x, 488.f);
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);
				
		m_vSavePos.x = m_pTarget->Get_Info().vPos.x;
		m_vSavePos.y = 488.f;

		m_bAttack = true;
	}

	if (m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
	{
		CSoundManager::Get_Instance()->PlaySound(L"AdventurerWarrior_Voice_Middle.wav", CSoundManager::BOSS_STAMPING_NORMAL);
		m_eCurState = EARTHQUAKE_ATTACK;
		m_bAttack = false;
	}
}

void CClegane::EARTHQUAKE_JUMP_STATE()
{
	m_bAnimisPlaying = true;

	// 도착 지점의 높이 - 시작 지점의 높이
	m_fEndHeight = m_vEndPos.y - m_vStartPos.y;

	// 최대 높이 - 시작 지점의 높이
	m_fHeight = m_fMaxHeight - m_vStartPos.y;

	// Y축으로의 중력 가속도
	m_fGravity = 2 * m_fHeight / (m_fMaxTime * m_fMaxTime);

	// Y축의 스피드
	m_fYSpeed = sqrtf(2 * m_fGravity * m_fHeight);

	float a = m_fGravity;
	float b = -2 * m_fYSpeed;
	float c = 2 * m_fEndHeight;

	// 도착 지점 도달 시간
	m_fEndTime = (-b + sqrtf(b * b - 4 * a * c)) / (2 * a);

	// x축 속도
	m_fXSpeed = (m_vStartPos.x - m_vEndPos.x) / m_fEndTime;

	// 시간
	m_fTimer += CTime_Manager::Get_Instance()->Get_DeltaTime();

	m_tInfo.vPos.x = m_vStartPos.x + m_fXSpeed * m_fTimer;
	m_tInfo.vPos.y = m_vStartPos.y - (m_fYSpeed * m_fTimer) - (0.5f * m_fGravity * m_fTimer *m_fTimer);

	if (m_vEndPos.x >= m_tInfo.vPos.x && m_vEndPos.y <= m_tInfo.vPos.y
		|| m_vEndPos.x <= m_tInfo.vPos.x && m_vEndPos.y <= m_tInfo.vPos.y)
	{
		m_fTimer = 0.f;
		m_tInfo.vPos.x = m_vEndPos.x;
		m_tInfo.vPos.y = 460.f;
		m_eCurState = EARTHQUAKE_HIT;
	}
}

void CClegane::EARTHQUAKE_ATTACK_STATE()
{
	CGameObject* pObj = nullptr;
	m_fDelay += CTime_Manager::Get_Instance()->Get_DeltaTime();
	int fX = 30;
	int fY = 0;

	if (m_tFrame.fStartFrame >= 0.f)
	{
		if (m_fDelay >= 0.05f && m_iSpawnCnt < 13)
		{
			switch (m_iSpawnCnt)
			{
			case 0:
				pObj = CAbstractFactory<CStone>::Create(m_vSavePos.x + rand() % fX, 0.f);
				pObj->Set_ObjectKey(L"Stone_3");
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STONE, pObj);
				break;
			case 1:
				pObj = CAbstractFactory<CStone>::Create(m_vSavePos.x - rand() % fX, 0.f);
				pObj->Set_ObjectKey(L"Stone_2");
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STONE, pObj);
				break;
			case 2:
				pObj = CAbstractFactory<CStone>::Create(m_vSavePos.x + rand() % fX, 0.f);
				pObj->Set_ObjectKey(L"Stone_3");
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STONE, pObj);
				break;
			case 3:
				pObj = CAbstractFactory<CStone>::Create(m_vSavePos.x - rand() % fX, 0.f);
				pObj->Set_ObjectKey(L"Stone_4");
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STONE, pObj);
				break;
			case 4:
				pObj = CAbstractFactory<CStone>::Create(m_vSavePos.x + rand() % fX, 0.f);
				pObj->Set_ObjectKey(L"Stone_1");
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STONE, pObj);
				break;
			case 5:
				pObj = CAbstractFactory<CStone>::Create(m_vSavePos.x - rand() % fX, 0.f);
				pObj->Set_ObjectKey(L"Stone_2");
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STONE, pObj);
				break;
			case 6:
				pObj = CAbstractFactory<CStone>::Create(m_vSavePos.x + rand() % fX, 0.f);
				pObj->Set_ObjectKey(L"Stone_2");
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STONE, pObj);
				break;
			case 7:
				pObj = CAbstractFactory<CStone>::Create(m_vSavePos.x - rand() % fX, 0.f);
				pObj->Set_ObjectKey(L"Stone_2");
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STONE, pObj);
				break;
			case 8:
				pObj = CAbstractFactory<CStone>::Create(m_vSavePos.x + rand() % fX, 0.f);
				pObj->Set_ObjectKey(L"Stone_3");
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STONE, pObj);
				break;
			case 9:
				pObj = CAbstractFactory<CStone>::Create(m_vSavePos.x - rand() % fX, 0.f);
				pObj->Set_ObjectKey(L"Stone_2");
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STONE, pObj);
				break;
			case 10:
				pObj = CAbstractFactory<CStone>::Create(m_vSavePos.x + rand() % fX, 0.f);
				pObj->Set_ObjectKey(L"Stone_4");
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STONE, pObj);
				break;
			case 11:
				pObj = CAbstractFactory<CStone>::Create(m_vSavePos.x - rand() % fX, 0.f);
				pObj->Set_ObjectKey(L"Stone_4");
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STONE, pObj);
				break;
			default:
				break;
			}			
			m_iSpawnCnt++;
			m_fDelay = 0.f;
		}
	}
	CSoundManager::Get_Instance()->PlaySound(L"Atk_Explosion_Large.wav", CSoundManager::BOSS_EARTHQUAKE_ATTACK);

	if (m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
	{
		m_eCurState = IDLE;
		m_bAnimisPlaying = false;
		m_iSpawnCnt = 0;
	}
}

void CClegane::EARTHQUAKE_HIT_STATE()
{
	m_fDelay += CTime_Manager::Get_Instance()->Get_DeltaTime();
	CGameObject* pObj = nullptr;

	if (m_tFrame.fStartFrame >= 0.f && m_iSpawnCnt < 3)
	{
		if (m_fDelay >= 0.1f)
		{
			switch (m_iSpawnCnt)
			{
			case 0:
				pObj = CAbstractFactory<CStamping_Hit_Effect>::Create(m_tInfo.vPos.x, 460.f);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STAMPING_HIT, pObj);						
				break;
			case 1:
				pObj = CAbstractFactory<CStamping_Hit_Effect>::Create(m_tInfo.vPos.x + 100, 460.f);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STAMPING_HIT, pObj);				

				pObj = CAbstractFactory<CStamping_Hit_Effect>::Create(m_tInfo.vPos.x - 100, 460.f);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STAMPING_HIT, pObj);			
				break;
			case 2:
				pObj = CAbstractFactory<CStamping_Hit_Effect>::Create(m_tInfo.vPos.x + 200, 460.f);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STAMPING_HIT, pObj);
				

				pObj = CAbstractFactory<CStamping_Hit_Effect>::Create(m_tInfo.vPos.x - 200, 460.f);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STAMPING_HIT, pObj);							
				break;
			default:
				break;
			}
			m_iSpawnCnt++;
			m_fDelay = 0.f;
		}
		CSoundManager::Get_Instance()->PlaySound(L"AdventurerWarrior_Voice_Middle.wav", CSoundManager::BOSS_STAMPING_NORMAL);
		CSoundManager::Get_Instance()->PlaySound(L"Atk_Explosion_Large.wav", CSoundManager::BOSS_WAVE);
	}

	if (m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
	{
		m_eCurState = IDLE;
		m_bAnimisPlaying = false;
		m_fDelay = 0.f;
		m_iSpawnCnt = 0;
	}
}

void CClegane::STAMPING_NORMAL_STATE()
{
	m_bNormalStamping = true;
	if (m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
	{
		m_eCurState = STAMPING_HIT;
	}
}

void CClegane::STAMPING_READY_STATE()
{
	m_bAnimisPlaying = true;
	m_iSelect = rand() % 3;
	m_fDelay += CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_vStartPos = m_tInfo.vPos;
	m_vEndPos = { m_pTarget->Get_Info().vPos.x ,488.f ,0.f };

	CSoundManager::Get_Instance()->PlaySound(L"AdventurerWarrior_Voice_Casting.wav", CSoundManager::BOSS_STAMPING_READY);
	if (m_fDelay >= 2.f)
	{
		switch (m_iSelect)
		{
		case 0:
			m_eCurState = STAMPING_JUMP;
			break;
		case 1:
			m_eCurState = STAMPING_NORMAL;
			break;
		case 2:
			m_eCurState = EARTHQUAKE_JUMP;
			break;
		default:
			break;
		}
		m_fDelay = 0.f;
	}

}

void CClegane::STAMPING_JUMP_STATE()
{
	m_bAnimisPlaying = true;

	// 도착 지점의 높이 - 시작 지점의 높이
	m_fEndHeight = m_vEndPos.y - m_vStartPos.y;

	// 최대 높이 - 시작 지점의 높이
	m_fHeight = m_fMaxHeight - m_vStartPos.y;

	// Y축으로의 중력 가속도
	m_fGravity = 2 * m_fHeight / (m_fMaxTime * m_fMaxTime);

	// Y축의 스피드
	m_fYSpeed = sqrtf(2 * m_fGravity * m_fHeight);

	float a = m_fGravity;
	float b = -2 * m_fYSpeed;
	float c = 2 * m_fEndHeight;

	// 도착 지점 도달 시간
	m_fEndTime = (-b + sqrtf(b * b - 4 * a * c)) / (2 * a);

	// x축 속도
	m_fXSpeed = (m_vStartPos.x - m_vEndPos.x) / m_fEndTime;

	// 시간
	m_fTimer += CTime_Manager::Get_Instance()->Get_DeltaTime();

	m_tInfo.vPos.x = m_vStartPos.x + m_fXSpeed * m_fTimer;
	m_tInfo.vPos.y = m_vStartPos.y - (m_fYSpeed * m_fTimer) - (0.5f * m_fGravity * m_fTimer *m_fTimer);

	if (m_vEndPos.x >= m_tInfo.vPos.x && m_vEndPos.y <= m_tInfo.vPos.y
		|| m_vEndPos.x <= m_tInfo.vPos.x && m_vEndPos.y <= m_tInfo.vPos.y)
	{
		m_fTimer = 0.f;
		m_tInfo.vPos.x = m_vEndPos.x;
		m_tInfo.vPos.y = 460.f;
		m_bJumpStamping = true;
		m_eCurState = STAMPING_HIT;
	}
}

void CClegane::STAMPING_HIT_STATE()
{

	if (m_tFrame.fStartFrame >= 2.f && m_bNormalStamping)
	{
		if (!m_bCreateWave)
		{
			if (m_bLeftRight)
			{
				CGameObject* pObj = CAbstractFactory<CStampingWave>::Create(m_tInfo.vPos.x - 20, m_tInfo.vPos.y);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STAMPING_WAVE, pObj);
			}
			else
			{
				CGameObject* pObj = CAbstractFactory<CStampingWave>::Create(m_tInfo.vPos.x + 20, m_tInfo.vPos.y);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STAMPING_WAVE, pObj);
			}
			CSoundManager::Get_Instance()->StopSound(CSoundManager::BOSS_STAMPING_NORMAL);
			CSoundManager::Get_Instance()->PlaySound(L"AdventurerWarrior_Voice_Middle.wav", CSoundManager::BOSS_STAMPING_NORMAL);
			CSoundManager::Get_Instance()->StopSound(CSoundManager::BOSS_WAVE);
			CSoundManager::Get_Instance()->PlaySound(L"Atk_Explosion_Large.wav", CSoundManager::BOSS_WAVE);
			m_bCreateWave = true;
		}
	}
	else if (m_tFrame.fStartFrame >= 1.f && !m_bNormalStamping)
	{

		m_fDelay += CTime_Manager::Get_Instance()->Get_DeltaTime();
		CGameObject* pObj = nullptr;
		
		if (m_tFrame.fStartFrame >= 0.f && m_iSpawnCnt < 3)
		{			
			if (m_fDelay >= 0.1f)
			{
				if (m_bLeftRight)
				{
					switch (m_iSpawnCnt)
					{
					case 0:
						pObj = CAbstractFactory<CStamping_Hit_Effect>::Create(m_tInfo.vPos.x - 100, 460.f);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STAMPING_HIT, pObj);					
						break;
					case 1:
						pObj = CAbstractFactory<CStamping_Hit_Effect>::Create(m_tInfo.vPos.x - 200, 460.f);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STAMPING_HIT, pObj);						

						pObj = CAbstractFactory<CStamping_Hit_Effect>::Create(m_tInfo.vPos.x - 300, 460.f);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STAMPING_HIT, pObj);
						break;
					
					case 2:
						pObj = CAbstractFactory<CStamping_Hit_Effect>::Create(m_tInfo.vPos.x - 400, 460.f);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STAMPING_HIT, pObj);
											

						pObj = CAbstractFactory<CStamping_Hit_Effect>::Create(m_tInfo.vPos.x - 500, 460.f);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STAMPING_HIT, pObj);					
						break;
					default:
						break;
					}
					m_iSpawnCnt++;
					m_fDelay = 0.f;
				}
				else
				{
					switch (m_iSpawnCnt)
					{
					case 0:
						pObj = CAbstractFactory<CStamping_Hit_Effect>::Create(m_tInfo.vPos.x + 100, 460.f);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STAMPING_HIT, pObj);					
						break;
					case 1:
						pObj = CAbstractFactory<CStamping_Hit_Effect>::Create(m_tInfo.vPos.x + 200, 460.f);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STAMPING_HIT, pObj);					

						pObj = CAbstractFactory<CStamping_Hit_Effect>::Create(m_tInfo.vPos.x + 300, 460.f);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STAMPING_HIT, pObj);						
						break;
					case 2:
						pObj = CAbstractFactory<CStamping_Hit_Effect>::Create(m_tInfo.vPos.x + 400, 460.f);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STAMPING_HIT, pObj);					

						pObj = CAbstractFactory<CStamping_Hit_Effect>::Create(m_tInfo.vPos.x + 500, 460.f);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::STAMPING_HIT, pObj);						
						break;
					default:
						break;
					}
					m_iSpawnCnt++;
					m_fDelay = 0.f;
				}
			}
		}
		CSoundManager::Get_Instance()->PlaySound(L"AdventurerWarrior_Voice_Middle.wav", CSoundManager::BOSS_STAMPING_NORMAL);
		CSoundManager::Get_Instance()->PlaySound(L"Atk_Explosion_Large.wav", CSoundManager::BOSS_WAVE);	
	}

	if (m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
	{
		m_eCurState = IDLE;
		m_bAnimisPlaying = false;
	}
}

void CClegane::WHIRLWIND_READY_STATE()
{
	m_fDelay += CTime_Manager::Get_Instance()->Get_DeltaTime();

	m_bAnimisPlaying = true;

	CSoundManager::Get_Instance()->PlaySound(L"AdventurerWarrior_Voice_Casting.wav", CSoundManager::BOSS_STAMPING_READY);

	if (m_fDelay >= 2.f)
	{
		m_eCurState = WHIRLWIND_LOOP;
		m_fDelay = 0.f;
		m_bAnimisPlaying = false;
	}
}

void CClegane::WHIRLWIND_LOOP_STATE()
{
	m_fSoundDelay += CTime_Manager::Get_Instance()->Get_DeltaTime();

	if (m_fSoundDelay >= 0.5f)
	{
		CSoundManager::Get_Instance()->StopSound(CSoundManager::BOSS_WHIRLWIND);
		CSoundManager::Get_Instance()->PlaySound(L"Atk_Flame_Small.wav", CSoundManager::BOSS_WHIRLWIND);
		m_fSoundDelay = 0.f;
	}

	m_fDelay += CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_fSpeed = 300.f;

	m_bAnimisPlaying = true;

	if (m_tInfo.vPos.x < m_pTarget->Get_Info().vPos.x)
		m_fRotation *= -1.f;

	m_tInfo.vPos.x += cosf(m_fRotation) * CTime_Manager::Get_Instance()->Get_DeltaTime() * m_fSpeed;

	if(m_tFrame.fStartFrame)

	if (m_fDelay >= 6.f)
	{
		m_eCurState = IDLE;
		m_bAnimisPlaying = false;
		m_fDelay = 0.f;
	}
}

void CClegane::DEAD_STATE()
{
	if (m_eCurState == DEAD && m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 2.f)
	{
		m_tFrame.fStartFrame = m_tFrame.fEndFrame - 2.f;
		m_fFrameSpeed = 0.f;

		if (m_pFreezeState != nullptr)
			m_pFreezeState = nullptr;

		if (CEventManager::Get_Instance()->Get_EventCnt() < 5)
		{			
			CEventManager::Get_Instance()->Set_EventCnt();
			CEventManager::Get_Instance()->Set_StartEvent(true);
		}		
	}	
	
}

void CClegane::HIT_ILSEOM_STATE()
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

void CClegane::FREEZE_STATE()
{
	if (CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::ICE_ALLOW) == nullptr)
	{
		m_bHitIceAllow = false;
	}

	if (CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::FREEZE) == nullptr)
	{
		m_bFreezeState = false;		
	}

	if (m_eCurState == WHIRLWIND_LOOP && m_bFreezeState)
		m_eCurState = IDLE;

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

void CClegane::HIT_BROAD_SWORD()
{
	if (CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::BROADSWORD_COL) == nullptr)
	{
		m_bHitBroadSword = false;
	}
}

void CClegane::HIT_GRENADE()
{
	if (CGameObject_Manager::Get_Instance()->Get_Target(OBJ_ID::GRENADE_COL) == nullptr)
	{
		m_bHitGrenade = false;
	}
}

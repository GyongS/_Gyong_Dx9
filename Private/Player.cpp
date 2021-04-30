#include "stdafx.h"
#include "Player.h"
#include "Time_Manager.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Key_Manager.h"
#include "PrisonerSwap_Effect.h"
#include "GameObject_Manager.h"
#include "Scroll_Manager.h"
#include "Ghost_Effect.h"
#include "IceCrossAllow.h"
#include "IceCrossShotEffect.h"
#include "Grenade.h"
#include "DashEffect.h"
#include "BroadSword_Effect.h"
#include "PrisonerSwap_Ready.h"
#include "DoubleJumpEffect.h"
#include "Collision.h"
#include "Scene_Manager.h"
#include "EventManager.h"
#include "DialogueBody.h"
#include "SkillCoolDown.h"
#include "SwapCoolDown.h"
#include "SoundManager.h"
#include "SkulHead.h"

CPlayer::CPlayer() : m_iComboCnt(0), m_bJump(false), m_iJumpCnt(0)
, m_fJumpForce(0.f), m_fJumpTime(0.f), m_fGravity(0.f), m_fJumpY(0.f), m_fDashSpeed(0.f), m_fDashTime(0.f)
, m_fTime(0.f)
, m_iColCnt(0)
, m_bDash(false), m_bDoubleJump(false), m_iMainFaceIconNum(0), m_iSwitchFaceIconNum(0), m_bSwap(false), m_bGround(false)
, m_bSkill_B_Shot(false)
, m_iHeadCnt(0)
, m_bDropItem(false)
, m_bPlayingBroadSword(false)
, m_bPlayingAnim(false)
, m_bStartDashEffect(false)
, m_bStartAttack(false)
, m_bSpawnEffect(false)
, m_bCheckCol(false)
, m_bGetBone(false)
, m_bInPortal(false)
, m_bILSEOM_Start(false)
, m_bILSEOM_End(false)
, m_bHit_ILSeom(false)
, m_bWalk(false)
, m_bUse_SKILL_A(false)
, m_bUse_SKILL_S(false)
, m_bUse_SKILL_D(false)
, m_bUse_Swap(false)
, m_bPickUpHead(false)
, m_fILSEOM_Delay(0.f)
, m_eCurType(TYPE_DEFAULT_NOBONE)
, m_eSaveType(TYPE_END)
, m_eCurDefaultNoboneState(DEFAULT_NOBONE_END)
, m_ePreDefaultNoboneState(DEFAULT_NOBONE_END)
, m_eCurDefaultState(DEFAULT_END)
, m_ePreDefaultState(DEFAULT_END)
, m_eCurDefaultNoHeadState(DEFAULT_NOHEAD_END)
, m_ePreDefaultNoHeadState(DEFAULT_NOHEAD_END)
, m_eCurPrisonerState(PRISONER_END)
, m_ePrePrisonerState(PRISONER_END)
, m_eCurSamuraiState(SAMURAI_END)
, m_ePreSamuraiState(SAMURAI_END)
, m_bHit(false)
, m_fInvincibletime(0.f)
{

}

HRESULT CPlayer::Ready_GameObject()
{
	m_tInfo.vPos = { 150.f, 430.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_fRotation = 345.f;
	m_tStat.fMaxHP = 150.f;
	m_tStat.fCurHP = m_tStat.fMaxHP;
	m_tStat.fDamage = 20.f;
	m_fSpeed = 200.f;
	m_pObjectKey = L"DEFAULT";
	m_pStateKey = L"DEFAULT_IDLE_NOBONE";
	/*m_pObjectKey = L"SAMURAI_MODE";
	m_pStateKey = L"SAMURAI_MODE_IDLE";*/
	/*m_pObjectKey = L"PRISONER_MODE";
	m_pStateKey = L"PRISONER_MODE_IDLE";*/
	//m_pStateKey = L"DEFAULT_RESPAWN";
	//m_eCurDefaultState = DEFAULT_IDLE;
	m_eCurDefaultNoboneState = DEFAULT_NOBONE_IDLE;
	m_eCurPrisonerState = PRISONER_IDLE;
	m_eCurSamuraiState = SAMURAI_IDLE;
	m_fJumpForce = 50.f;
	m_fGravity = 9.8f;
	m_fDashSpeed = 300.f;
	m_tInfo.iColCX = 40;
	m_tInfo.iColCY = 40;
	m_eOBJID = OBJ_ID::PLAYER;
	return S_OK;
}

int CPlayer::Update_GameObject()
{
	if (CEventManager::Get_Instance()->Get_bEvent())
	{
		Tutorial_Stage();
		return OBJ_NOEVENT;
	}

	NextStage();

	if (m_bHit)
	{
		m_fInvincibletime += CTime_Manager::Get_Instance()->Get_DeltaTime();
		if (m_fInvincibletime >= 2.f)
		{
			m_bHit = false;
			m_fInvincibletime = 0.f;
		}
	}

	if (m_tStat.fCurHP <= 0.f)
		m_tStat.fCurHP = m_tStat.fMaxHP;

	if (m_bDead)
		return OBJ_DEAD;

	Change_Type();
	Change_AnimState();
	Update_SkillCoolDown();



	if (m_eCurDefaultState == DEFAULT_RESPAWN && m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
	{
		m_eCurDefaultState = DEFAULT_IDLE;
	}

	if (m_eCurDefaultState != DEFAULT_RESPAWN)
	{
		STATE_SWAP();

		if (!m_bPlayingAnim && !m_bStartAttack)
		{
			STATE_WALK();
		}

		if (!m_bSwap)
		{
			if (!m_bPlayingAnim)
			{
				STATE_ATTACK();
				if (!m_bStartAttack)
				{
					STATE_JUMP();
					STATE_DASH();
				}
			}
			STATE_SKILL_A();
			STATE_SKILL_S();
			STATE_SKILL_D();

		}
	}
	OffSet();
	return OBJ_NOEVENT;
}

void CPlayer::Late_Update_GameObject()
{
	CGameObject::FrameMove(m_fFrameSpeed);
}

void CPlayer::Render_GameObject()
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
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	/*D3DXMATRIX FontmatScale, FontmatTrans, FontmatWorld;
	D3DXMATRIX FontmatScale2, FontmatTrans2, FontmatWorld2;
*/
/*TCHAR szbuff[64], szTemp[64];

swprintf_s(szbuff, L"%f", m_tInfo.vPos.x);

D3DXMatrixScaling(&FontmatScale, 1.f, 1.f, 0.f);
D3DXMatrixTranslation(&FontmatTrans, 400.f, 300.f, 0.f);

FontmatWorld = FontmatScale * FontmatTrans;

CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&FontmatWorld);
CGraphicDevice::Get_Instance()->Get_Font()->DrawTextW(CGraphicDevice::Get_Instance()->Get_Sprite(), szbuff, lstrlen(szbuff), nullptr, 0, D3DCOLOR_ARGB(255, 255, 0, 0));

swprintf_s(szTemp, L"%f", m_tInfo.vPos.y);

D3DXMatrixScaling(&FontmatScale2, 1.f, 1.f, 0.f);
D3DXMatrixTranslation(&FontmatTrans2, 400.f, 330.f, 0.f);

FontmatWorld2 = FontmatScale2 * FontmatTrans2;

CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&FontmatWorld2);
CGraphicDevice::Get_Instance()->Get_Font()->DrawTextW(CGraphicDevice::Get_Instance()->Get_Sprite(), szTemp, lstrlen(szTemp), nullptr, 0, D3DCOLOR_ARGB(255, 255, 0, 0));*/

}

void CPlayer::Release_GameObject()
{

}

void CPlayer::Tutorial_Stage()
{
	switch (m_eCurType)
	{
	case CPlayer::TYPE_DEFAULT:
		m_eCurDefaultState = DEFAULT_IDLE;
		Change_AnimState();
		break;
	case CPlayer::TYPE_DEFAULT_NOBONE:
		m_eCurDefaultNoboneState = DEFAULT_NOBONE_IDLE;
		STATE_GETBONE();
		Change_AnimState();
		break;
	case CPlayer::TYPE_DEFAULT_NOHEAD:
		m_eCurDefaultNoHeadState = DEFAULT_NOHEAD_IDLE;
		Change_AnimState();
		break;
	case CPlayer::TYPE_PRISONER:
		m_eCurPrisonerState = PRISONER_IDLE;
		Change_AnimState();
		break;
	case CPlayer::TYPE_SAMURAI:
		m_eCurSamuraiState = SAMURAI_IDLE;
		Change_AnimState();
		break;
	case CPlayer::TYPE_END:
		break;
	default:
		break;
	}

	if (!CEventManager::Get_Instance()->Get_bCreateDialogue())
	{
		CGameObject* pObj = CAbstractFactory<CDialogueBody>::Create(400.f, 550.f);
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EVENT_UI, pObj);
		CEventManager::Get_Instance()->Set_CreateDialogue(true);
	}
}

void CPlayer::NextStage()
{
	if (m_bInPortal)
	{
		if (CKey_Manager::Get_Instance()->Key_Down(KEY_F))
		{
			switch (CScene_Manager::Get_Instance()->Get_Scene())
			{
			case CScene_Manager::SCENE_STAGE1:
				CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_STAGE2);
				break;
			case CScene_Manager::SCENE_STAGE2:
				CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_STAGE3);
				break;
			case CScene_Manager::SCENE_STAGE3:
				CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_STAGE4);
				break;
			case CScene_Manager::SCENE_STAGE4:
				CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_STAGE5);
				break;
			case CScene_Manager::SCENE_STAGE5:
				CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::BONUS_STAGE);
				break;
			case CScene_Manager::BONUS_STAGE:
				CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_STAGE6);
				break;
			case CScene_Manager::SCENE_STAGE6:
				CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_STAGE7);
				break;
			default:
				break;
			}
			m_bInPortal = false;
		}
	}
	else if (m_bInItemShop)
	{
		if (CKey_Manager::Get_Instance()->Key_Down(KEY_F))
		{
			// 아이템 구매
		}
		m_bInItemShop = false;
	}
}

void CPlayer::Change_Type()
{
	CGameObject* pObj = nullptr;

	if (m_eCurType != m_ePreType && !m_bSwap && m_eCurType == TYPE_DEFAULT_NOHEAD)
	{
		m_eCurDefaultNoHeadState = DEFAULT_NOHEAD_IDLE;
		m_pObjectKey = L"DEFAULT_NOHEAD";
		m_pStateKey = L"DEFAULT_IDLE_NOBONE";
		m_ePreType = m_eCurType;
	}

	if (m_eCurType != m_ePreType && !m_bSwap &&
		CScene_Manager::Get_Instance()->Get_Scene() != CScene_Manager::SCENE_STAGE1) /////////// !=
	{
		switch (m_eCurType)
		{
		case CPlayer::TYPE_DEFAULT:
			m_eCurDefaultState = DEFAULT_SWITCH;
			m_pObjectKey = L"DEFAULT";
			m_pStateKey = L"DEFAULT_SWITCH";
			m_bSwap = true;
			break;
		case CPlayer::TYPE_PRISONER:
			m_tInfo.iColCX = 40;
			m_tInfo.iColCY = 50;
			m_eCurPrisonerState = PRISONER_SWITCH;
			m_pObjectKey = L"PRISONER_MODE";
			m_pStateKey = L"PRISONER_MODE_SWITCH";
			pObj = CAbstractFactory<CPrisonerSwap_Ready>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);
			CSoundManager::Get_Instance()->StopSound(CSoundManager::SWAP_READY);
			CSoundManager::Get_Instance()->PlaySound(L"Default_Switch.wav", CSoundManager::SWAP_READY);
			m_bSwap = true;
			break;
		case CPlayer::TYPE_SAMURAI:
			m_tInfo.iColCX = 41;
			m_tInfo.iColCY = 36;
			m_eCurSamuraiState = SAMURAI_SWITCH;
			m_pObjectKey = L"SAMURAI_MODE";
			m_pStateKey = L"SAMURAI_MODE_SWITCH";
			CSoundManager::Get_Instance()->StopSound(CSoundManager::SWAP_READY);
			CSoundManager::Get_Instance()->PlaySound(L"Default_Switch.wav", CSoundManager::SWAP_READY);
			m_bSwap = true;
			break;
		default:
			break;
		}
		m_ePreType = m_eCurType;
	}
}

void CPlayer::Change_AnimState()
{
	switch (m_eCurType)
	{
	case CPlayer::TYPE_DEFAULT:
		if (m_ePreDefaultState != m_eCurDefaultState)
		{
			switch (m_eCurDefaultState)
			{
			case CPlayer::DEFAULT_RESPAWN:
				m_pStateKey = L"DEFAULT_RESPAWN";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 26.f;
				m_fFrameSpeed = 0.3f;
				break;
			case CPlayer::DEFAULT_IDLE:
				m_pStateKey = L"DEFAULT_IDLE";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 4.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::DEFAULT_WALK:
				m_pStateKey = L"DEFAULT_WALK";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 8.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::DEFAULT_ATTACK_A:
				m_pStateKey = L"DEFAULT_ATTACK";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 4.f;
				m_fFrameSpeed = 1.3f;
				break;
			case CPlayer::DEFAULT_ATTACK_B:
				m_pStateKey = L"DEFAULT_ATTACK2";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 4.f;
				m_fFrameSpeed = 1.3f;
				break;
			case CPlayer::DEFAULT_JUMP:
				m_pStateKey = L"DEFAULT_JUMP";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 2.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::DEFAULT_DOUBLEJUMP:
				m_pStateKey = L"DEFAULT_JUMP";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 2.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::DEFAULT_FALL:
				m_pStateKey = L"DEFAULT_FALL";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 5.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::DEFAULT_DASH:
				m_pStateKey = L"DEFAULT_DASH";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 2.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::DEFAULT_SWITCH:
				m_pStateKey = L"DEFAULT_SWITCH";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 7.f;
				m_fFrameSpeed = 1.3f;
				break;
			case CPlayer::DEFAULT_SKILL_A:
				break;
			case CPlayer::DEFAULT_DEAD:
				m_pStateKey = L"DEFAULT_DEAD";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 9.f;
				m_fFrameSpeed = 1.f;
				break;
			}
			m_ePreDefaultState = m_eCurDefaultState;
		}
		break;
	case CPlayer::TYPE_DEFAULT_NOBONE:
		if (m_ePreDefaultNoboneState != m_eCurDefaultNoboneState)
		{
			switch (m_eCurDefaultNoboneState)
			{
			case CPlayer::DEFAULT_NOBONE_IDLE:
				m_pStateKey = L"DEFAULT_IDLE_NOBONE";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 4.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::DEFAULT_NOBONE_WALK:
				m_pStateKey = L"DEFAULT_WALK_NOBONE";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 8.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::DEFAULT_NOBONE_JUMP:
				m_pStateKey = L"DEFAULT_JUMP";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 2.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::DEFAULT_NOBONE_FALL:
				m_pStateKey = L"DEFAULT_FALL";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 5.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::DEFAULT_NOBONE_DASH:
				m_pStateKey = L"DEFAULT_DASH";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 2.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::DEFAULT_NOBONE_GETBONE:
				m_pStateKey = L"DEFAULT_GET_BONE";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 10.f;
				m_fFrameSpeed = 0.7f;
				break;
			default:
				break;
			}
			m_ePreDefaultNoboneState = m_eCurDefaultNoboneState;
		}
		break;
	case CPlayer::TYPE_DEFAULT_NOHEAD:
		if (m_ePreDefaultNoHeadState != m_eCurDefaultNoHeadState)
		{
			switch (m_eCurDefaultNoHeadState)
			{
			case CPlayer::DEFAULT_NOHEAD_IDLE:
				m_pStateKey = L"DEFAULT_NOHEAD_IDLE";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 3.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::DEFAULT_NOHEAD_WALK:
				m_pStateKey = L"DEFAULT_NOHEAD_WALK";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 8.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::DEFAULT_NOHEAD_ATTACK_A:
				m_pStateKey = L"DEFAULT_NOHEAD_ATTACK";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 4.f;
				m_fFrameSpeed = 1.3f;
				break;
			case CPlayer::DEFAULT_NOHEAD_ATTACK_B:
				m_pStateKey = L"DEFAULT_NOHEAD_ATTACK2";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 4.f;
				m_fFrameSpeed = 1.3f;
				break;
			case CPlayer::DEFAULT_NOHEAD_JUMP:
				m_pStateKey = L"DEFAULT_NOHEAD_JUMP";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 2.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::DEFAULT_NOHEAD_FALL:
				m_pStateKey = L"DEFAULT_NOHEAD_FALL";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 5.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::DEFAULT_NOHEAD_DASH:
				m_pStateKey = L"DEFAULT_NOHEAD_DASH";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 1.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::DEFAULT_NOHEAD_DEAD:
				break;
			default:
				break;
			}
			m_ePreDefaultNoHeadState = m_eCurDefaultNoHeadState;
		}
		break;
	case CPlayer::TYPE_PRISONER:
		if (m_ePrePrisonerState != m_eCurPrisonerState)
		{
			switch (m_eCurPrisonerState)
			{
			case CPlayer::PRISONER_IDLE:
				m_pStateKey = L"PRISONER_MODE_IDLE";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 6.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::PRISONER_WALK:
				m_pStateKey = L"PRISONER_MODE_WALK";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 10.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::PRISONER_ATTACK_A:
				m_pStateKey = L"PRISONER_MODE_ATTACK_A";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 5.f;
				m_fFrameSpeed = 1.7f;
				break;
			case CPlayer::PRISONER_ATTACK_B:
				m_pStateKey = L"PRISONER_MODE_ATTACK_B";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 5.f;
				m_fFrameSpeed = 1.7f;
				break;
			case CPlayer::PRISONER_ATTACK_C:
				m_pStateKey = L"PRISONER_MODE_ATTACK_C";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 5.f;
				m_fFrameSpeed = 1.7f;
				break;
			case CPlayer::PRISONER_ATTACK_D:
				m_pStateKey = L"PRISONER_MODE_ATTACK_D";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 6.f;
				m_fFrameSpeed = 1.5f;
				break;
			case CPlayer::PRISONER_ATTACK_E:
				m_pStateKey = L"PRISONER_MODE_ATTACK_E";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 5.f;
				m_fFrameSpeed = 1.5f;
				break;
			case CPlayer::PRISONER_DASH:
				m_pStateKey = L"PRISONER_MODE_DASH";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 7.f;
				m_fFrameSpeed = 0.5f;
				break;
			case CPlayer::PRISONER_JUMP:
				m_pStateKey = L"PRISONER_MODE_JUMP";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 3.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::PRISONER_DOUBLE_JUMP:
				m_pStateKey = L"PRISONER_MODE_JUMP";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 3.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::PRISONER_JUMPATTACK:
				m_pStateKey = L"PRISONER_MODE_JUMP_ATTACK";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 3.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::PRISONER_FALL:
				m_pStateKey = L"PRISONER_MODE_JUMP_FALL";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 4.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::PRISONER_SKILL_A:
				m_pStateKey = L"PRISONER_MODE_SKILL";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 9.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::PRISONER_SKILL_B_CHARGE:
				m_pStateKey = L"PRISONER_MODE_SKILL2_CHARGE";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 7.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::PRISONER_SKILL_B_END:
				m_pStateKey = L"PRISONER_MODE_SKILL2_END";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 2.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::PRISONER_SKILL_B_SHOT:
				m_pStateKey = L"PRISONER_MODE_SKILL2_SHOT";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 4.f;
				m_fFrameSpeed = 1.2f;
				break;
			case CPlayer::PRISONER_SKILL_C:
				m_pStateKey = L"PRISONER_MODE_SKILL2_START";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 2.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::PRISONER_SWITCH:
				m_pStateKey = L"PRISONER_MODE_SWITCH";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 7.f;
				m_fFrameSpeed = 0.4f;
				break;
			case CPlayer::PRISONER_DEAD:
				break;
			case CPlayer::PRISONER_END:
				break;
			default:
				break;
			}
			m_ePrePrisonerState = m_eCurPrisonerState;
		}
		break;
	case CPlayer::TYPE_SAMURAI:
		if (m_ePreSamuraiState != m_eCurSamuraiState)
		{
			switch (m_eCurSamuraiState)
			{
			case CPlayer::SAMURAI_IDLE:
				m_pStateKey = L"SAMURAI_MODE_IDLE";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 6.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::SAMURAI_WALK:
				m_pStateKey = L"SAMURAI_MODE_WALK";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 6.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::SAMURAI_ATTACK_A:
				m_pStateKey = L"SAMURAI_MODE_ATTACK";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 6.f;
				m_fFrameSpeed = 1.5f;
				break;
			case CPlayer::SAMURAI_ATTACK_B:
				m_pStateKey = L"SAMURAI_MODE_ATTACK2";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 4.f;
				m_fFrameSpeed = 1.5f;
				break;
			case CPlayer::SAMURAI_ATTACK_C:
				m_pStateKey = L"SAMURAI_MODE_ATTACK3";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 4.f;
				m_fFrameSpeed = 1.5f;
				break;
			case CPlayer::SAMURAI_DASH:
				m_pStateKey = L"SAMURAI_MODE_DASH";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 5.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::SAMURAI_JUMP:
				m_pStateKey = L"SAMURAI_MODE_WALK";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 6.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::SAMURAI_JUMP_ATTACK:
				m_pStateKey = L"SAMURAI_MODE_JUMP";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 3.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::SAMURAI_DOUBLE_JUMP:
				m_pStateKey = L"SAMURAI_MODE_JUMP";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 3.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::SAMURAI_FALL:
				m_pStateKey = L"SAMURAI_MODE_FALL";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 5.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::SAMURAI_PARRY:
				m_pStateKey = L"SAMURAI_MODE_PARRY";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 3.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::SAMURAI_PARRY_ATTACK:
				m_pStateKey = L"SAMURAI_MODE_PARRY_ATTACK";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 3.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::SAMURAI_SKILL_A:
				m_pStateKey = L"SAMURAI_MODE_SKILL";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 7.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::SAMURAI_SKILL_ILSEOM:
				m_pStateKey = L"SAMURAI_MODE_ILSEOM";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 8.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::SAMURAI_SKILL_C:
				m_pStateKey = L"SAMURAI_MODE_SKILL3";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 4.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::SAMURAI_SWITCH:
				m_pStateKey = L"SAMURAI_MODE_SWITCH";
				m_tFrame.fStartFrame = 0.f;
				m_tFrame.fEndFrame = 4.f;
				m_fFrameSpeed = 1.f;
				break;
			case CPlayer::SAMURAI_DEAD:
				break;
			case CPlayer::SAMURAI_END:
				break;
			default:
				break;
			}
			m_ePreSamuraiState = m_eCurSamuraiState;
		}
		break;
	default:
		break;
	}
}

void CPlayer::Update_SkillCoolDown()
{
	if (m_bUse_SKILL_A)
	{
		m_tStat.fSKill_A_Cool -= CTime_Manager::Get_Instance()->Get_DeltaTime();
		if (m_bPickUpHead)
		{
			m_eCurType = TYPE_DEFAULT;
			m_pObjectKey = L"DEFAULT";
			m_pStateKey = L"DEFAULT_IDLE";
			m_eCurDefaultState = DEFAULT_IDLE;
			m_tFrame.fStartFrame = 0.f;
			m_tStat.fSKill_A_Cool = 0;
			CSoundManager::Get_Instance()->StopSound(CSoundManager::COOLDOWN_END);
			CSoundManager::Get_Instance()->PlaySound(L"Object_GearChange.wav", CSoundManager::COOLDOWN_END);
			m_bPickUpHead = false;
		}

		if (m_tStat.fSKill_A_Cool <= 0 && m_eCurType == TYPE_DEFAULT_NOHEAD )
		{
			m_eCurType = TYPE_DEFAULT;
			m_pObjectKey = L"DEFAULT";
			m_pStateKey = L"DEFAULT_IDLE";
			m_eCurDefaultState = DEFAULT_IDLE;
			m_tFrame.fStartFrame = 0.f;
			CSoundManager::Get_Instance()->StopSound(CSoundManager::COOLDOWN_END);
			CSoundManager::Get_Instance()->PlaySound(L"Object_GearChange.wav", CSoundManager::COOLDOWN_END);
			m_bPickUpHead = false;
			m_bUse_SKILL_A = false;
		}
		else if (m_tStat.fSKill_A_Cool <= 0 && m_eCurType != TYPE_DEFAULT_NOHEAD)
		{
			CSoundManager::Get_Instance()->StopSound(CSoundManager::COOLDOWN_END);
			CSoundManager::Get_Instance()->PlaySound(L"Object_GearChange.wav", CSoundManager::COOLDOWN_END);
			m_bUse_SKILL_A = false;
		}		
	}

	if (m_bUse_SKILL_S)
	{
		m_tStat.fSKill_S_Cool -= CTime_Manager::Get_Instance()->Get_DeltaTime();
		if (m_tStat.fSKill_S_Cool <= 0)
		{
			CSoundManager::Get_Instance()->StopSound(CSoundManager::COOLDOWN_END);
			CSoundManager::Get_Instance()->PlaySound(L"Object_GearChange.wav", CSoundManager::COOLDOWN_END);
			m_bUse_SKILL_S = false;
		}
	}

	if (m_bUse_SKILL_D)
	{
		m_tStat.fSKill_D_Cool -= CTime_Manager::Get_Instance()->Get_DeltaTime();
		if (m_tStat.fSKill_D_Cool <= 0)
		{
			CSoundManager::Get_Instance()->StopSound(CSoundManager::COOLDOWN_END);
			CSoundManager::Get_Instance()->PlaySound(L"Object_GearChange.wav", CSoundManager::COOLDOWN_END);
			m_bUse_SKILL_D = false;
		}
	}

	if (m_bUse_Swap)
	{
		m_tStat.fSwap_Cool -= CTime_Manager::Get_Instance()->Get_DeltaTime();
		if (m_tStat.fSwap_Cool <= 0)
		{
			CSoundManager::Get_Instance()->StopSound(CSoundManager::COOLDOWN_END);
			CSoundManager::Get_Instance()->PlaySound(L"Object_GearChange.wav", CSoundManager::COOLDOWN_END);
			m_bUse_Swap = false;
		}

	}
}

void CPlayer::OffSet()
{
	int	iOffSetX = WINCX >> 1;
	int iOffSetY = WINCY >> 1;

	if (iOffSetX < m_tInfo.vPos.x + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X))
	{
		CScroll_Manager::Set_ScrollPos({ iOffSetX - (m_tInfo.vPos.x + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X)),0.f, 0.f });
	}
	if (iOffSetX > m_tInfo.vPos.x + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X))
	{
		CScroll_Manager::Set_ScrollPos({ iOffSetX - (m_tInfo.vPos.x + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X)),0.f, 0.f });

	}
	if (iOffSetY < m_tInfo.vPos.y + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y))
	{
		CScroll_Manager::Set_ScrollPos({ 0.f, iOffSetY - (m_tInfo.vPos.y + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y)), 0.f });

	}
	if (iOffSetY > m_tInfo.vPos.y + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y))
	{
		CScroll_Manager::Set_ScrollPos({ 0.f, iOffSetY - (m_tInfo.vPos.y + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y)), 0.f });

	}
}

CPlayer* CPlayer::Create()
{
	CPlayer* pInstance = new CPlayer;
	if (FAILED(pInstance->Ready_GameObject()))
	{
		MessageBox(nullptr, L"Create Failed - Player.cpp", L"SystemError", MB_OK);
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;

}

void CPlayer::Free()
{
	delete this;
}

void CPlayer::STATE_GETBONE()
{
	if (m_eCurType == TYPE_DEFAULT)
		return;


	else if (CEventManager::Get_Instance()->Get_EventCnt() >= 2 &&
		CEventManager::Get_Instance()->Get_DialogueCount() >= 7)
	{
		m_eCurDefaultNoboneState = DEFAULT_NOBONE_GETBONE;

		if (m_eCurDefaultNoboneState == DEFAULT_NOBONE_GETBONE &&
			m_tFrame.fStartFrame >= 5.f)
		{
			m_bGetBone = true;
		}

		if (m_eCurDefaultNoboneState == DEFAULT_NOBONE_GETBONE &&
			m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f && m_bGetBone)
		{
			m_eCurType = TYPE_DEFAULT;
		}
	}
}

void CPlayer::STATE_WALK()
{
	if (m_bSwap && m_eCurType == TYPE_PRISONER | m_eCurType == TYPE_SAMURAI)
		return;

	if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_LEFT) && !m_bDash)
	{
		if (m_tInfo.vPos.x < 50.)
		{
			m_tInfo.vPos.x = 50.f;
		}
		else
		{
			m_tInfo.vPos.x += -1.f * CTime_Manager::Get_Instance()->Get_DeltaTime() * m_fSpeed;
		}

		if (!m_bSwap)
		{
			switch (m_eCurType)
			{
			case CPlayer::TYPE_DEFAULT:
				m_eCurDefaultState = DEFAULT_WALK;
				break;
			case CPlayer::TYPE_DEFAULT_NOBONE:
				m_eCurDefaultNoboneState = DEFAULT_NOBONE_WALK;
				break;
			case CPlayer::TYPE_DEFAULT_NOHEAD:
				m_eCurDefaultNoHeadState = DEFAULT_NOHEAD_WALK;
				break;
			case CPlayer::TYPE_PRISONER:
				m_eCurPrisonerState = PRISONER_WALK;
				break;
			case CPlayer::TYPE_SAMURAI:
				m_eCurSamuraiState = SAMURAI_WALK;
				break;
			}
		}
		m_bLeftRight = true;
	}
	else if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_RIGHT) && !m_bDash)
	{
		switch (CScene_Manager::Get_Instance()->Get_Scene())
		{
		case CScene_Manager::SCENE_STAGE7:
			if (m_tInfo.vPos.x > 1021.f)
			{
				m_tInfo.vPos.x = 1021.f;
			}
			break;
		default:
			if (m_tInfo.vPos.x > (TILECX * TILEX) - 50)
			{
				m_tInfo.vPos.x = (TILECX * TILEX) - 50;
			}
			break;
		}

		m_tInfo.vPos.x += 1.f * CTime_Manager::Get_Instance()->Get_DeltaTime() * m_fSpeed;

		if (!m_bSwap)
		{
			m_bWalk = true;
			switch (m_eCurType)
			{
			case CPlayer::TYPE_DEFAULT:
				m_eCurDefaultState = DEFAULT_WALK;
				break;
			case CPlayer::TYPE_DEFAULT_NOBONE:
				m_eCurDefaultNoboneState = DEFAULT_NOBONE_WALK;
				break;
			case CPlayer::TYPE_DEFAULT_NOHEAD:
				m_eCurDefaultNoHeadState = DEFAULT_NOHEAD_WALK;

				break;
			case CPlayer::TYPE_PRISONER:
				m_eCurPrisonerState = PRISONER_WALK;
				break;
			case CPlayer::TYPE_SAMURAI:
				m_eCurSamuraiState = SAMURAI_WALK;
				break;
			}
		}
		m_bLeftRight = false;
	}
	else
	{
		if (!m_bSwap && !m_bDash && !m_bStartAttack)
		{
			switch (m_eCurType)
			{
				m_bWalk = false;
			case CPlayer::TYPE_DEFAULT:
				if (!m_bJump && m_eCurDefaultState != DEFAULT_ATTACK_A && m_eCurDefaultState != DEFAULT_ATTACK_B && !m_bDash)
				{
					m_eCurDefaultState = DEFAULT_IDLE;
				}
				break;
			case CPlayer::TYPE_DEFAULT_NOBONE:
				m_eCurDefaultNoboneState = DEFAULT_NOBONE_IDLE;
				break;
			case CPlayer::TYPE_DEFAULT_NOHEAD:
				m_eCurDefaultNoHeadState = DEFAULT_NOHEAD_IDLE;
				break;
			case CPlayer::TYPE_PRISONER:
				if (!m_bJump && !m_bDash && !m_bSwap)
				{
					m_eCurPrisonerState = PRISONER_IDLE;
				}
				break;
			case CPlayer::TYPE_SAMURAI:
				m_eCurSamuraiState = SAMURAI_IDLE;
				break;
			}
		}
	}

	if (CKey_Manager::Get_Instance()->Key_Pressing(VK_DOWN) && CKey_Manager::Get_Instance()->Key_Down(KEY_C))
	{
		m_tInfo.vPos.y += 100;
		m_bGround = false;
		switch (m_eCurType)
		{
		case CPlayer::TYPE_DEFAULT:
			m_eCurDefaultState = DEFAULT_FALL;
			break;
		case CPlayer::TYPE_DEFAULT_NOBONE:
			m_eCurDefaultNoboneState = DEFAULT_NOBONE_FALL;
			break;
		case CPlayer::TYPE_DEFAULT_NOHEAD:
			m_eCurDefaultNoHeadState = DEFAULT_NOHEAD_FALL;
			break;
		case CPlayer::TYPE_PRISONER:
			m_eCurPrisonerState = PRISONER_FALL;
			break;
		case CPlayer::TYPE_SAMURAI:
			m_eCurSamuraiState = SAMURAI_FALL;
			break;
		}
	}

	if (CKey_Manager::Get_Instance()->Key_Down(KEY_C) && CEventManager::Get_Instance()->Get_EventCnt() >= 1)
	{
		m_bCheckCol = true;
		if (!m_bJump && !m_bSwap)
		{
			CSoundManager::Get_Instance()->StopSound(CSoundManager::JUMP);
			CSoundManager::Get_Instance()->PlaySound(L"Default_Jump.wav", CSoundManager::JUMP);
			switch (m_eCurType)
			{
			case CPlayer::TYPE_DEFAULT:
				m_eCurDefaultState = DEFAULT_JUMP;
				break;
			case CPlayer::TYPE_DEFAULT_NOBONE:
				m_eCurDefaultNoboneState = DEFAULT_NOBONE_JUMP;
				break;
			case CPlayer::TYPE_DEFAULT_NOHEAD:
				m_eCurDefaultNoHeadState = DEFAULT_NOHEAD_JUMP;
				break;
			case CPlayer::TYPE_PRISONER:
				m_eCurPrisonerState = PRISONER_JUMP;
				break;
			case CPlayer::TYPE_SAMURAI:
				m_eCurSamuraiState = SAMURAI_JUMP;
				break;
			}
			m_bJump = true;
			m_fJumpY = m_tInfo.vPos.y;
		}
		if (m_iJumpCnt < 2)
			m_iJumpCnt++;
	}
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_X) && !m_bDash && !m_bSwap && CEventManager::Get_Instance()->Get_EventCnt() >= 3)
	{

		CSoundManager::Get_Instance()->StopSound(CSoundManager::DASH);
		CSoundManager::Get_Instance()->PlaySound(L"Default_Dash.wav", CSoundManager::DASH);

		switch (m_eCurType)
		{
		case CPlayer::TYPE_DEFAULT:
			m_eCurDefaultState = DEFAULT_DASH;
			break;
		case CPlayer::TYPE_DEFAULT_NOBONE:
			m_eCurDefaultNoboneState = DEFAULT_NOBONE_DASH;
			break;
		case CPlayer::TYPE_DEFAULT_NOHEAD:
			m_eCurDefaultNoHeadState = DEFAULT_NOHEAD_DASH;
			break;
		case CPlayer::TYPE_PRISONER:
			m_eCurPrisonerState = PRISONER_DASH;
			break;
		case CPlayer::TYPE_SAMURAI:
			m_eCurSamuraiState = SAMURAI_DASH;
			break;
		}
		m_bDash = true;
	}


	if (CKey_Manager::Get_Instance()->Key_Down(KEY_SPACE) && m_eSaveType != TYPE_END && !m_bUse_Swap)
	{
		if (m_eCurDefaultState == DEFAULT_JUMP || m_eCurDefaultState == DEFAULT_FALL ||
			m_eCurPrisonerState == PRISONER_JUMP || m_eCurPrisonerState == PRISONER_FALL)
			return;

		m_tStat.fSwap_MaxCool = 5.f;
		m_tStat.fSwap_Cool = m_tStat.fSwap_MaxCool;

		CGameObject* pObj = CAbstractFactory<CSwapCoolDown>::Create(50.f, 553.f);
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);

		PlayerType eTempType;
		eTempType = m_eCurType;
		m_eCurType = m_eSaveType;
		m_eSaveType = eTempType;
		m_bUse_Swap = true;

	}

	else if (m_bGround && !m_bWalk)
	{
		switch (m_eCurType)
		{
		case CPlayer::TYPE_DEFAULT:
			m_eCurDefaultState = DEFAULT_IDLE;
			break;
		case CPlayer::TYPE_DEFAULT_NOBONE:
			m_eCurDefaultNoboneState = DEFAULT_NOBONE_IDLE;
			break;
		case CPlayer::TYPE_DEFAULT_NOHEAD:
			break;
		case CPlayer::TYPE_PRISONER:
			m_eCurPrisonerState = PRISONER_IDLE;
			break;
		case CPlayer::TYPE_SAMURAI:
			m_eCurSamuraiState = SAMURAI_IDLE;
			break;
		default:
			break;
		}
	}
}

void CPlayer::STATE_ATTACK()
{
	CGameObject* pObj = nullptr;

	switch (m_eCurType)
	{
	case CPlayer::TYPE_DEFAULT:
		if (CKey_Manager::Get_Instance()->Key_Down(KEY_Z) && CEventManager::Get_Instance()->Get_EventCnt() >= 2)
		{
			m_eCurDefaultState = DEFAULT_ATTACK_A;
			m_bStartAttack = true;

			if (m_tFrame.fStartFrame >= 1.7f && m_bStartAttack)
			{
				m_iComboCnt++;

				if (m_iColCnt > 0)
					m_iColCnt = 0;

				switch (m_eCurDefaultState)
				{
				case CPlayer::DEFAULT_ATTACK_A:
					CSoundManager::Get_Instance()->StopSound(CSoundManager::DEFAULT_ATTACK_0_SFX);
					CSoundManager::Get_Instance()->PlaySound(L"Skul_Atk_1.wav", CSoundManager::DEFAULT_ATTACK_0_SFX);
					break;
				case CPlayer::DEFAULT_ATTACK_B:
					CSoundManager::Get_Instance()->StopSound(CSoundManager::DEFAULT_ATTACK_1_SFX);
					CSoundManager::Get_Instance()->PlaySound(L"Skul_Atk_2.wav", CSoundManager::DEFAULT_ATTACK_1_SFX);
					break;
				default:
					break;
				}
			}
			switch (m_iComboCnt)
			{
			case 0:
				m_eCurDefaultState = DEFAULT_ATTACK_A;
				break;
			case 1:
				m_eCurDefaultState = DEFAULT_ATTACK_B;
				break;
			default:
				m_iComboCnt = 0;
				break;
			}

		}

		else if (m_bStartAttack && (int)m_tFrame.fStartFrame >= 1.f && m_iColCnt == 0)
		{

			if (m_bLeftRight)
			{
				pObj = CAbstractFactory<CCollision>::Create(m_tInfo.vPos.x - 5, m_tInfo.vPos.y);
				pObj->Set_ColScale(32, 32);
				pObj->Set_OBJID(OBJ_ID::PLAYER_ATTACK);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::PLAYER_ATTACK, pObj);
			}
			else
			{
				pObj = CAbstractFactory<CCollision>::Create(m_tInfo.vPos.x + 5, m_tInfo.vPos.y);
				pObj->Set_ColScale(32, 32);
				pObj->Set_OBJID(OBJ_ID::PLAYER_ATTACK);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::PLAYER_ATTACK, pObj);
			}
			m_iColCnt++;

		}
		else if (m_bStartAttack && m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
		{
			m_eCurDefaultState = DEFAULT_IDLE;
			m_iComboCnt = 0;
			m_bStartAttack = false;
		}
		break;
	case CPlayer::TYPE_DEFAULT_NOBONE:
		break;
	case CPlayer::TYPE_DEFAULT_NOHEAD:
		if (CKey_Manager::Get_Instance()->Key_Down(KEY_Z) && CEventManager::Get_Instance()->Get_EventCnt() >= 2)
		{
			m_eCurDefaultNoHeadState = DEFAULT_NOHEAD_ATTACK_A;
			m_bStartAttack = true;

			if (m_tFrame.fStartFrame >= 1.7f && m_bStartAttack)
			{
				m_iComboCnt++;

				if (m_iColCnt > 0)
					m_iColCnt = 0;

				switch (m_eCurDefaultNoHeadState)
				{
				case CPlayer::DEFAULT_NOHEAD_ATTACK_A:
					CSoundManager::Get_Instance()->StopSound(CSoundManager::DEFAULT_ATTACK_0_SFX);
					CSoundManager::Get_Instance()->PlaySound(L"Skul_Atk_1.wav", CSoundManager::DEFAULT_ATTACK_0_SFX);
					break;
				case CPlayer::DEFAULT_NOHEAD_ATTACK_B:
					CSoundManager::Get_Instance()->StopSound(CSoundManager::DEFAULT_ATTACK_1_SFX);
					CSoundManager::Get_Instance()->PlaySound(L"Skul_Atk_2.wav", CSoundManager::DEFAULT_ATTACK_1_SFX);
					break;
				default:
					break;
				}

			}
			switch (m_iComboCnt)
			{
			case 0:
				m_eCurDefaultNoHeadState = DEFAULT_NOHEAD_ATTACK_A;
				break;
			case 1:
				m_eCurDefaultNoHeadState = DEFAULT_NOHEAD_ATTACK_B;
				break;
			default:
				m_iComboCnt = 0;
				break;
			}
		}
		else if (m_bStartAttack && (int)m_tFrame.fStartFrame >= 1.f && m_iColCnt == 0)
		{

			if (m_bLeftRight)
			{
				pObj = CAbstractFactory<CCollision>::Create(m_tInfo.vPos.x - 5, m_tInfo.vPos.y);
				pObj->Set_ColScale(32, 32);
				pObj->Set_OBJID(OBJ_ID::PLAYER_ATTACK);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::PLAYER_ATTACK, pObj);
			}
			else
			{
				pObj = CAbstractFactory<CCollision>::Create(m_tInfo.vPos.x + 5, m_tInfo.vPos.y);
				pObj->Set_ColScale(32, 32);
				pObj->Set_OBJID(OBJ_ID::PLAYER_ATTACK);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::PLAYER_ATTACK, pObj);
			}
			m_iColCnt++;

		}
		else if (m_bStartAttack && m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
		{
			m_eCurDefaultState = DEFAULT_IDLE;
			m_iComboCnt = 0;
			m_bStartAttack = false;
		}
		break;
	case CPlayer::TYPE_PRISONER:
		if (CKey_Manager::Get_Instance()->Key_Down(KEY_Z) && CEventManager::Get_Instance()->Get_EventCnt() >= 2)
		{
			m_bStartAttack = true;
			if (m_tFrame.fStartFrame >= 1.5f)
			{
				m_iComboCnt++;

				if (m_iColCnt > 0)
					m_iColCnt = 0;
			}
			switch (m_iComboCnt)
			{
			case 0:
				m_eCurPrisonerState = PRISONER_ATTACK_A;

				break;
			case 1:
				m_eCurPrisonerState = PRISONER_ATTACK_B;

				break;
			case 2:
				m_eCurPrisonerState = PRISONER_ATTACK_C;

				break;
			case 3:
				m_eCurPrisonerState = PRISONER_ATTACK_D;

				break;
			case 4:
				m_eCurPrisonerState = PRISONER_ATTACK_E;

				break;
			default:
				m_iComboCnt = 0;
				break;
			}

			if (m_bLeftRight)
				m_tInfo.vPos.x -= 1.f * CTime_Manager::Get_Instance()->Get_DeltaTime() * m_fSpeed;
			else
				m_tInfo.vPos.x += 1.f * CTime_Manager::Get_Instance()->Get_DeltaTime() * m_fSpeed;
		}

		else if (m_bStartAttack && (int)m_tFrame.fStartFrame >= 0.7f && m_iColCnt == 0)
		{
			switch (m_eCurPrisonerState)
			{
			case CPlayer::PRISONER_ATTACK_A:
				CSoundManager::Get_Instance()->StopSound(CSoundManager::PRISONER_ATTACK_0);
				CSoundManager::Get_Instance()->PlaySound(L"Atk_Sword_Small_1.wav", CSoundManager::PRISONER_ATTACK_0);
				break;
			case CPlayer::PRISONER_ATTACK_B:
				CSoundManager::Get_Instance()->StopSound(CSoundManager::PRISONER_ATTACK_1);
				CSoundManager::Get_Instance()->PlaySound(L"Atk_Sword_Small_2.wav", CSoundManager::PRISONER_ATTACK_1);
				break;
			case CPlayer::PRISONER_ATTACK_C:
				CSoundManager::Get_Instance()->StopSound(CSoundManager::PRISONER_ATTACK_2);
				CSoundManager::Get_Instance()->PlaySound(L"Atk_Sword_Small_3.wav", CSoundManager::PRISONER_ATTACK_2);
				break;
			case CPlayer::PRISONER_ATTACK_D:
				CSoundManager::Get_Instance()->StopSound(CSoundManager::PRISONER_ATTACK_3);
				CSoundManager::Get_Instance()->PlaySound(L"Atk_Sword_Small_4.wav", CSoundManager::PRISONER_ATTACK_3);
				break;
			case CPlayer::PRISONER_ATTACK_E:
				CSoundManager::Get_Instance()->StopSound(CSoundManager::PRISONER_ATTACK_4);
				CSoundManager::Get_Instance()->PlaySound(L"Atk_Sword_Small_5.wav", CSoundManager::PRISONER_ATTACK_4);
				break;
			default:
				break;
			}

			if (m_bLeftRight)
			{
				pObj = CAbstractFactory<CCollision>::Create(m_tInfo.vPos.x - 15, m_tInfo.vPos.y);
				pObj->Set_ColScale(32, 32);
				pObj->Set_OBJID(OBJ_ID::PLAYER_ATTACK);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::PLAYER_ATTACK, pObj);
			}
			else
			{
				pObj = CAbstractFactory<CCollision>::Create(m_tInfo.vPos.x + 15, m_tInfo.vPos.y);
				pObj->Set_ColScale(32, 32);
				pObj->Set_OBJID(OBJ_ID::PLAYER_ATTACK);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::PLAYER_ATTACK, pObj);
			}
			m_iColCnt++;
		}

		else if (m_bStartAttack && m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
		{
			m_eCurPrisonerState = PRISONER_IDLE;
			m_iComboCnt = 0;
			m_bStartAttack = false;
		}
		break;
	case CPlayer::TYPE_SAMURAI:
		if (CKey_Manager::Get_Instance()->Key_Down(KEY_Z) && CEventManager::Get_Instance()->Get_EventCnt() >= 2)
		{
			m_bStartAttack = true;
			if (m_tFrame.fStartFrame >= 1.5f)
			{
				m_iComboCnt++;

				if (m_iColCnt > 0)
					m_iColCnt = 0;
			}

			switch (m_iComboCnt)
			{
			case 0:
				m_eCurSamuraiState = SAMURAI_ATTACK_A;
				break;
			case 1:
				m_eCurSamuraiState = SAMURAI_ATTACK_B;
				break;
			case 2:
				m_eCurSamuraiState = SAMURAI_ATTACK_C;
				break;
			default:
				m_iComboCnt = 0;
				break;
			}


			if (m_bLeftRight)
				m_tInfo.vPos.x -= 1.f * CTime_Manager::Get_Instance()->Get_DeltaTime() * m_fSpeed;
			else
				m_tInfo.vPos.x += 1.f * CTime_Manager::Get_Instance()->Get_DeltaTime() * m_fSpeed;

		}


		else if (m_bStartAttack && (int)m_tFrame.fStartFrame >= 1.f && m_iColCnt == 0)
		{
			switch (m_eCurSamuraiState)
			{
			case CPlayer::SAMURAI_ATTACK_A:
				CSoundManager::Get_Instance()->StopSound(CSoundManager::SAMURAI_ATTACK_0_SFX);
				CSoundManager::Get_Instance()->PlaySound(L"Atk_Sword_Small_3.wav", CSoundManager::SAMURAI_ATTACK_0_SFX);
				break;
			case CPlayer::SAMURAI_ATTACK_B:
				CSoundManager::Get_Instance()->StopSound(CSoundManager::SAMURAI_ATTACK_1_SFX);
				CSoundManager::Get_Instance()->PlaySound(L"Atk_Sword_Small_4.wav", CSoundManager::SAMURAI_ATTACK_1_SFX);
				break;
			case CPlayer::SAMURAI_ATTACK_C:
				CSoundManager::Get_Instance()->StopSound(CSoundManager::SAMURAI_ATTACK_2_SFX);
				CSoundManager::Get_Instance()->PlaySound(L"Atk_Sword_Small_5.wav", CSoundManager::SAMURAI_ATTACK_2_SFX);
				break;
			}

			if (m_bLeftRight)
			{
				pObj = CAbstractFactory<CCollision>::Create(m_tInfo.vPos.x - 15, m_tInfo.vPos.y);
				pObj->Set_ColScale(32, 32);
				pObj->Set_OBJID(OBJ_ID::PLAYER_ATTACK);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::PLAYER_ATTACK, pObj);
			}
			else
			{
				pObj = CAbstractFactory<CCollision>::Create(m_tInfo.vPos.x + 15, m_tInfo.vPos.y);
				pObj->Set_ColScale(32, 32);
				pObj->Set_OBJID(OBJ_ID::PLAYER_ATTACK);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::PLAYER_ATTACK, pObj);
			}
			m_iColCnt++;
		}
		else if (m_bStartAttack && m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
		{
			m_eCurSamuraiState = SAMURAI_IDLE;
			m_iComboCnt = 0;
			m_bStartAttack = false;
		}
		break;
	case CPlayer::TYPE_END:
		break;
	default:
		break;
	}
}

void CPlayer::STATE_JUMP()
{
	if (m_bJump && m_iComboCnt < 3)
	{
		m_bGround = false;
		if (!m_bDoubleJump)
			m_tInfo.vPos.y = m_fJumpY - (m_fJumpForce * m_fJumpTime - m_fGravity * m_fJumpTime * m_fJumpTime * 0.5f);

		if (m_iJumpCnt > 1 && m_iJumpCnt < 3)
		{
			if (!m_bDoubleJump)
			{
				CSoundManager::Get_Instance()->StopSound(CSoundManager::DOUBLE_JUMP);
				CSoundManager::Get_Instance()->PlaySound(L"Default_Jump_Air.wav", CSoundManager::DOUBLE_JUMP);
				m_fJumpTime = 0.f;
				m_fJumpY = m_tInfo.vPos.y;
				CGameObject* pObj = CAbstractFactory<CDoubleJumpEffect>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);
				switch (m_eCurType)
				{
				case CPlayer::TYPE_DEFAULT:
					m_eCurDefaultState = DEFAULT_DOUBLEJUMP;
					break;
				case CPlayer::TYPE_DEFAULT_NOBONE:
					break;
				case CPlayer::TYPE_DEFAULT_NOHEAD:
					break;
				case CPlayer::TYPE_PRISONER:
					m_eCurPrisonerState = PRISONER_DOUBLE_JUMP;
					break;
				case CPlayer::TYPE_SAMURAI:
					m_eCurSamuraiState = SAMURAI_DOUBLE_JUMP;
					break;
				case CPlayer::TYPE_END:
					break;
				default:
					break;
				}
				m_bDoubleJump = true;
			}
			m_tInfo.vPos.y = m_fJumpY - (m_fJumpForce * m_fJumpTime - m_fGravity * m_fJumpTime * m_fJumpTime * 0.5f);
		}
		if (CKey_Manager::Get_Instance()->Key_Down(KEY_X) && !m_bDash)
		{
			switch (m_eCurType)
			{
			case CPlayer::TYPE_DEFAULT:
				m_eCurDefaultState = DEFAULT_DASH;
				break;
			case CPlayer::TYPE_DEFAULT_NOBONE:
				m_eCurDefaultNoboneState = DEFAULT_NOBONE_DASH;
				break;
			case CPlayer::TYPE_DEFAULT_NOHEAD:
				m_eCurDefaultNoHeadState = DEFAULT_NOHEAD_DASH;
				break;
			case CPlayer::TYPE_PRISONER:
				m_eCurPrisonerState = PRISONER_DASH;
				break;
			case CPlayer::TYPE_SAMURAI:
				m_eCurSamuraiState = SAMURAI_DASH;
				break;
			}
			m_bDash = true;
		}
		else if (m_bJump && !m_bDash && !m_bGround && !m_bDoubleJump || m_bJump && !m_bDash && !m_bGround && m_bDoubleJump)
		{
			m_fJumpTime += 0.15f;
			switch (m_eCurType)
			{
			case CPlayer::TYPE_DEFAULT:
				m_eCurDefaultState = DEFAULT_FALL;
				break;
			case CPlayer::TYPE_DEFAULT_NOBONE:
				m_eCurDefaultNoboneState = DEFAULT_NOBONE_FALL;
				break;
			case CPlayer::TYPE_DEFAULT_NOHEAD:
				m_eCurDefaultNoHeadState = DEFAULT_NOHEAD_FALL;
				break;
			case CPlayer::TYPE_PRISONER:
				m_eCurPrisonerState = PRISONER_FALL;
				break;
			case CPlayer::TYPE_SAMURAI:
				m_eCurSamuraiState = SAMURAI_FALL;
				break;
			}
		}
	}

	else if (!m_bGround && !m_bJump)
	{
		switch (m_eCurType)
		{
		case CPlayer::TYPE_DEFAULT:
			m_eCurDefaultState = DEFAULT_FALL;
			break;
		case CPlayer::TYPE_DEFAULT_NOBONE:
			m_eCurDefaultNoboneState = DEFAULT_NOBONE_FALL;
			break;
		case CPlayer::TYPE_DEFAULT_NOHEAD:
			break;
		case CPlayer::TYPE_PRISONER:
			m_eCurPrisonerState = PRISONER_FALL;
			break;
		case CPlayer::TYPE_SAMURAI:
			m_eCurSamuraiState = SAMURAI_FALL;
			break;
		default:
			break;
		}
		m_tInfo.vPos.y += m_fJumpTime * CTime_Manager::Get_Instance()->Get_DeltaTime() * m_fJumpForce;
		m_fJumpTime += 0.25f;
	}

}

void CPlayer::STATE_DASH()
{
	if (m_bDash)
	{
		CGameObject* pObj = CAbstractFactory<CGhost_Effect>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::GHOST_EFFECT, pObj);

		if (!m_bStartDashEffect)
		{
			pObj = CAbstractFactory<CDashEffect>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);
			m_bStartDashEffect = true;
		}

		if (m_bLeftRight) // 왼쪽을 보고있다.
		{
			m_fDashTime += 0.25f;
			m_tInfo.vPos.x -= m_fDashTime * CTime_Manager::Get_Instance()->Get_DeltaTime() * m_fDashSpeed;

			if (m_fDashTime >= 5.f)
			{
				m_bDash = false;
				m_fDashTime = 0.f;
				m_bStartDashEffect = false;
			}

		}
		else
		{

			m_fDashTime += 0.25f;
			m_tInfo.vPos.x += m_fDashTime * CTime_Manager::Get_Instance()->Get_DeltaTime() * m_fDashSpeed;

			if (m_fDashTime >= 5.f)
			{
				m_bDash = false;
				m_fDashTime = 0.f;
				m_bStartDashEffect = false;
			}
		}
	}
}

void CPlayer::STATE_SWAP()
{
	if (m_bSwap && m_eCurPrisonerState == PRISONER_SWITCH && m_tFrame.fStartFrame >= 1.f)
	{
		m_fFrameSpeed = 1.f;

		if (m_bSwap && m_eCurPrisonerState == PRISONER_SWITCH && (int)m_tFrame.fStartFrame >= 1)
		{
			if (!m_bSpawnEffect)
			{
				CGameObject* pObj = CAbstractFactory<CPrisonerSwap_Effect>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::PRISONER_SWAP_EFFECT, pObj);
				CSoundManager::Get_Instance()->StopSound(CSoundManager::PRISONER_SAWP);
				CSoundManager::Get_Instance()->PlaySound(L"explode.wav", CSoundManager::PRISONER_SAWP);
				m_bSpawnEffect = true;
			}
			else if (m_bSwap && m_eCurPrisonerState == PRISONER_SWITCH && m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
			{
				m_bSwap = false;
				m_bSpawnEffect = false;
			}
		}
	}
	if (m_bSwap && m_eCurDefaultState == DEFAULT_SWITCH && m_tFrame.fStartFrame >= 1.f)
	{
		if (m_bSwap && m_eCurDefaultState == DEFAULT_SWITCH && m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
		{
			m_bSwap = false;
		}
	}
	if (m_bSwap && m_eCurSamuraiState == SAMURAI_SWITCH && m_tFrame.fStartFrame >= 1.f)
	{
		if (m_bSwap && m_eCurSamuraiState == SAMURAI_SWITCH && m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
		{
			m_bSwap = false;
		}
	}

}

void CPlayer::STATE_SKILL_A()
{
	switch (m_eCurType)
	{
	case CPlayer::TYPE_DEFAULT:
		if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_A) && !m_bPlayingAnim && !m_bUse_SKILL_A)
		{
			m_tStat.fSKill_A_MaxCool = 5.f;
			m_tStat.fSKill_A_Cool = m_tStat.fSKill_A_MaxCool;
			m_iSkillCnt = 0;
			m_bUse_SKILL_A = true;
			m_eCurType = TYPE_DEFAULT_NOHEAD;
			m_pObjectKey = L"DEFAULT_NOHEAD";
			m_pStateKey = L"DEFAULT_NOHEAD_IDLE";
			m_eCurDefaultNoHeadState = DEFAULT_NOHEAD_IDLE;
			CGameObject* pObj = CAbstractFactory<CSkillCoolDown>::Create(97.f, 552.f);
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);

			if (m_bLeftRight)
			{
				pObj = CAbstractFactory<CSkulHead>::Create(m_tInfo.vPos.x - 50, m_tInfo.vPos.y);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::SKUL_HEAD, pObj);
			}
			else
			{
				pObj = CAbstractFactory<CSkulHead>::Create(m_tInfo.vPos.x + 50, m_tInfo.vPos.y);
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::SKUL_HEAD, pObj);
			}
		}
		break;
	case CPlayer::TYPE_PRISONER:
		if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_A) && !m_bPlayingAnim && !m_bUse_SKILL_A)
		{
			CSoundManager::Get_Instance()->StopSound(CSoundManager::ICECROSSBOW_READY_SFX);
			CSoundManager::Get_Instance()->PlaySound(L"weapon_icecrossbow_charge.wav", CSoundManager::ICECROSSBOW_READY_SFX);
			m_eCurPrisonerState = PRISONER_SKILL_B_CHARGE;
			m_bPlayingAnim = true;

			m_tStat.fSKill_A_MaxCool = 3.f;
			m_tStat.fSKill_A_Cool = m_tStat.fSKill_A_MaxCool;
			m_iSkillCnt = 0;
			m_bUse_SKILL_A = true;

			CGameObject* pObj = CAbstractFactory<CSkillCoolDown>::Create(97.f, 552.f);
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);
		}
		if (CKey_Manager::Get_Instance()->Key_Up(KEY_A) && m_bUse_SKILL_A && m_bPlayingAnim)
		{
			m_eCurPrisonerState = PRISONER_SKILL_B_SHOT;

			if (!m_bSkill_B_Shot)
			{
				CGameObject* pObj = nullptr;
				if (m_bLeftRight)
				{
					pObj = CAbstractFactory<CIceCrossShotEffect>::Create(m_tInfo.vPos.x - 30.f, m_tInfo.vPos.y);
					CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);
				}
				else
				{
					pObj = CAbstractFactory<CIceCrossShotEffect>::Create(m_tInfo.vPos.x + 30.f, m_tInfo.vPos.y);
					CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EFFECT, pObj);
				}

				CSoundManager::Get_Instance()->StopSound(CSoundManager::ICECROSSBOW_SHOT_SFX);
				CSoundManager::Get_Instance()->PlaySound(L"weapon_icecrossbow_release2.wav", CSoundManager::ICECROSSBOW_SHOT_SFX);
				for (int i = 0; i < 4; ++i)
				{
					pObj = CAbstractFactory<CIceCrossAllow>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y, m_fRotation);
					pObj->Set_OBJID(OBJ_ID::ICE_ALLOW);
					CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::ICE_ALLOW, pObj);
					m_fRotation += 10.f;
				}
				m_fRotation = 345.f;

				m_bSkill_B_Shot = true;
			}
		}

		if (m_eCurPrisonerState == PRISONER_SKILL_B_SHOT && m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
		{
			m_eCurPrisonerState = PRISONER_IDLE;
			m_bPlayingAnim = false;
			m_bSkill_B_Shot = false;
		}
		break;
	case CPlayer::TYPE_SAMURAI:
		if (CKey_Manager::Get_Instance()->Key_Down(KEY_A) && !m_bILSEOM_Start && !m_bILSEOM_End && !m_bHit_ILSeom && !m_bUse_SKILL_A)
		{
			m_bILSEOM_Start = true;
			m_bPlayingAnim = true;
			m_tStat.fSKill_A_MaxCool = 3.f;
			m_tStat.fSKill_A_Cool = m_tStat.fSKill_A_MaxCool;
			m_iSkillCnt = 0;
			m_bUse_SKILL_A = true;
			CGameObject* pObj = CAbstractFactory<CSkillCoolDown>::Create(97.f, 552.f);
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);

			m_eCurSamuraiState = SAMURAI_SKILL_ILSEOM;
		}

		if (m_eCurSamuraiState == SAMURAI_SKILL_ILSEOM)
		{
			if (m_tFrame.fStartFrame >= 0.f && m_bILSEOM_Start)
			{
				m_fFrameSpeed = 0.f;
				m_fILSEOM_Delay += CTime_Manager::Get_Instance()->Get_DeltaTime();
				if (m_fILSEOM_Delay >= 0.5f && m_bILSEOM_Start)
				{
					if (m_bLeftRight)
					{
						CGameObject* pObj = CAbstractFactory<CCollision>::Create(m_tInfo.vPos.x - 100, m_tInfo.vPos.y);
						pObj->Set_ColScale(300, 100);
						pObj->Set_OBJID(OBJ_ID::ILSEOM);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::ILSEOM, pObj);
						m_tInfo.vPos.x -= 100.f * CTime_Manager::Get_Instance()->Get_DeltaTime() * 300.f;
					}
					else
					{
						CGameObject* pObj = CAbstractFactory<CCollision>::Create(m_tInfo.vPos.x + 100, m_tInfo.vPos.y);
						pObj->Set_ColScale(300, 100);
						pObj->Set_OBJID(OBJ_ID::ILSEOM);
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::ILSEOM, pObj);
						m_tInfo.vPos.x += 100.f * CTime_Manager::Get_Instance()->Get_DeltaTime() * 300.f;
					}
					m_fFrameSpeed = 1.f;
					CSoundManager::Get_Instance()->StopSound(CSoundManager::SAMURAI_ILSEOM_START);
					CSoundManager::Get_Instance()->PlaySound(L"Samurai_IlSeomStart.wav", CSoundManager::SAMURAI_ILSEOM_START);
					m_bILSEOM_Start = false;
					m_fILSEOM_Delay = 0.f;
				}
			}
			if (m_tFrame.fStartFrame >= 4.f)
			{
				m_fFrameSpeed = 0.f;
				m_fILSEOM_Delay += CTime_Manager::Get_Instance()->Get_DeltaTime();
				if (m_fILSEOM_Delay >= 0.5f)
				{
					m_bILSEOM_End = true;
					m_fFrameSpeed = 1.f;
				}
			}
			if (m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
			{
				m_eCurSamuraiState = SAMURAI_IDLE;
				m_fILSEOM_Delay = 0.f;
				m_bILSEOM_End = false;
				m_bPlayingAnim = false;
			}
		}

		break;
	default:
		break;
	}
}

void CPlayer::STATE_SKILL_S()
{
	if (m_bJump)
		return;

	switch (m_eCurType)
	{
	case CPlayer::TYPE_DEFAULT:
		break;
	case CPlayer::TYPE_DEFAULT_NOBONE:
		break;
	case CPlayer::TYPE_DEFAULT_NOHEAD:
		break;
	case CPlayer::TYPE_PRISONER:
		if (CKey_Manager::Get_Instance()->Key_Down(KEY_S) && !m_bUse_SKILL_S)
		{
			m_tStat.fSKill_S_MaxCool = 4.f;
			m_tStat.fSKill_S_Cool = m_tStat.fSKill_S_MaxCool;
			m_iSkillCnt = 1;
			m_bUse_SKILL_S = true;
			m_bPlayingAnim = true;
			m_bPlayingBroadSword = true;
			CGameObject* pObj = CAbstractFactory<CSkillCoolDown>::Create(138.f, 552.f);
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);
			m_eCurPrisonerState = PRISONER_SKILL_A;
			CSoundManager::Get_Instance()->StopSound(CSoundManager::BROADSWORD_READY_SFX);
			CSoundManager::Get_Instance()->PlaySound(L"weapon_broadsword_charge2.wav", CSoundManager::BROADSWORD_READY_SFX);
		}
		else if (m_bPlayingBroadSword)
		{
			m_fTime += 3.f * CTime_Manager::Get_Instance()->Get_DeltaTime();
			if (m_bLeftRight)
			{
				m_tInfo.vPos.x -= 5.f * cosf(45.f * PI / 180.f) * m_fTime;
				m_tInfo.vPos.y -= 5.f * sinf(45.f * PI / 180.f) * m_fTime - 0.5f * 9.f * m_fTime * m_fTime;
			}
			else
			{
				m_tInfo.vPos.x += 5.f * cosf(45.f * PI / 180.f) * m_fTime;
				m_tInfo.vPos.y -= 5.f * sinf(45.f * PI / 180.f) * m_fTime - 0.5f * 9.f * m_fTime * m_fTime;
			}
			CSoundManager::Get_Instance()->PlaySound(L"weapon_broadsword_release2.wav", CSoundManager::BROADSWORD_RELEASE);

			if (m_bPlayingAnim && (int)m_tFrame.fStartFrame >= 4 && (int)m_tFrame.fStartFrame < 5)
			{
				if (m_bLeftRight)
				{
					CGameObject* pObj = CAbstractFactory<CBroadSword_Effect>::Create(m_tInfo.vPos.x - 80.f, m_tInfo.vPos.y + 10);
					CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::BROADSWORD_EFFECT, pObj);

				}
				else
				{
					CGameObject* pObj = CAbstractFactory<CBroadSword_Effect>::Create(m_tInfo.vPos.x + 80.f, m_tInfo.vPos.y + 10);
					CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::BROADSWORD_EFFECT, pObj);

				}
				CSoundManager::Get_Instance()->StopSound(CSoundManager::BROADSWORD_HIT);
				CSoundManager::Get_Instance()->PlaySound(L"hit_broadsword.wav", CSoundManager::BROADSWORD_HIT);
				m_fTime = 0.f;
				m_bPlayingBroadSword = false;
			}
		}
		else if (m_bPlayingAnim && m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
		{
			m_bPlayingAnim = false;
		}
		break;
	case CPlayer::TYPE_SAMURAI:

		break;
	case CPlayer::TYPE_END:
		break;
	default:
		break;
	}
}

void CPlayer::STATE_SKILL_D()
{
	switch (m_eCurType)
	{
	case CPlayer::TYPE_DEFAULT:
		break;
	case CPlayer::TYPE_PRISONER:
		if (CKey_Manager::Get_Instance()->Key_Down(KEY_D) && !m_bUse_SKILL_D)
		{
			m_tStat.fSKill_D_MaxCool = 5.f;
			m_tStat.fSKill_D_Cool = m_tStat.fSKill_D_MaxCool;
			m_iSkillCnt = 2;
			CGameObject* pObj = CAbstractFactory<CGrenade>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::GRENADE, pObj);

			pObj = CAbstractFactory<CSkillCoolDown>::Create(179.f, 552.f);
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::UI, pObj);

			CSoundManager::Get_Instance()->StopSound(CSoundManager::GRENADE_SHOT);
			CSoundManager::Get_Instance()->PlaySound(L"Grenade_Shot.wav", CSoundManager::GRENADE_SHOT);
			m_bUse_SKILL_D = true;
		}
		break;
	case CPlayer::TYPE_SAMURAI:
		break;
	default:
		break;
	}
}

HRESULT CPlayer::Late_Ready_GameObject()
{
	return E_NOTIMPL;
}

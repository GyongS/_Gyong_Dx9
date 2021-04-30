#include "stdafx.h"
#include "GameObject.h"
#include "Texture_Manager_Client.h"
#include "Time_Manager.h"
#include "GameObject_Manager.h"

CGameObject::CGameObject() : m_bLeftRight(false), m_pStateKey(nullptr), m_fSpeed(0.f), m_fFrameSpeed(0.f)
, m_pObjectKey(nullptr), m_bLateInit(true), m_fRotation(0.f), m_eOBJID(OBJ_ID::END), m_eOBJ_TYPE(OBJ_TYPE::END)
, m_iAnimNum(0)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tStat, sizeof(STAT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}


CGameObject::~CGameObject()
{
}

HRESULT CGameObject::Late_Ready_GameObject()
{
	if (m_bLateInit)
	{
		this->Late_Ready_GameObject();
	}
	m_bLateInit = false;

	return S_OK;
}

void CGameObject::Update_ColRect()
{
}

void CGameObject::FrameMove(float fSpeed)
{
	m_tFrame.fStartFrame += m_tFrame.fEndFrame * CTime_Manager::Get_Instance()->Get_DeltaTime() * fSpeed;
	if (m_tFrame.fStartFrame >= m_tFrame.fEndFrame)
		m_tFrame.fStartFrame = 0.f;
}
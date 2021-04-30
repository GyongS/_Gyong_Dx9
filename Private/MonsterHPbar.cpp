#include "stdafx.h"
#include "MonsterHPbar.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "BrownWood.h"
#include "Warrior.h"
#include "GameObject_Manager.h"
#include "Scroll_Manager.h"
#include "Time_Manager.h"
#include "Math_Manager.h"

CMonsterHPbar::CMonsterHPbar() :
	m_fFillAmount(0.f),
	m_fBackHPFill(0.f),
	m_fBackFill(0.f),
	m_fRatioX(0.f)
	
{
}


CMonsterHPbar::~CMonsterHPbar()
{
}

HRESULT CMonsterHPbar::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f ,1.f, 0.f };
	m_pObjectKey = L"EnemyHealthBar";
	m_fFill = 1.f;

	return S_OK;
}

int CMonsterHPbar::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;
	

	return OBJ_NOEVENT;
}

void CMonsterHPbar::Late_Update_GameObject()
{	
	if (m_pTarget == nullptr)
	{
		m_bDead = true;
		return;
	}

	m_fFillAmount = m_pTarget->Get_Stat().fCurHP / m_pTarget->Get_Stat().fMaxHP;
	m_fRatioX = m_pTarget->Get_Stat().fMaxHP - m_pTarget->Get_Stat().fCurHP;

	m_tInfo.vPos.x = m_pTarget->Get_Info().vPos.x;
	m_tInfo.vPos.y = m_pTarget->Get_Info().vPos.y + 40.f;

	MonsterBackHp(m_fFillAmount);
}

void CMonsterHPbar::Render_GameObject()
{

	if (m_pTarget == nullptr)
	{
		return;
	}

	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

	if (nullptr == pTexInfo)
		return;

	m_fFillAmount = m_pTarget->Get_Stat().fCurHP / m_pTarget->Get_Stat().fMaxHP;


	if (0.f > m_fFillAmount)
		m_fFillAmount = 0.f;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X), m_tInfo.vPos.y + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y), 0.f);

	matWorld = matScale * matTrans;

	RECT rcTemp = { 0, 0,pTexInfo->tImageInfo.Width * m_fFillAmount, pTexInfo->tImageInfo.Height };
	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rcTemp, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CMonsterHPbar::Release_GameObject()
{
}

void CMonsterHPbar::MonsterBackHp(float _Infill)
{
	if (m_fFill < (_Infill - CTime_Manager::Get_Instance()->Get_DeltaTime()))
		m_fFill += CTime_Manager::Get_Instance()->Get_DeltaTime();
	else if (m_fFill >(_Infill + CTime_Manager::Get_Instance()->Get_DeltaTime()))
		m_fFill -= CTime_Manager::Get_Instance()->Get_DeltaTime();
	else
		m_fFill = _Infill;

	m_fBackHPFill = m_fFill;
	m_fBackFill = _Infill;
	m_fFillAmount = CMath_Manager::Get_Instance()->Lerp(m_fFillAmount, m_fBackFill, CTime_Manager::Get_Instance()->Get_DeltaTime() * 1.f);
}

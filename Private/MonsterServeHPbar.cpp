#include "stdafx.h"
#include "MonsterServeHPbar.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "GameObject_Manager.h"
#include "MonsterHPbar.h"
#include "BrownWood.h"
#include "Warrior.h"
#include "Scroll_Manager.h"

CMonsterServeHPbar::CMonsterServeHPbar()
{
}


CMonsterServeHPbar::~CMonsterServeHPbar()
{
}

HRESULT CMonsterServeHPbar::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f ,1.f, 0.f };
	m_pObjectKey = L"EnemyHealthBar_Damage";
	return S_OK;
}

int CMonsterServeHPbar::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;		

	// 수정해야함

	
	return OBJ_NOEVENT;
}

void CMonsterServeHPbar::Late_Update_GameObject()
{
	if (m_pTarget == nullptr)
	{
		m_bDead = true;
		return;
	}

	m_fFillAmount = 0.f;

	m_tInfo.vPos.x = m_pTarget->Get_Info().vPos.x;
	m_tInfo.vPos.y = m_pTarget->Get_Info().vPos.y + 40.f;

}

void CMonsterServeHPbar::Render_GameObject()
{

	if (m_pTarget == nullptr)
	{
		return;
	}
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

	if (nullptr == pTexInfo)
		return;	

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

void CMonsterServeHPbar::Release_GameObject()
{
}

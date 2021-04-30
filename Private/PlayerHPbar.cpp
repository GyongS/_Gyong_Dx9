#include "stdafx.h"
#include "PlayerHPbar.h"
#include "Texture_Manager_Client.h"
#include "GraphicDevice.h"
#include "GameObject_Manager.h"
#include "Player.h"
#include "Key_Manager.h"
#include "Time_Manager.h"
#include "Math_Manager.h"
#include "PlayerServeHPBar.h"

CPlayerHPbar::CPlayerHPbar() : m_fFillAmount(0.f), m_fBackHPFill(0.f), m_fBackFill(0.f)
{
}


CPlayerHPbar::~CPlayerHPbar()
{
}

HRESULT CPlayerHPbar::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f ,1.f, 0.f };
	m_pObjectKey = L"Player_HPBar";
	m_fFill = 1.f;

	return S_OK;
}

int CPlayerHPbar::Update_GameObject()
{	
	PlayerBackHp(m_fFillAmount);

	return OBJ_NOEVENT;
}

void CPlayerHPbar::Late_Update_GameObject()
{
}

void CPlayerHPbar::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

	if (nullptr == pTexInfo)
		return;

	m_fFillAmount = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Stat().fCurHP / CGameObject_Manager::Get_Instance()->Get_Player()->Get_Stat().fMaxHP;

	if (0.f > m_fFillAmount)
		m_fFillAmount = 0.f;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	RECT rcTemp = { 0, 0,pTexInfo->tImageInfo.Width * m_fFillAmount, pTexInfo->tImageInfo.Height};
	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rcTemp, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMATRIX FontmatScale, FontmatTrans, FontmatWorld;

	// 나중에 최대 체력 올리는 컨텐츠 있을 시 수정
	swprintf_s(m_szbuff, L"%d / 150", (int)static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Stat().fCurHP);

	D3DXMatrixScaling(&FontmatScale, 0.7f, 0.7f, 0.f);
	D3DXMatrixTranslation(&FontmatTrans, 115.f, 572.f, 0.f);

	FontmatWorld = FontmatScale * FontmatTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&FontmatWorld);
	CGraphicDevice::Get_Instance()->Get_Font()->DrawTextW(CGraphicDevice::Get_Instance()->Get_Sprite(), m_szbuff, lstrlen(m_szbuff), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CPlayerHPbar::Release_GameObject()
{
}

void CPlayerHPbar::PlayerBackHp(float _Infill)
{
	if(m_fFill < (_Infill - CTime_Manager::Get_Instance()->Get_DeltaTime()))
		m_fFill += CTime_Manager::Get_Instance()->Get_DeltaTime();
	else if(m_fFill > (_Infill + CTime_Manager::Get_Instance()->Get_DeltaTime()))
		m_fFill -= CTime_Manager::Get_Instance()->Get_DeltaTime();
	else
		m_fFill = _Infill;
		
	m_fBackHPFill = m_fFill;
	m_fBackFill = _Infill;
	m_fFillAmount = CMath_Manager::Get_Instance()->Lerp(m_fFillAmount, m_fBackFill, CTime_Manager::Get_Instance()->Get_DeltaTime() * 1.f);
	
}

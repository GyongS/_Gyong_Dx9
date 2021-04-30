#include "stdafx.h"
#include "BossHPBar.h"
#include "Texture_Manager_Client.h"
#include "GraphicDevice.h"
#include "GameObject_Manager.h"
#include "Key_Manager.h"
#include "Time_Manager.h"
#include "Math_Manager.h"
#include "Clegane.h"

CBossHPBar::CBossHPBar() : m_fFillAmount(0.f), m_fBackHPFill(0.f), m_fBackFill(0.f), m_fMinusX(0.f)
{
}


CBossHPBar::~CBossHPBar()
{
}

HRESULT CBossHPBar::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f ,1.f, 0.f };
	m_pObjectKey = L"BOSS_HP_BAR";
	m_fFill = 1.f;
	return S_OK;
}

int CBossHPBar::Update_GameObject()
{	
	BossBackHp(m_fFillAmount);

	return OBJ_NOEVENT;
}

void CBossHPBar::Late_Update_GameObject()
{
}

void CBossHPBar::Render_GameObject()
{

	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

	if (nullptr == pTexInfo)
		return;

	m_fFillAmount = CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Stat().fCurHP / CGameObject_Manager::Get_Instance()->Get_Boss()->Get_Stat().fMaxHP;

	if (0.f > m_fFillAmount)
		m_fFillAmount = 0.f;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXMatrixScaling(&matScale,m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);	

	matWorld = matScale * matTrans;

	RECT rcTemp = { 0, 0,pTexInfo->tImageInfo.Width * m_fFillAmount, pTexInfo->tImageInfo.Height };
	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rcTemp, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	D3DXMATRIX FontmatScale, FontmatTrans, FontmatWorld;

	TCHAR _szTemp[64];

	swprintf_s(m_szbuff, L"%d", rcTemp.right);

	D3DXMatrixScaling(&FontmatScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&FontmatTrans, 400.f, 300.f, 0.f);

	FontmatWorld = FontmatScale * FontmatTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&FontmatWorld);
	CGraphicDevice::Get_Instance()->Get_Font()->DrawTextW(CGraphicDevice::Get_Instance()->Get_Sprite(), m_szbuff, lstrlen(m_szbuff), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CBossHPBar::Release_GameObject()
{
}

void CBossHPBar::BossBackHp(float _Infill)
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

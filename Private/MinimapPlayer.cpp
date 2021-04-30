#include "stdafx.h"
#include "MinimapPlayer.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "GameObject_Manager.h"
#include "Scroll_Manager.h"

CMinimapPlayer::CMinimapPlayer()
{
}


CMinimapPlayer::~CMinimapPlayer()
{
}

HRESULT CMinimapPlayer::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"MiniPlayer";
	return S_OK;
}

int CMinimapPlayer::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;


	return OBJ_NOEVENT;
}

void CMinimapPlayer::Late_Update_GameObject()
{
}

void CMinimapPlayer::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

	if (nullptr == pTexInfo)
		return;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	float fX = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos.x;
	float fY = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos.y;

	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, fX + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X) +  3150, fY + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y) + 2350, 0.f);

	matWorld = matScale * matTrans;
	Set_Ratio(matWorld, 0.2f, 0.2f);

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CMinimapPlayer::Release_GameObject()
{
}

void CMinimapPlayer::Set_Ratio(D3DXMATRIX & matWorld, float fRatioX, float fRatioY)
{
	matWorld._11 *= fRatioX;
	matWorld._21 *= fRatioX;
	matWorld._31 *= fRatioX;
	matWorld._41 *= fRatioX;

	matWorld._12 *= fRatioY;
	matWorld._22 *= fRatioY;
	matWorld._32 *= fRatioY;
	matWorld._42 *= fRatioY;
}

#include "stdafx.h"
#include "MinimapPortal.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "GameObject_Manager.h"
#include "Scroll_Manager.h"
#include "Scene_Manager.h"

CMinimapPortal::CMinimapPortal()
{
}


CMinimapPortal::~CMinimapPortal()
{
}

HRESULT CMinimapPortal::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"MiniPortal";
	return S_OK;
}

int CMinimapPortal::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;


	return OBJ_NOEVENT;
}

void CMinimapPortal::Late_Update_GameObject()
{
}

void CMinimapPortal::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

	if (nullptr == pTexInfo)
		return;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	float fX = m_pTarget->Get_Info().vPos.x;
	float fY = 0;

	switch (CScene_Manager::Get_Instance()->Get_Scene())
	{
	case CScene_Manager::SCENE_STAGE1:
		fY = m_pTarget->Get_Info().vPos.y + 20;
		break;
	case CScene_Manager::SCENE_STAGE2:
		fY = m_pTarget->Get_Info().vPos.y + 30;
		break;
	case CScene_Manager::SCENE_STAGE3:
		fY = m_pTarget->Get_Info().vPos.y + 30;
		break;
	case CScene_Manager::SCENE_STAGE4:
		fY = m_pTarget->Get_Info().vPos.y + 30;
		break;
	case CScene_Manager::SCENE_STAGE5:
		fY = m_pTarget->Get_Info().vPos.y + 30;
		break;		
	}
	

	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, fX + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X) + 3150, fY + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y) + 2350, 0.f);

	matWorld = matScale * matTrans;
	Set_Ratio(matWorld, 0.2f, 0.2f);

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CMinimapPortal::Release_GameObject()
{
}

void CMinimapPortal::Set_Ratio(D3DXMATRIX & matWorld, float fRatioX, float fRatioY)
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

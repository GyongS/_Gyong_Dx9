#include "stdafx.h"
#include "Stage1Helper.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Scroll_Manager.h"

CStage1Helper::CStage1Helper()
{
}


CStage1Helper::~CStage1Helper()
{
}

HRESULT CStage1Helper::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f ,1.f, 0.f };
	m_pObjectKey = L"Stage1Tutorial";
	return S_OK;
}

int CStage1Helper::Update_GameObject()
{

	return OBJ_NOEVENT;
}

void CStage1Helper::Late_Update_GameObject()
{
}

void CStage1Helper::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

	if (pTexInfo == nullptr)
		return;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X), m_tInfo.vPos.y + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y), 0.f);

	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMATRIX FontmatScale, FontmatTrans, FontmatWorld;

	m_Dialogue = L"↓ + C 버튼으로\n내려갈 수 있어.\n어서 인간들을 쫓아가";

	D3DXMatrixScaling(&FontmatScale, 0.8f, 0.8f, 0.f);
	D3DXMatrixTranslation(&FontmatTrans, (m_tInfo.vPos.x - 60) + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X), (m_tInfo.vPos.y + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y)) - 80, 0.f);
	FontmatWorld = FontmatScale * FontmatTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&FontmatWorld);
	CGraphicDevice::Get_Instance()->Get_Font()->DrawTextW(CGraphicDevice::Get_Instance()->Get_Sprite(), m_Dialogue.c_str(), lstrlen(m_Dialogue.c_str()), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CStage1Helper::Release_GameObject()
{
}

#include "stdafx.h"
#include "Item_Shop.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Scroll_Manager.h"
#include "GameObject_Manager.h"
#include "Scene_Manager.h"
#include "Player.h"

CItem_Shop::CItem_Shop()
{
}


CItem_Shop::~CItem_Shop()
{
}

HRESULT CItem_Shop::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"Resources";
	m_pStateKey = L"Shop";
	m_tFrame.fStartFrame = 0.f;
	m_tFrame.fEndFrame = 8.f;
	m_fFrameSpeed = 1.f;
	m_tInfo.iColCX = 50;
	m_tInfo.iColCY = 150;
	m_eOBJID = OBJ_ID::SHOP;
	return S_OK;
}

int CItem_Shop::Update_GameObject()
{
	return OBJ_NOEVENT;
}

void CItem_Shop::Late_Update_GameObject()
{
	FrameMove(m_fFrameSpeed);

}

void CItem_Shop::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey, m_pStateKey, (int)m_tFrame.fStartFrame);

	if (nullptr == pTexInfo)
		return;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans, matScale, matWorld;
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X), m_tInfo.vPos.y + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y), 0.f);

	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	const TEXINFO* pTexInfo2 = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(L"GOLD_ICON");

	D3DXMATRIX matTrans2, matScale2, matWorld2;
	D3DXMatrixScaling(&matScale2, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans2, 520 + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X), 340.f + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y), 0.f);

	matWorld2 = matScale2 * matTrans2;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld2);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo2->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMATRIX FontmatScale, FontmatTrans, FontmatWorld;

	TCHAR szbuff[64], szTemp[64];

	// ±¸¸ÅÇÏ¸é ---·Î ¹Ù²¨¾ß´ï

	swprintf_s(szbuff, L"10000");

	D3DXMatrixScaling(&FontmatScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&FontmatTrans, 496.f + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X), 300.f + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y), 0.f);

	FontmatWorld = FontmatScale * FontmatTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&FontmatWorld);
	CGraphicDevice::Get_Instance()->Get_Font()->DrawTextW(CGraphicDevice::Get_Instance()->Get_Sprite(), szbuff, lstrlen(szbuff), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_InItemShop())
	{
		pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(L"InShopPopup");

		D3DXMATRIX matTrans, matScale, matWorld;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, (m_tInfo.vPos.x + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_X)) + 10, (m_tInfo.vPos.y + CScroll_Manager::Get_ScrollPos(SCROLL_ID::SCROLL_Y)) + 50, 0.f);

		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CItem_Shop::Release_GameObject()
{
}

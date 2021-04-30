#include "stdafx.h"
#include "BroadSword_Effect.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Scroll_Manager.h"
#include "Collision.h"
#include "GameObject_Manager.h"

CBroadSword_Effect::CBroadSword_Effect()
{
}


CBroadSword_Effect::~CBroadSword_Effect()
{
}

HRESULT CBroadSword_Effect::Ready_GameObject()
{
	m_tInfo.vSize        = { 1.f, 1.f, 0.f };
	m_pObjectKey         = L"HIT_EFFECT";
	m_pStateKey          = L"BroadSword";
	m_tFrame.fStartFrame = 0.f;
	m_tFrame.fEndFrame   = 10.f;
	return S_OK;
}

HRESULT CBroadSword_Effect::Late_Ready_GameObject()
{
	CGameObject* pObj = CAbstractFactory<CCollision>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
	pObj->Set_ColScale(150, 100);
	pObj->Set_OBJID(OBJ_ID::BROADSWORD_COL);
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::BROADSWORD_COL, pObj);
	return S_OK;
}

int CBroadSword_Effect::Update_GameObject()
{
	CGameObject::Late_Ready_GameObject();

	if (m_tFrame.fStartFrame >= m_tFrame.fEndFrame - 1.f)
	{
		return OBJ_DEAD;
	}
	if (m_tFrame.fStartFrame <= 3.f)
	{
		CScroll_Manager::Set_bScrollShake(true);
	}
	if (m_tFrame.fStartFrame > 3.f)
	{
		CScroll_Manager::Set_bScrollShake(false);
	}



	return OBJ_NOEVENT;
}

void CBroadSword_Effect::Late_Update_GameObject()
{
	FrameMove();
}

void CBroadSword_Effect::Render_GameObject()
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
}

void CBroadSword_Effect::Release_GameObject()
{
}

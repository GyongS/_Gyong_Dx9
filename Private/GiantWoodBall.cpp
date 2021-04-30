#include "stdafx.h"
#include "GiantWoodBall.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Scroll_Manager.h"
#include "Time_Manager.h"

CGiantWoodBall::CGiantWoodBall() : m_fLifeTime(0.f)
{
}


CGiantWoodBall::~CGiantWoodBall()
{
}

HRESULT CGiantWoodBall::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_eOBJID = OBJ_ID::GIANTWOOD_BALL;
	m_pObjectKey = L"WOODMONSTER";
	m_pStateKey = L"WOODMONSTER_BALL";
	m_tFrame.fStartFrame = 0.f;
	m_tFrame.fEndFrame = 1.f;
	m_tInfo.iColCX = 15;
	m_tInfo.iColCY = 15;
	m_fSpeed = 200.f;
	return S_OK;
}

int CGiantWoodBall::Update_GameObject()
{
	if (m_bDead || m_fLifeTime >= 2.f)
		return OBJ_DEAD;

	m_fLifeTime += CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_tInfo.vPos.x += cosf(m_fRotation * 180.f / PI) * CTime_Manager::Get_Instance()->Get_DeltaTime() * m_fSpeed;
	m_tInfo.vPos.y -= sinf(m_fRotation * 180.f / PI) * CTime_Manager::Get_Instance()->Get_DeltaTime() * m_fSpeed;

	return OBJ_NOEVENT;
}

void CGiantWoodBall::Late_Update_GameObject()
{	
	FrameMove();
}

void CGiantWoodBall::Render_GameObject()
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

void CGiantWoodBall::Release_GameObject()
{
}

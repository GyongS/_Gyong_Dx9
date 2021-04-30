#include "stdafx.h"
#include "Event_UI_Top.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Time_Manager.h"
#include "GameObject_Manager.h"
#include "Event_Text.h"
#include "EventManager.h"

CEvent_UI_Top::CEvent_UI_Top() : m_bStartText(false),m_bReadyText(false)
{
}


CEvent_UI_Top::~CEvent_UI_Top()
{
}

HRESULT CEvent_UI_Top::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f ,1.f, 0.f };
	m_pObjectKey = L"Event_UI";
	return S_OK;
}

int CEvent_UI_Top::Update_GameObject()
{
	if(!CEventManager::Get_Instance()->Get_bEvent())
	{
		if (m_tInfo.vPos.y > -200.f)
		{
			m_tInfo.vPos.y -= 100.f * CTime_Manager::Get_Instance()->Get_DeltaTime();
		}

		if (m_tInfo.vPos.y <= -200.f)
		{
			m_tInfo.vPos.y = -200.f;
			m_bReadyText = false;
		}
	}
	else if(CEventManager::Get_Instance()->Get_bEvent())
	{
		if (m_tInfo.vPos.y < -100)
			m_tInfo.vPos.y += 100.f * CTime_Manager::Get_Instance()->Get_DeltaTime();

		if (m_tInfo.vPos.y >= -100)
		{
			m_tInfo.vPos.y = -100;
			if(!m_bReadyText)
				m_bStartText = true;
		}
		
	}
	if (m_bStartText)
	{
		CGameObject* pObj = CAbstractFactory<CEvent_Text>::Create(100.f, 520.f);
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJ_ID::EVENT_UI, pObj);

		m_bReadyText = true;
		m_bStartText = false;
	}
	
	return OBJ_NOEVENT;
}

void CEvent_UI_Top::Late_Update_GameObject()
{
}

void CEvent_UI_Top::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(m_pObjectKey);

	if (nullptr == pTexInfo)
		return;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CEvent_UI_Top::Release_GameObject()
{
}

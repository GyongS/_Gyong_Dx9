#include "stdafx.h"
#include "DialogueBody.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "EventManager.h"
#include "Scene_Manager.h"

CDialogueBody::CDialogueBody()
{
}


CDialogueBody::~CDialogueBody()
{
}

HRESULT CDialogueBody::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_pObjectKey = L"Dialogue_Body";
	return S_OK;
}

int CDialogueBody::Update_GameObject()
{
	if (!CEventManager::Get_Instance()->Get_bEvent())
	{
 		CEventManager::Get_Instance()->Set_CreateDialogue(false);
		return OBJ_DEAD;
	}

	return OBJ_NOEVENT;
}

void CDialogueBody::Late_Update_GameObject()
{
}

void CDialogueBody::Render_GameObject()
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

	DialogueName();
}

void CDialogueBody::Release_GameObject()
{
}

void CDialogueBody::DialogueName()
{
	D3DXMATRIX FontmatScale, FontmatTrans, FontmatWorld;

	if (CEventManager::Get_Instance()->Get_EventCnt() < 3)
	{
		m_DialogueName = L"스켈레톤";
		D3DXMatrixScaling(&FontmatScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&FontmatTrans, 165, 490, 0.f);
	}
	else if(CEventManager::Get_Instance()->Get_EventCnt() >= 3 && CEventManager::Get_Instance()->Get_EventCnt() < 4)
	{
		m_DialogueName = L"마녀";
		D3DXMatrixScaling(&FontmatScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&FontmatTrans, 165, 490, 0.f);
	}
	else if(CEventManager::Get_Instance()->Get_EventCnt() >= 4 && CScene_Manager::Get_Instance()->Get_Scene() == CScene_Manager::SCENE_STAGE7)
	{
		switch (CEventManager::Get_Instance()->Get_DialogueCount())
		{
		case 0:
			m_DialogueName = L"113기 지용석";
			break;
		case 1:
			m_DialogueName = L"병거니우스";
			break;
		case 2:
			m_DialogueName = L"113기 지용석";
			break;
		case 3:
			m_DialogueName = L"병거니우스";
			break;
		case 4:
			m_DialogueName = L"113기 지용석";
			break;
		default:
			break;
		}
		D3DXMatrixScaling(&FontmatScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&FontmatTrans, 145, 490, 0.f);
	}
	
	
	FontmatWorld = FontmatScale * FontmatTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&FontmatWorld);
	CGraphicDevice::Get_Instance()->Get_Font()->DrawTextW(CGraphicDevice::Get_Instance()->Get_Sprite(), m_DialogueName.c_str(), lstrlen(m_DialogueName.c_str()), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (CEventManager::Get_Instance()->Get_DialogueCount() % 2 == 0 && CScene_Manager::Get_Instance()->Get_Scene() == CScene_Manager::SCENE_STAGE7)
	{
		const TEXINFO* pTexInfo = CTexture_Manager_Client::Get_Instance()->Get_TexInfo(L"Skul_Tutorial");

		if (nullptr == pTexInfo)
			return;

		float fCenterX = pTexInfo->tImageInfo.Width >> 1;
		float fCenterY = pTexInfo->tImageInfo.Height >> 1;


		D3DXMATRIX matTrans, matScale, matWorld;

		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, 100, 480, 0.f);

		matWorld = matScale * matTrans;

		CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphicDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

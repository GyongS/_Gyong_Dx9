#include "stdafx.h"
#include "Scroll_Manager.h"
#include "Scene_Manager.h"

D3DXVECTOR3 CScroll_Manager::m_vScroll = {};
bool CScroll_Manager::m_bScrollShake = false;

CScroll_Manager::CScroll_Manager()
{
}


CScroll_Manager::~CScroll_Manager()
{
}

float CScroll_Manager::Get_ScrollPos(SCROLL_ID::ID eScroll)
{
	switch (eScroll)
	{
	case SCROLL_ID::SCROLL_X:
		return m_vScroll.x;
	case SCROLL_ID::SCROLL_Y:
		return m_vScroll.y;
	}
}

D3DXVECTOR3 CScroll_Manager::Get_ScrollPos()
{
	return m_vScroll;
}

void CScroll_Manager::Set_ScrollPos(const D3DXVECTOR3 & vScroll)
{
	m_vScroll += vScroll;
}

void CScroll_Manager::Scroll_Lock()
{
	if (-50 < m_vScroll.x)
		m_vScroll.x = -50.f;

	if (CScene_Manager::Get_Instance()->Get_Scene() == CScene_Manager::SCENE_STAGE7)
	{
		if (m_bScrollShake)
		{
			m_vScroll.x = m_vScroll.x + float(rand() % 10) - 10;
			m_vScroll.y = WINCY - WINCY + float(rand() % 10);
			return;
		}

		if (m_vScroll.x < WINCX - (TILECX * TILEX) + 900)
			m_vScroll.x = WINCX - (TILECX * TILEX) + 900;

		if (m_vScroll.y < WINCY - WINCY)
			m_vScroll.y = WINCY - WINCY;
	}
	else if (CScene_Manager::Get_Instance()->Get_Scene() == CScene_Manager::BONUS_STAGE)
	{
		if (m_bScrollShake)
		{
			m_vScroll.x = m_vScroll.x + float(rand() % 10);
			m_vScroll.y = WINCY - WINCY + float(rand() % 10);
			return;
		}

		if (m_vScroll.x < WINCX - (TILECX * TILEX) + 900)
			m_vScroll.x = WINCX - (TILECX * TILEX) + 900;

		if (m_vScroll.y < WINCY - WINCY)
			m_vScroll.y = WINCY - WINCY;
	}
	else
	{
		if (m_bScrollShake)
		{
			m_vScroll.x = m_vScroll.x + float(rand() % 10);
			m_vScroll.y = WINCY - WINCY + float(rand() % 10);
			return;
		}

		if (m_vScroll.x < WINCX - (TILECX * TILEX) + 50)
			m_vScroll.x = WINCX - (TILECX * TILEX) + 50;

		if (m_vScroll.y < WINCY - WINCY)
			m_vScroll.y = WINCY - WINCY;
	}	
}
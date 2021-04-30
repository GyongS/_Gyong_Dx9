#include "stdafx.h"
#include "Event_Text.h"
#include "GraphicDevice.h"
#include "Texture_Manager_Client.h"
#include "Player.h"
#include "GameObject_Manager.h"
#include "Time_Manager.h"
#include "EventManager.h"
#include <locale>
#include <codecvt>
#include "Key_Manager.h"
#include "SoundManager.h"

CEvent_Text::CEvent_Text() : m_fTimer(0.f), m_iTextCnt(0), m_iIndex(0), m_iSaveIndex(0)
{
}


CEvent_Text::~CEvent_Text()
{
}

HRESULT CEvent_Text::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	return S_OK;
}

int CEvent_Text::Update_GameObject()
{
	if (!CEventManager::Get_Instance()->Get_bEvent())
		return OBJ_DEAD;

	CEventManager::Get_Instance()->Set_DialogueCount(m_iIndex);

	switch (CEventManager::Get_Instance()->Get_EventCnt())
	{
	case 1:
		Tutorial_0_Event();
		break;
	case 2:
		Tutorial_1_Event();
		break;
	case 3:
		Tutorial_2_Event();
		break;
	case 4:
		BossDead_Event();
		break;
	default:
		BossDead_Event();
		break;
	}

	return OBJ_NOEVENT;
}

void CEvent_Text::Late_Update_GameObject()
{
}

void CEvent_Text::Render_GameObject()
{

	D3DXMATRIX FontmatScale, FontmatTrans, FontmatWorld;

	D3DXMatrixScaling(&FontmatScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&FontmatTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	FontmatWorld = FontmatScale * FontmatTrans;

	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&FontmatWorld);
	CGraphicDevice::Get_Instance()->Get_Font()->DrawTextW(CGraphicDevice::Get_Instance()->Get_Sprite(), m_strCurText.c_str(), lstrlen(m_strCurText.c_str()), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CEvent_Text::Release_GameObject()
{
}

void CEvent_Text::Tutorial_0_Event()
{
	string filePath = "../Data/TutorialDialogue_0.txt";
	wifstream load(filePath.data());

	// 한글 폰트 깨짐 현상 방지
	load.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));

	if (load.is_open())
	{
		wstring strText;

		while (!load.eof())
		{
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
			}
			else
				break;			
		}
	}
	m_iSaveIndex = 0;

	if (CKey_Manager::Get_Instance()->Key_Down(KEY_SPACE))
	{
		if (m_iTextCnt <= m_strSaveText[m_iIndex].size())
		{
			m_iTextCnt = m_strSaveText[m_iIndex].size();
			m_strCurText = m_strSaveText[m_iIndex].substr(0, m_iTextCnt);			
		}
		else
		{
			++m_iIndex;
			if (m_iIndex == 5)
			{
				CEventManager::Get_Instance()->Set_StartEvent(false);
				m_iIndex = 0;
				CEventManager::Get_Instance()->Set_DialogueCount(m_iIndex);
				return;
			}

			m_iTextCnt = 0;
			m_strCurText = L"";
			m_fTimer = 0.f;
		}
	}

	if (m_iTextCnt > m_strSaveText[m_iIndex].size())
		return;

	m_fTimer += CTime_Manager::Get_Instance()->Get_DeltaTime();

	if (m_fTimer >= 0.1f)
	{
		m_strCurText = m_strSaveText[m_iIndex].substr(0, m_iTextCnt);
		++m_iTextCnt;
		CSoundManager::Get_Instance()->StopSound(CSoundManager::UI_TALK);
		CSoundManager::Get_Instance()->PlaySound(L"UI_Talk.wav", CSoundManager::UI_TALK);
		m_fTimer = 0.f;
	}
}

void CEvent_Text::Tutorial_1_Event()
{
	string filePath = "../Data/TutorialDialogue_1.txt";
	wifstream load(filePath.data());

	// 한글 폰트 깨짐 현상 방지
	load.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));

	if (load.is_open())
	{
		wstring strText;

		while (!load.eof())
		{
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
			}
			else
				break;
		}
	}
	m_iSaveIndex = 0;

	if (CKey_Manager::Get_Instance()->Key_Down(KEY_SPACE))
	{
		if (m_iTextCnt <= m_strSaveText[m_iIndex].size())
		{
			m_iTextCnt = m_strSaveText[m_iIndex].size();
			m_strCurText = m_strSaveText[m_iIndex].substr(0, m_iTextCnt);
		}
		else
		{
			++m_iIndex;
			if (m_iIndex == 10)
			{
				CEventManager::Get_Instance()->Set_StartEvent(false);
				m_iIndex = 0;
				CEventManager::Get_Instance()->Set_DialogueCount(m_iIndex);
				return;
			}

			m_iTextCnt = 0;
			m_strCurText = L"";
			m_fTimer = 0.f;
		}
	}

	if (m_iTextCnt > m_strSaveText[m_iIndex].size())
		return;

	m_fTimer += CTime_Manager::Get_Instance()->Get_DeltaTime();

	if (m_fTimer >= 0.1f)
	{
		m_strCurText = m_strSaveText[m_iIndex].substr(0, m_iTextCnt);
		++m_iTextCnt;
		CSoundManager::Get_Instance()->StopSound(CSoundManager::UI_TALK);
		CSoundManager::Get_Instance()->PlaySound(L"UI_Talk.wav", CSoundManager::UI_TALK);
		m_fTimer = 0.f;
	}
}

void CEvent_Text::Tutorial_2_Event()
{
	string filePath = "../Data/TutorialDialogue_2.txt";
	wifstream load(filePath.data());

	// 한글 폰트 깨짐 현상 방지
	load.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));

	if (load.is_open())
	{
		wstring strText;

		while (!load.eof())
		{
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
			}
			else
				break;
		}
	}
	m_iSaveIndex = 0;

	if (CKey_Manager::Get_Instance()->Key_Down(KEY_SPACE))
	{
		if (m_iTextCnt <= m_strSaveText[m_iIndex].size())
		{
			m_iTextCnt = m_strSaveText[m_iIndex].size();
			m_strCurText = m_strSaveText[m_iIndex].substr(0, m_iTextCnt);
		}
		else
		{
			++m_iIndex;
			if (m_iIndex == 8)
			{
				CEventManager::Get_Instance()->Set_StartEvent(false);
				m_iIndex = 0;
				CEventManager::Get_Instance()->Set_DialogueCount(m_iIndex);
				return;
			}

			m_iTextCnt = 0;
			m_strCurText = L"";
			m_fTimer = 0.f;
		}
	}

	if (m_iTextCnt > m_strSaveText[m_iIndex].size())
		return;

	m_fTimer += CTime_Manager::Get_Instance()->Get_DeltaTime();

	if (m_fTimer >= 0.1f)
	{
		m_strCurText = m_strSaveText[m_iIndex].substr(0, m_iTextCnt);
		++m_iTextCnt;
		CSoundManager::Get_Instance()->StopSound(CSoundManager::UI_TALK);
		CSoundManager::Get_Instance()->PlaySound(L"UI_Talk.wav", CSoundManager::UI_TALK);
		m_fTimer = 0.f;
	}
}

void CEvent_Text::BossDead_Event()
{
	string filePath = "../Data/BossDeadDialogue.txt";
	wifstream load(filePath.data());

	// 한글 폰트 깨짐 현상 방지
	load.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));

	if (load.is_open())
	{
		wstring strText;

		while (!load.eof())
		{
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}
			if (getline(load, strText))
			{
				m_strSaveText[m_iSaveIndex] = strText;
				++m_iSaveIndex;
			}		
			else
				break;
		}
	}
	m_iSaveIndex = 0;

	if (CKey_Manager::Get_Instance()->Key_Down(KEY_SPACE))
	{
		if (m_iTextCnt <= m_strSaveText[m_iIndex].size())
		{
			m_iTextCnt = m_strSaveText[m_iIndex].size();
			m_strCurText = m_strSaveText[m_iIndex].substr(0, m_iTextCnt);
		}
		else
		{
			++m_iIndex;
			if (m_iIndex == 5)
			{
				CEventManager::Get_Instance()->Set_StartEvent(false);
				m_iIndex = 0;
				CEventManager::Get_Instance()->Set_DialogueCount(m_iIndex);
				return;
			}

			m_iTextCnt = 0;
			m_strCurText = L"";
			m_fTimer = 0.f;
		}
	}

	if (m_iTextCnt > m_strSaveText[m_iIndex].size())
		return;

	m_fTimer += CTime_Manager::Get_Instance()->Get_DeltaTime();

	if (m_fTimer >= 0.1f)
	{
		m_strCurText = m_strSaveText[m_iIndex].substr(0, m_iTextCnt);
		++m_iTextCnt;
		CSoundManager::Get_Instance()->StopSound(CSoundManager::UI_TALK);
		CSoundManager::Get_Instance()->PlaySound(L"UI_Talk.wav", CSoundManager::UI_TALK);
		m_fTimer = 0.f;
	}
}
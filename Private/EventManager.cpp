#include "stdafx.h"
#include "EventManager.h"

IMPLEMENT_SINGLETON(CEventManager)

CEventManager::CEventManager() : m_bEvent(false), m_iEventCnt(0), m_bCreateDialogue(false)
{
}


CEventManager::~CEventManager()
{
}

void CEventManager::Set_StartEvent(bool _bCheck)
{
	m_bEvent = _bCheck;
}

void CEventManager::Set_EventCnt()
{
	++m_iEventCnt;
}

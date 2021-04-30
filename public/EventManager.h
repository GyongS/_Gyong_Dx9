#pragma once
class CEventManager
{
	DECLARE_SINGLETON(CEventManager)
public:
	CEventManager();
	~CEventManager();

public:
	void Set_StartEvent(bool _bCheck);
	void Set_CreateDialogue(bool _bCheck) { m_bCreateDialogue = _bCheck; }
	void Set_EventCnt();
	void Set_DialogueCount(int _Count) { m_iDialogueCount = _Count; }
public:
	bool& Get_bEvent() { return m_bEvent; }
	bool& Get_bCreateDialogue() { return m_bCreateDialogue; }
	int&  Get_EventCnt() { return m_iEventCnt; }
	int&  Get_DialogueCount() { return m_iDialogueCount; }
private:
	bool m_bCreateDialogue;
	bool m_bEvent;
	int  m_iEventCnt;
	int  m_iDialogueCount;
};


#pragma once
#include "GameObject.h"
class CDialogueBody : public CGameObject
{
public:
	CDialogueBody();
	virtual ~CDialogueBody();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	wstring m_DialogueName;
	void DialogueName();

};


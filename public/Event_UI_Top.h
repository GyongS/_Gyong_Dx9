#pragma once
#include "GameObject.h"
class CEvent_UI_Top : public CGameObject
{
public:
	CEvent_UI_Top();
	virtual ~CEvent_UI_Top();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	bool m_bStartText;
	bool m_bReadyText;
};


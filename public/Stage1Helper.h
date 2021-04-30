#pragma once
#include "GameObject.h"
class CStage1Helper : public CGameObject
{
public:
	CStage1Helper();
	virtual ~CStage1Helper();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	wstring m_Dialogue;
};


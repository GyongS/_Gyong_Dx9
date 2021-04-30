#pragma once
#include "GameObject.h"
class CCage_Human : public CGameObject
{
public:
	CCage_Human();
	virtual ~CCage_Human();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	bool m_bDropItem;

};
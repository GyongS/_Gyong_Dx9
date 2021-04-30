#pragma once
#include "GameObject.h"
class CStone : public CGameObject
{
public:
	CStone();
	virtual ~CStone();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	float m_fDelay;
};


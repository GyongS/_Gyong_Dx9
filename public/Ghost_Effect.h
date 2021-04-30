#pragma once
#include "GameObject.h"
class CGhost_Effect : public CGameObject
{
public:
	CGhost_Effect();
	virtual ~CGhost_Effect();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	int m_iAlpha;
};


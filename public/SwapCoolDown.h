#pragma once
#include "GameObject.h"
class CSwapCoolDown : public CGameObject
{
public:
	CSwapCoolDown();
	virtual ~CSwapCoolDown();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	float m_fSwap_CoolTime;
};


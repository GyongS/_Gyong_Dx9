#pragma once
#include "GameObject.h"
class CStamping_Hit_Effect : public CGameObject
{
public:
	CStamping_Hit_Effect();
	virtual ~CStamping_Hit_Effect();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};


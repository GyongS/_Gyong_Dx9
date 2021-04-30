#pragma once
#include "GameObject.h"
class CAttackSign : public CGameObject
{
public:
	CAttackSign();
	virtual ~CAttackSign();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	int m_iCnt;
};


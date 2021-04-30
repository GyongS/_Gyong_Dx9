#pragma once
#include "GameObject.h"
class CDarkQuartz : public CGameObject
{
public:
	CDarkQuartz();
	virtual ~CDarkQuartz();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};


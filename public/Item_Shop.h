#pragma once
#include "GameObject.h"
class CItem_Shop : public CGameObject
{
public:
	CItem_Shop();
	virtual ~CItem_Shop();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};


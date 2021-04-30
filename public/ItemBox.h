#pragma once
#include "GameObject.h"
class CItemBox : public CGameObject
{
public:
	CItemBox();
	virtual ~CItemBox();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};


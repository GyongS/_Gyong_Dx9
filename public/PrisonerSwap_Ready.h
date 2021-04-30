#pragma once
#include "GameObject.h"
class CPrisonerSwap_Ready :	public CGameObject
{
public:
	CPrisonerSwap_Ready();
	virtual ~CPrisonerSwap_Ready();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};


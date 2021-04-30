#pragma once
#include "GameObject.h"
class CMinimapMonster : public CGameObject
{
public:
	CMinimapMonster();
	virtual ~CMinimapMonster();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	void Set_Ratio(D3DXMATRIX& matWorld, float fRatioX, float fRatioY);
};


#pragma once
#include "GameObject.h"
class CPlayerHit_UI : public CGameObject
{
public:
	CPlayerHit_UI();
	virtual ~CPlayerHit_UI();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
    int m_iAlpha;

};


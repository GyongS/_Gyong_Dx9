#pragma once
#include "GameObject.h"
class CBossHPBack : public CGameObject
{
public:
	CBossHPBack();
	virtual ~CBossHPBack();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	TCHAR			m_szbuff[64];
};


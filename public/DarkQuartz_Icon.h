#pragma once
#include "GameObject.h"
class CDarkQuartz_Icon : public CGameObject
{
public:
	CDarkQuartz_Icon();
	virtual ~CDarkQuartz_Icon();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	
private:
	TCHAR			m_szbuff[64];

};


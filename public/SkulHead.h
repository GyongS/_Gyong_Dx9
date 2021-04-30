#pragma once
#include "GameObject.h"
class CSkulHead : public CGameObject
{
public:
	CSkulHead();
	virtual ~CSkulHead();

	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Late_Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;


public:
	bool& Get_GroundCheck() { return m_bGround; }

public:
	void Set_GroundCheck(bool _bCheck) { m_bGround = _bCheck; }
	void Set_JumpTime() { m_fTime = 0.f; }

private:
	float m_fGravity;
	float m_fTime;

	bool m_bGround;

};


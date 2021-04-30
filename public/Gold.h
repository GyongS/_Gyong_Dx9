#pragma once
#include "GameObject.h"
class CGold : public CGameObject
{
public:
	CGold();
	virtual ~CGold();

	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Late_Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	bool& Get_GroundCheck() { return m_bGround; }
	bool& Get_bPickUp() { return m_bPIckUp; }
public:
	void Set_GroundCheck(bool _bCheck) { m_bGround = _bCheck; }
	void Set_JumpTime() { m_fJumpTime = 0.f; }

private:
	float m_fJumpTime;
	float m_fGravity;

	float m_fTimer;

	bool m_bGround;
	bool m_bGet_Gold;
	bool m_bPIckUp;
};


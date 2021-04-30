#pragma once
#include "GameObject.h"
class CPrisonerHead : public CGameObject
{
public:
	CPrisonerHead();
	virtual ~CPrisonerHead();

	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Late_Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
public:
	bool& Get_JumpCheck() { return m_bJump; }

public:
	void Set_JumpCheck(bool _bCheck) { m_bJump = _bCheck; }
	void Set_JumpTime() { m_fJumpTime = 0.f; }

private:
	float m_fJumpForce;
	float m_fJumpTime;
	float m_fJumpY;
	float m_fGravity;

	bool m_bJump;
};


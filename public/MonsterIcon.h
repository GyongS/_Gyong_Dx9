#pragma once
#include "GameObject.h"
class CMonsterIcon : public CGameObject
{
public:
	CMonsterIcon();
	virtual ~CMonsterIcon();

	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Late_Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	int& Get_MonsterCount() { return m_iMonsterCnt; }

public:
	void Set_MonsterCnt(int _InCnt) { m_iMonsterCnt += _InCnt; }

private:
	int m_iMonsterCnt;
};


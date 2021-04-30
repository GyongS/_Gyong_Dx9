#pragma once
#include "GameObject.h"
class CTutorialSkeleton : public CGameObject
{
public:
	enum Skeleton_STATE
	{
		GIVE,
		DEAD,
		END
	};

public:
	CTutorialSkeleton();
	virtual ~CTutorialSkeleton();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	void Change_State();

private:
	Skeleton_STATE m_eCurState;
	Skeleton_STATE m_ePreState;
};


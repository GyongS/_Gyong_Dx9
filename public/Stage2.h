#pragma once
#include "Scene.h"
class CStage2 :	public CScene
{
public:
	CStage2();
	virtual ~CStage2();

	virtual HRESULT Ready_Scene() override;
	virtual int Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

public:
	static CStage2* Create();

private:
	void Stage_Event();
};


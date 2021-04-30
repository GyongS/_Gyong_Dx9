#pragma once
#include "Scene.h"
class CStage1 :
	public CScene
{
public:
	CStage1();
	virtual ~CStage1();

	virtual HRESULT Ready_Scene() override;
	virtual int Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

public:
	static CStage1* Create();

private:
	void Start_Create_UI();
	void Tutorial_Start();
	void StageEvent();
};


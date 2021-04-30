#pragma once
#include "Scene.h"
class CStage5 : public CScene
{
public:
	CStage5();
	virtual ~CStage5();

	virtual HRESULT Ready_Scene() override;
	virtual int Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

public:
	static CStage5* Create();
};


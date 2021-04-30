#pragma once
#include "Scene.h"
class CStage4 : public CScene
{
public:
	CStage4();
	virtual ~CStage4();

	virtual HRESULT Ready_Scene() override;
	virtual int Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

public:
	static CStage4* Create();
};


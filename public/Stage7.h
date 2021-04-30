#pragma once
#include "Scene.h"
class CStage7 : public CScene
{
public:
	CStage7();
	virtual ~CStage7();

	virtual HRESULT Ready_Scene() override;
	virtual int Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

public:
	static CStage7* Create();
};


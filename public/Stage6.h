#pragma once
#include "Scene.h"
class CStage6 : public CScene
{
public:
	CStage6();
	virtual ~CStage6();

	virtual HRESULT Ready_Scene() override;
	virtual int Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

public:
	static CStage6* Create();

};


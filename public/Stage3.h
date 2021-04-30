#pragma once
#include "Scene.h"
class CStage3 : public CScene
{
public:
	CStage3();
	virtual ~CStage3();

	virtual HRESULT Ready_Scene() override;
	virtual int Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
public:
	static CStage3* Create();
};


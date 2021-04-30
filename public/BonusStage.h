#pragma once
#include "Scene.h"
class CBonusStage : public CScene
{
public:
	CBonusStage();
	virtual ~CBonusStage();

	virtual HRESULT Ready_Scene() override;
	virtual int Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

public:
	static CBonusStage* Create();
};


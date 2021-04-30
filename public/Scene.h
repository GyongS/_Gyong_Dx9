#pragma once
class CScene
{
public:
	CScene();
	virtual ~CScene();
public:
	virtual HRESULT Ready_Scene()PURE;
	virtual int Update_Scene()PURE;
	virtual void Late_Update_Scene()PURE;
	virtual void Render_Scene()PURE;
	virtual void Release_Scene()PURE;
};


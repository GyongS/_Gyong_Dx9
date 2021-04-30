#pragma once
class CFrame_Manager;
class CGraphicDevice;
class CMainGame
{
public:
	explicit CMainGame();
	~CMainGame();

public:
	HRESULT Ready_MainGame();
	void Update();
	void Late_Update();
	void Render(CFrame_Manager* pFrameManager);
	void Release();

private:
	HRESULT Single_Resources_Load();
	CGraphicDevice* m_pGrphic_Device;

private:
	HRESULT Load_Single_UI_Texture();
};


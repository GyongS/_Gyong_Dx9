#pragma once
class CGraphicDevice final
{
	DECLARE_SINGLETON(CGraphicDevice)

public:
	enum WINMODE { MODE_FULL, MODE_WIN };

private:
	explicit CGraphicDevice();
	~CGraphicDevice();
	// 복사 생성자, 디폴트 대입연산자. 
	explicit CGraphicDevice(const CGraphicDevice& rObject) {};
	void operator=(const CGraphicDevice& rObject) {};

public:
	LPDIRECT3DDEVICE9	Get_Device() { return m_pDevice; }
	LPD3DXSPRITE		Get_Sprite() { return m_pSprite; }
	LPD3DXFONT			Get_Font() { return m_pFont; }
	LPD3DXLINE			Get_Line() { return m_pLine; }

public:
	// 객체를 제어하기 위한 컴객체들을 생성 및 초기화 할 공간. 
	HRESULT Ready_Graphic_Device(WINMODE eMode);
	void Release_Graphic_Device();
	void Render_Begin();
	void Render_End(HWND hWND = nullptr);

private:
	LPDIRECT3D9			m_pSDK;
	LPDIRECT3DDEVICE9	m_pDevice; // 실질적으로 장치를 제어하는 com 객체. 
	LPD3DXSPRITE		m_pSprite;
	LPD3DXFONT			m_pFont;
	LPD3DXLINE			m_pLine;
};



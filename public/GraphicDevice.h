#pragma once
class CGraphicDevice final
{
	DECLARE_SINGLETON(CGraphicDevice)

public:
	enum WINMODE { MODE_FULL, MODE_WIN };

private:
	explicit CGraphicDevice();
	~CGraphicDevice();
	// ���� ������, ����Ʈ ���Կ�����. 
	explicit CGraphicDevice(const CGraphicDevice& rObject) {};
	void operator=(const CGraphicDevice& rObject) {};

public:
	LPDIRECT3DDEVICE9	Get_Device() { return m_pDevice; }
	LPD3DXSPRITE		Get_Sprite() { return m_pSprite; }
	LPD3DXFONT			Get_Font() { return m_pFont; }
	LPD3DXLINE			Get_Line() { return m_pLine; }

public:
	// ��ü�� �����ϱ� ���� �İ�ü���� ���� �� �ʱ�ȭ �� ����. 
	HRESULT Ready_Graphic_Device(WINMODE eMode);
	void Release_Graphic_Device();
	void Render_Begin();
	void Render_End(HWND hWND = nullptr);

private:
	LPDIRECT3D9			m_pSDK;
	LPDIRECT3DDEVICE9	m_pDevice; // ���������� ��ġ�� �����ϴ� com ��ü. 
	LPD3DXSPRITE		m_pSprite;
	LPD3DXFONT			m_pFont;
	LPD3DXLINE			m_pLine;
};



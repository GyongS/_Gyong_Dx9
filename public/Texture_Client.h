#pragma once
class CTexture_Client
{
public:
	explicit CTexture_Client();
	virtual ~CTexture_Client();
public:
	virtual const TEXINFO* Get_TexInfo(const wstring& wstrStateKey = L"",
		const int& iIndex = 0)PURE;
	virtual HRESULT Insert_Texture(const wstring& wstrFilePath,
		const wstring& wstrStateKey = L"",
		const int& iIndex = 0)PURE;
	virtual void Release_Texture()PURE;
};


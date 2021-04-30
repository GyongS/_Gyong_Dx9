#pragma once
#include "Texture_Client.h"
class CMultiTexture_Client :
	public CTexture_Client
{
public:
	CMultiTexture_Client();
	virtual ~CMultiTexture_Client();

	virtual const TEXINFO * Get_TexInfo(const wstring & wstrStateKey = L"", const int & iIndex = 0) override;
	virtual HRESULT Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey = L"", const int & iIndex = 0) override;
	virtual void Release_Texture() override;

private:
	// 여기 들어가는 키값은 ? StateKey
	map<wstring, vector<TEXINFO*>> m_mapTexture;
};


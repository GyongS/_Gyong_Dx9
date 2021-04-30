#pragma once
#include "Texture_Client.h"
class CSingle_Texture_Client :
	public CTexture_Client
{
public:
	CSingle_Texture_Client();
	virtual ~CSingle_Texture_Client();

	virtual const TEXINFO * Get_TexInfo(const wstring & wstrStateKey = L"", const int & iIndex = 0) override;
	virtual HRESULT Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey = L"", const int & iIndex = 0) override;
	virtual void Release_Texture() override;

private:
	TEXINFO m_tTexInfo;
};


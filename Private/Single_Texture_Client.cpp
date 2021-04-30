#include "stdafx.h"
#include "Single_Texture_Client.h"
#include "GraphicDevice.h"

CSingle_Texture_Client::CSingle_Texture_Client()
{
}


CSingle_Texture_Client::~CSingle_Texture_Client()
{
	Release_Texture();
}

const TEXINFO * CSingle_Texture_Client::Get_TexInfo(const wstring & wstrStateKey, const int & iIndex)
{
	return &m_tTexInfo;
}

HRESULT CSingle_Texture_Client::Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey, const int & iIndex)
{
	if (FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &m_tTexInfo.tImageInfo)))
	{
		ERR_MSG(L"Loading Image Info Failed");
		return E_FAIL;
	}
	if (FAILED(D3DXCreateTextureFromFileEx(CGraphicDevice::Get_Instance()->Get_Device(),
		wstrFilePath.c_str(),
		m_tTexInfo.tImageInfo.Width,
		m_tTexInfo.tImageInfo.Height,
		m_tTexInfo.tImageInfo.MipLevels,
		0,
		m_tTexInfo.tImageInfo.Format,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		nullptr,
		nullptr,
		&m_tTexInfo.pTexture)))
	{
		wstring wstrErr = wstrFilePath + L"Loading Failed";
		ERR_MSG(wstrErr.c_str());
		return E_FAIL;
	}
	return S_OK;
}

void CSingle_Texture_Client::Release_Texture()
{
	if (m_tTexInfo.pTexture)
	{
		m_tTexInfo.pTexture->Release();
	}
}

#pragma once
class CTexture_Client;
class CTexture_Manager_Client
{
DECLARE_SINGLETON(CTexture_Manager_Client)

public:
	CTexture_Manager_Client();
	~CTexture_Manager_Client();
public:
	wstring Get_String() { return m_wstrFullPath; }
	void Set_String(const wstring& wstrFullPath) { m_wstrFullPath = wstrFullPath; }
public:
	HRESULT ReadImageFile(const wstring& wstrPath);
public:
	const TEXINFO* Get_TexInfo(const wstring& wstrObjectKey, const wstring& wstrStateKey = L"", const int& iIndex = 0);
	HRESULT Insert_Texture_Manager(TEX_ID::ID eTexID, const wstring& wstrFilePath, const wstring& wstrObjectKey, const wstring& wstrStateKey = L"", const int& iIndex = 0);
	void Release_Texture_Manager();

private:
	wstring m_wstrFullPath;
	// 여기에서 키값은 ObjectKey
	map<wstring, CTexture_Client*> m_mapTexture;
};


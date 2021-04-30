#include "stdafx.h"
#include "Texture_Manager_Client.h"
#include "Texture_Client.h"
#include "Single_Texture_Client.h"
#include "MultiTexture_Client.h"

IMPLEMENT_SINGLETON(CTexture_Manager_Client)

CTexture_Manager_Client::CTexture_Manager_Client()
{
}


CTexture_Manager_Client::~CTexture_Manager_Client()
{
	Release_Texture_Manager();
}

HRESULT CTexture_Manager_Client::ReadImageFile(const wstring & wstrPath)
{
	wifstream fin;
	fin.open(wstrPath);

	if (fin.fail())
	{
		ERR_MSG(L"�̹��� ��� �ҷ��ö� ����! Texture_Manager.cpp");
		return E_FAIL;
	}
	TCHAR szFilePath[MAX_PATH] = L"";
	TCHAR szObjectKey[MAX_PATH] = L"";
	TCHAR szStateKey[MAX_PATH] = L"";
	TCHAR szCount[MAX_PATH] = L"";
	DWORD dwCount = 0;

	while (true)
	{
		fin.getline(szFilePath, MAX_PATH, L'|');
		fin.getline(szObjectKey, MAX_PATH, L'|');
		fin.getline(szStateKey, MAX_PATH, L'|');
		fin.getline(szCount, MAX_PATH);

		if (fin.eof())
			break;
		dwCount = _ttoi(szCount);
		if (FAILED(Insert_Texture_Manager(TEX_ID::MULTI_TEX, szFilePath, szObjectKey, szStateKey, dwCount)))
		{
			ERR_MSG(L"���⼭ �����ߴٴ°� �� ���� ���, ������ ���ų�, ������Ʈ Ű ���� ��� �������� ");
			return E_FAIL;
		}

	}
	fin.close();

	//ERR_MSG(L"�ҷ����� �����߽�!"); 
	return S_OK;
}

const TEXINFO * CTexture_Manager_Client::Get_TexInfo(const wstring & wstrObjectKey, const wstring & wstrStateKey, const int & iIndex)
{
	auto& iter_Find = m_mapTexture.find(wstrObjectKey);
	if (m_mapTexture.end() == iter_Find)
		return nullptr;

	return iter_Find->second->Get_TexInfo(wstrStateKey, iIndex);
}

HRESULT CTexture_Manager_Client::Insert_Texture_Manager(TEX_ID::ID eTexID, const wstring & wstrFilePath, const wstring & wstrObjectKey, const wstring & wstrStateKey, const int & iIndex)
{
	auto& iter_Find = m_mapTexture.find(wstrObjectKey);
	// ó�� ���� ���� ��
	CTexture_Client* pTexture = nullptr;
	if (m_mapTexture.end() == iter_Find)
	{
		switch (eTexID)
		{
		case TEX_ID::MULTI_TEX:
			pTexture = new CMultiTexture_Client;
			break;
		case TEX_ID::SINGLE_TEX:
			pTexture = new CSingle_Texture_Client;
			break;
		default:
			break;
		}
		if (FAILED(pTexture->Insert_Texture(wstrFilePath, wstrStateKey, iIndex)))
		{
			ERR_MSG(L"TextureManager Insert Faield");
			return E_FAIL;
		}
		m_mapTexture.emplace(wstrObjectKey, pTexture);
	}
	else if (eTexID == TEX_ID::MULTI_TEX)
	{
		//m_mapTexture[wstrObjectKey] == CMultiTexture* pTexture
		//m_mapTexture[wstrObjectKey] ObjectKey = Player , CMultiTexture -> �ʿ� Statekey�� Attack, Dash - > ���� �׸� 6, 11 �� �ִ�. 
		if (FAILED(m_mapTexture[wstrObjectKey]->Insert_Texture(wstrFilePath, wstrStateKey, iIndex)))
		{
			ERR_MSG(L"TextureManager MultiTexture Insert Faield");
			return E_FAIL;
		}
	}
	return S_OK;
}

void CTexture_Manager_Client::Release_Texture_Manager()
{
	for (auto& rPair : m_mapTexture)
		Safe_Delete(rPair.second);

	m_mapTexture.clear();
}

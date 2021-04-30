#pragma once
typedef struct tagTexInfo
{
	LPDIRECT3DTEXTURE9 pTexture; // hBitmap�� ������ �����̶�� �����ϸ� �ǰڴ�. �׸������� �����ϱ� ���� �İ�ü. 
	D3DXIMAGE_INFO tImageInfo;// �̹����� ������ ���Ͽ� �о�� ������ ����ü. 
}TEXINFO;

typedef struct tagTile
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	BYTE byDrawID;
	BYTE byOption;
}TILE;

typedef struct tagTileCollision
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	BYTE byDrawID;
	BYTE byOption; 
}TILE_COLLISION;

typedef struct tagMapObject
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	BYTE byDrawID;
}MAPOBJECT;

typedef struct tagUnitInfo
{
#ifdef _AFX
	CString strName;
#else
	wstring strName;
#endif
	int		iAtt;
	int		iDef;
	BYTE	byJop;
	BYTE	byItem;
}UNITINFO;

typedef struct tagFrame
{
	float fStartFrame;
	float fEndFrame;
}FRAME;

typedef struct tagInfo
{
	D3DXVECTOR3	vPos;
	D3DXVECTOR3	vSize;
	D3DXVECTOR3	vDir;
	int iColCX;
	int iColCY;
}INFO;

typedef struct tagStat
{
	float fMaxHP;
	float fCurHP;
	float fSKill_A_Cool;
	float fSKill_A_MaxCool;
	float fSKill_S_Cool;
	float fSKill_S_MaxCool;
	float fSKill_D_Cool;
	float fSKill_D_MaxCool;
	float fSwap_Cool;
	float fSwap_MaxCool;
	float fDamage;
	int iGold;
	int iDarkQuartz;
}STAT;

typedef struct tagImagePath
{
	wstring wstrFilePath = L"";
	wstring wstrObjectKey = L"";
	wstring wstrStateKey = L"";
	DWORD dwCount = 0;
}IMAGEPATH;
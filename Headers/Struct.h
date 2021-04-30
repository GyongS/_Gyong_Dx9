#pragma once
typedef struct tagTexInfo
{
	LPDIRECT3DTEXTURE9 pTexture; // hBitmap과 동일한 역할이라고 생각하면 되겠다. 그림한장을 제어하기 위한 컴객체. 
	D3DXIMAGE_INFO tImageInfo;// 이미지의 정보를 파일에 읽어와 보관할 구조체. 
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
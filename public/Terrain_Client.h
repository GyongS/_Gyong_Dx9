#pragma once
#include "GameObject.h"
class CTerrain_Client : public CGameObject
{
public:
	CTerrain_Client();
	virtual ~CTerrain_Client();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void LoadTileData(const wstring& wstrFilePath);
	void Render_Decoration();
	void Render_Tile_Collision();
	void Render_MiniMap();
public:
	void Set_Ratio(D3DXMATRIX& matWorld, float fRatioX, float fRatioY);

public:
	vector<TILE*>& Get_Tile() { return m_vecTile; }
	vector<MAPOBJECT*>& Get_MapObject() { return m_vecMapObject; }
	vector<TILE_COLLISION*>& Get_Tile_Col() { return m_vecCollision; }

public:
	static CTerrain_Client* Create(const wstring& wstrFilePath);

private:
	vector<TILE*> m_vecTile;
	vector<MAPOBJECT*> m_vecMapObject;
	vector<TILE_COLLISION*> m_vecCollision;

};


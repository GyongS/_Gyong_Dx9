#pragma once
class CGameObject;
class CCollisionManager
{
public:
	CCollisionManager();
	~CCollisionManager();

public:
	static bool Check_Sphere(CGameObject* _Dst, CGameObject* _Src);
	static bool Check_ColRect(CGameObject* _Dst, CGameObject* _Src);
	static bool Check_ColRectEX(CGameObject* _Dst, CGameObject* _Src, float* _x, float* _y);
	static bool Check_TileRect(TILE_COLLISION* _Dst, CGameObject* _Src, float* _x, float* _y);

	static void Collision_Rect(list<CGameObject*>& _Dst, list<CGameObject*>& _Src);
	static void Collision_RectEx(list<CGameObject*>& _Dst, list<CGameObject*>& _Src);
	static void Collision_TIle(list<CGameObject*>& _Dst, list<CGameObject*>& _Src);

};


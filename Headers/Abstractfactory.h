#pragma once
#pragma once

class CGameObject;

template <typename T>
class CAbstractFactory
{
public:
	static CGameObject* Create()
	{
		CGameObject* pObj = new T;
		pObj->Ready_GameObject();

		return pObj;
	}
	static CGameObject* CreateTileCol(D3DXVECTOR3 vPos)
	{
		CGameObject* pObj = new T;
		pObj->Ready_GameObject();
		pObj->Set_ColPos(vPos);

		return pObj;
	}
	static CGameObject* Create(float _x, float _y)
	{
		CGameObject* pObj = new T;
		pObj->Ready_GameObject();
		pObj->Set_Pos(_x, _y);

		return pObj;
	}
	static CGameObject* Create(float _x, float _y, int _cx, int _cy)
	{
		CGameObject* pObj = new T;
		pObj->Ready_GameObject();
		pObj->Set_Pos(_x, _y);
		pObj->Set_Scale(_cx, _cy);

		return pObj;
	}
	static CGameObject* CreateCollision(float _x, float _y, int _cx = 0, int _cy = 0)
	{
		CGameObject* pObj = new T;
		pObj->Ready_GameObject();
		pObj->Set_Pos(_x, _y);
		pObj->Set_ColScale(_cx, _cy);
		return pObj;
	}
	static CGameObject* Create(float _x, float _y, float _fAngle)
	{
		CGameObject* pObj = new T;
		pObj->Ready_GameObject();
		pObj->Set_Pos(_x, _y);
		pObj->Set_Rotation(_fAngle);

		return pObj;
	}
	static CGameObject* Create(float _x, float _y, CGameObject* _pTarget)
	{
		CGameObject* pObj = new T;
		pObj->Ready_GameObject();
		pObj->Set_Pos(_x, _y);
		pObj->Set_Target(_pTarget);

		return pObj;
	}
};
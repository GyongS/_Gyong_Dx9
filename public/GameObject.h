#pragma once
class CGameObject
{
public:
	explicit CGameObject();
	virtual ~CGameObject();
public:
	virtual HRESULT Ready_GameObject()PURE;
	virtual HRESULT Late_Ready_GameObject();
	virtual int Update_GameObject()PURE;
	virtual void Late_Update_GameObject()PURE;
	virtual void Render_GameObject()PURE;
	virtual void Release_GameObject()PURE;

public:
	void Set_Dead() { m_bDead = true; }
	void Set_ColPos(D3DXVECTOR3 _vPos)
	{
		m_tInfo.vPos = _vPos;
	}
	void Set_Pos(float _x, float _y)
	{
		m_tInfo.vPos.x = _x;
		m_tInfo.vPos.y = _y;
	}
	void Set_PosX(float _x) { m_tInfo.vPos.x += _x; }
	void Set_PosY(float _y) { m_tInfo.vPos.y += _y; }
	void Set_ColScale(int _InScaleX, int _InScaleY)
	{
		m_tInfo.iColCX = _InScaleX;
		m_tInfo.iColCY = _InScaleY;
	}
	void Set_Rotation(float _rot) { m_fRotation = _rot; }
	void Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }
	void Set_ObjectKey(TCHAR* InObjectKey) { m_pObjectKey = InObjectKey; }
	void Set_OBJID(OBJ_ID::ID _eID) { m_eOBJID = _eID; }
	void Set_OBJTYPE(OBJ_TYPE::TYPE _eType) { m_eOBJ_TYPE = _eType; }
	void Set_AnimNum(int _Num) { m_iAnimNum = _Num; }
public:
	const INFO& Get_Info() const { return m_tInfo; }
	const STAT& Get_Stat() const { return m_tStat; }
	const OBJ_ID::ID& Get_OBJID() const { return m_eOBJID; }
	const OBJ_TYPE::TYPE& Get_OBJTYPE() const { return m_eOBJ_TYPE; }
	const TCHAR* Get_ObjectKey() const { return m_pObjectKey; }
	const TCHAR* Get_StateKey() const { return m_pStateKey; }
	const bool& Get_bLeftRight() const { return m_bLeftRight; }
	
private:
	bool m_bLateInit;

protected:
	void Update_ColRect();
	void FrameMove(float fSpeed = 1.f);

protected:
	INFO m_tInfo;
	STAT m_tStat;
	OBJ_ID::ID m_eOBJID;
	OBJ_TYPE::TYPE m_eOBJ_TYPE;
	FRAME m_tFrame;
	bool m_bLeftRight;
	bool m_bDead;

	const TCHAR* m_pStateKey;
	const TCHAR* m_pObjectKey;
    CGameObject* m_pTarget;

	int m_iAnimNum;
	float m_fFrameSpeed;
	float m_fSpeed;
	float m_fRotation;
};


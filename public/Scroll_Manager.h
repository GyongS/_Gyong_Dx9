#pragma once
class CScroll_Manager
{
public:
	CScroll_Manager();
	~CScroll_Manager();
public:
	static float Get_ScrollPos(SCROLL_ID::ID eScroll);
	static D3DXVECTOR3 Get_ScrollPos();
	static void Set_ScrollPos(const D3DXVECTOR3& vScroll);
	static void Set_bScrollShake(bool _bCheck) { m_bScrollShake = _bCheck; }
private:
	static D3DXVECTOR3 m_vScroll;

public:
	static void Scroll_Lock();
	static bool m_bScrollShake;
};


#pragma once
class CKey_Manager
{
	DECLARE_SINGLETON(CKey_Manager)
private:
	CKey_Manager();
	~CKey_Manager() = default;
public:
	void Update_KeyManager();
	bool Key_Up(DWORD dwKey);
	bool Key_Down(DWORD dwKey);
	bool Key_Pressing(DWORD dwKey);

private:
	DWORD m_dwKey;
	DWORD m_dwKeyUp;
	DWORD m_dwKeyDOWN;
};


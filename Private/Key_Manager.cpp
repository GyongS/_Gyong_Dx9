#include "stdafx.h"
#include "Key_Manager.h"

IMPLEMENT_SINGLETON(CKey_Manager)

CKey_Manager::CKey_Manager()
	:m_dwKey(0)
	, m_dwKeyUp(0)
	, m_dwKeyDOWN(0)
{
}

void CKey_Manager::Update_KeyManager()
{
	m_dwKey = 0;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwKey |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwKey |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwKey |= KEY_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwKey |= KEY_DOWN;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwKey |= KEY_SPACE;
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwKey |= KEY_RETURN;
	if (GetAsyncKeyState(0x5A) & 0x8000)
		m_dwKey |= KEY_Z;
	if (GetAsyncKeyState(0x58) & 0x8000)
		m_dwKey |= KEY_X;
	if (GetAsyncKeyState(0x43) & 0x8000)
		m_dwKey |= KEY_C;
	if (GetAsyncKeyState(0x41) & 0x8000)
		m_dwKey |= KEY_A;
	if (GetAsyncKeyState(0x53) & 0x8000)
		m_dwKey |= KEY_S;
	if (GetAsyncKeyState(0x44) & 0x8000)
		m_dwKey |= KEY_D;
	if (GetAsyncKeyState(0x46) & 0x8000)
		m_dwKey |= KEY_F;
	if (GetAsyncKeyState(0x51) & 0x8000)
		m_dwKey |= KEY_Q;

}

bool CKey_Manager::Key_Up(DWORD dwKey)
{
	if (m_dwKey & dwKey)
	{
		m_dwKeyUp |= dwKey;
		return false;
	}
	else if (m_dwKeyUp & dwKey)
	{
		// 0000 0111 
		// 0000 0100
		// 0000 0011
		m_dwKeyUp ^= dwKey;
		return true;
	}
	return false;
}

bool CKey_Manager::Key_Down(DWORD dwKey)
{
	if (m_dwKey & dwKey && !(m_dwKeyDOWN & dwKey))
	{
		m_dwKeyDOWN |= dwKey;
		return true;
	}
	else if (!(m_dwKey & dwKey) && (m_dwKeyDOWN & dwKey))
	{
		// 0000 0111 
		// 0000 0100
		// 0000 0011
		m_dwKeyDOWN ^= dwKey;
		return false;
	}
	return false;
}

bool CKey_Manager::Key_Pressing(DWORD dwKey)
{
	if (m_dwKey & dwKey)
		return true;

	return false;
}

#pragma once

#ifdef _AFX
#define ERR_MSG(msg) AfxMessageBox(msg)
#else
#define ERR_MSG(msg) MessageBox(nullptr, msg, L"System_Error", MB_OK)
#endif // _AFX

#define TILECX 32
#define TILECY 32

#define TILEX 60
#define TILEY 30

#define WINCX 800
#define WINCY 600

#define OBJ_NOEVENT 0
#define OBJ_DEAD    1

#define PI 3.141592

#define DECLARE_SINGLETON(ClassName)								\
public:																\
static ClassName* Get_Instance()									\
{																	\
	if (nullptr == m_pInstance)										\
		m_pInstance = new ClassName;								\
																	\
	return m_pInstance;												\
}																	\
static void Destroy_Instance()										\
{																	\
	if (m_pInstance)												\
	{																\
		delete m_pInstance;											\
		m_pInstance = nullptr;										\
	}																\
}																	\
private:															\
	static ClassName* m_pInstance;							

#define IMPLEMENT_SINGLETON(ClassName)								\
ClassName* ClassName::m_pInstance = nullptr;				


//0000 0001
#define KEY_LEFT	0x00000001

//0000 0010
#define KEY_RIGHT	0x00000002
#define KEY_UP		0x00000004
#define KEY_DOWN	0x00000008
#define KEY_SPACE	0x00000010
#define KEY_RETURN	0x00000020
#define KEY_Z       0x00000040
#define KEY_X       0x00000080
#define KEY_C       0X00000100
#define KEY_A       0X00000200
#define KEY_S       0X00000400
#define KEY_D       0X00000800
#define KEY_F       0X00001000
#define KEY_Q       0X00002000
#pragma once
class CSoundManager
{
public:
	static CSoundManager* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSoundManager;

		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
public:
	enum CHANNELID {
		BGM,
		COOLDOWN_END,
		// 플레이어
		DEFAULT_ATTACK_0_SFX,
		DEFAULT_ATTACK_1_SFX,
		ICECROSSBOW_READY_SFX,
		ICECROSSBOW_SHOT_SFX,
		BROADSWORD_READY_SFX,
		BROADSWORD_RELEASE,
		BROADSWORD_HIT,		
		MONSTER_DEAD,
		BIG_MONSTER_DEAD,
		PRISONER_ATTACK,
		PLAYER_ATTACK_SFX,
		ICE_PARTICLE,
		ICE_ARROW,
		FREEZE_STATE,
		DEAD,
		DASH,
		JUMP,
		DOUBLE_JUMP,
		GRENADE_SHOT,
		GRENADE_BOMB,
		DEFAULT_ATTACK,
		SAMURAI_ATTACK_0_SFX,
		SAMURAI_ATTACK_1_SFX,
		SAMURAI_ATTACK_2_SFX,
		SAMURAI_ILSEOM_START,
		SAMURAI_ILSEOM_END,
		SWAP_READY,
		PRISONER_SAWP,
		PRISONER_ATTACK_0,
		PRISONER_ATTACK_1,
		PRISONER_ATTACK_2,
		PRISONER_ATTACK_3,
		PRISONER_ATTACK_4,
		BOSS_STAMPING_READY,
		BOSS_STAMPING_NORMAL,
		BOSS_WHIRLWIND,
		BOSS_WAVE,
		BOSS_EARTHQUAKE_ATTACK,
		STONE_EFFECT,
		STONE_END,
		UI_TALK,
		MAXCHANNEL
	};
private:
	CSoundManager();
	~CSoundManager();

public:
	HRESULT Initialize();

	void Release();
public:
	void PlaySound(const TCHAR* pSoundKey, CHANNELID eID);
	void PlayBGM(const TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopBGM();
	void StopAll();

private:
	void LoadSoundFile();

private:
	static CSoundManager* m_pInstance;
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;

};


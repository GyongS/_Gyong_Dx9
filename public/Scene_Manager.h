#pragma once
class CScene;
class CScene_Manager
{
	DECLARE_SINGLETON(CScene_Manager)
public:
	enum ID
	{
		SCENE_LOADING,
		SCENE_STAGE1,
		SCENE_STAGE2,
		SCENE_STAGE3,
		SCENE_STAGE4,
		SCENE_STAGE5,
		SCENE_STAGE6,
		SCENE_STAGE7,
		BONUS_STAGE,
		SCENE_END
	};
private:
	CScene_Manager();
	~CScene_Manager();
public:
	void Scene_Change_SceneManager(ID eNextScene);
	const ID& Get_Scene() const { return m_eNextScene; }
public:
	void Update_Scene();
	void Late_Update_Scene();
	void Render_Scene();
	void Release_Scene();

private:
	ID m_eCurScene;
	ID m_eNextScene;
	CScene* m_pScene;
};


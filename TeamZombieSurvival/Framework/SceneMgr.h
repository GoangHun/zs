#pragma once
#include "Singleton.h"
#include "Scene.h"

//열거형 추후 이동 예정

class SceneMgr : public Singleton<SceneMgr>
{
	friend Singleton<SceneMgr>;

protected:
	SceneMgr() = default;
	virtual ~SceneMgr() override = default;

	std::vector<Scene*> scenes; //생성된 씬들을 담을 컨테이너
	SceneId currentSceneId = SceneId::None; //현재 씬Id 관리
	SceneId startSceneId = SceneId::Title; //제일 처음에 실행될 씬의 Id

	Scene* currentScene = nullptr;

	//선언과 동시에 초기화할 경우, 멤버 이니셜라이저를 통해서도 초기화 한다면 멤버 이니셜라이저로 초기화된다.
public:

	void ChangeScene(SceneId id); //씬을 변경하는 함수
	Scene* GetCurrScene() const;

	void Init();
	void Release();

	void Update(float dt);
	void Draw(sf::RenderWindow& window);
};

#define SCENE_MGR (SceneMgr::Instance())


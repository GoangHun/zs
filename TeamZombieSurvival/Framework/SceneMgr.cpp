#include "stdafx.h"
#include "SceneMgr.h"
#include "SceneDev1.h"
#include "SceneTitle.h"

void SceneMgr::ChangeScene(SceneId id)
{
	currentScene->Exit(); //현재 열려있는 씬 종료
	currentSceneId = id; 
	currentScene = scenes[(int)currentSceneId];
	currentScene->Enter(); //새로 열 씬 시작
}

Scene* SceneMgr::GetCurrScene() const
{
	return currentScene;
}

void SceneMgr::Init()
{
	if (!scenes.empty())
	{
		Release();
	}
	scenes.push_back(new SceneTitle());
	scenes.push_back(new SceneDev1());
	

	for (auto scene : scenes)
	{
		scene->Init(); //각 씬 초기화
	}

	currentSceneId = startSceneId; //현재 씬을 시작 씬 id로 초기화
	currentScene = scenes[(int)currentSceneId];
	currentScene->Enter(); //현재(시작) 씬 시작
}

void SceneMgr::Release()
{
	if (scenes.empty())
	{
		return;
	}
	for (auto scene : scenes)
	{
		delete scene; 
	}
	scenes.clear(); //벡터 비워주기

	currentSceneId = SceneId::None;
	currentScene = nullptr;
}

void SceneMgr::Update(float dt)
{
	currentScene->Update(dt);
}

void SceneMgr::Draw(sf::RenderWindow& window)
{
	currentScene->Draw(window);
}

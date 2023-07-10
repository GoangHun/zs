#include "stdafx.h"
#include "Scene.h"
#include "GameObject.h"
#include "ResourceMgr.h"
#include "Framework.h"

Scene::Scene(SceneId id) : sceneId(id), window(FRAMEWORK.GetWindow())
{
}

Scene::~Scene()
{

}

//void Scene::Init()
//{
//	Release();
//
//	for (auto go : gameObjects)
//	{
//		go->Init();
//	}
//}
//
//void Scene::Release()
//{
//	for (auto go : gameObjects)
//	{
//		//go->Release(); GeamObjects의 소멸자에서 release 하는 것으로 변경
//		delete go;
//	}
//	gameObjects.clear();
//}

void Scene::Enter()
{
	RESOURCE_MGR.Load(resources);
	//시작할 때마다 초기 상태로 변경
	for (auto go : gameObjects)
	{
		go->Reset();
	}
}

void Scene::Exit()
{
	for (auto go : removeGameObjects)
	{
		gameObjects.remove(go);
	}
	removeGameObjects.clear();

	RESOURCE_MGR.UnLoad(resources);
}

void Scene::Update(float dt)
{
	for (auto go : gameObjects)
	{
		if (go->GetActive()) //활성화 된 것 만 업데이트
		{
			go->Update(dt);
		}
	}

	for (auto go : removeGameObjects)
	{
		gameObjects.remove(go);
	}
	removeGameObjects.clear();
}

void Scene::Draw(sf::RenderWindow& window)
{
	SortGos(); //draw 전에 정렬 한 번 하기
	window.setView(worldView);
	for (auto go : gameObjects)
	{
		if (go->sortLayer >= 100)
			//return;
			continue;

		if (go->GetActive()) //활성화 된 것 만 업데이트
		{
			go->Draw(window);
		}
	}

	//ui의 sortLayer는 100이상
	window.setView(uiView);
	for (auto go : gameObjects)
	{
		if (go->sortLayer < 100)
			//return;
			continue;

		if (go->GetActive()) //활성화 된 것 만 업데이트
		{
			go->Draw(window);
		}
	}
}

GameObject* Scene::FindGo(const string& name)
{
	auto compare = [name](GameObject* go) {return go->GetName() == name; };
	auto it = find_if(gameObjects.begin(), gameObjects.end(), compare);
	if (it == gameObjects.end())
		return nullptr;
	return *it;

	/*위의 람다 표현식을 사용한 것 과 택1
	for (auto go : gameObjects)
	{
		if (go->GetName() == name)
		{
			return go;
		}
	}
	return nullptr;*/
}

void Scene::FindGo(list<GameObject*>& list, const string& name)
{
	//list.clear(); //리스트에 기존에 담긴 것 삭제, 선택옵션
	for (auto go : gameObjects)
	{
		if (go->GetName() == name)
		{
			list.push_back(go);
		}
	}
}

bool Scene::Exist(GameObject* go)
{
	return find(gameObjects.begin(), gameObjects.end(), go) != gameObjects.end();
}

GameObject* Scene::AddGo(GameObject* go)
{
	if (!Exist(go)) //존재하지 않는 경우에만 넣기
	{
		gameObjects.push_back(go);
	}
	return go;
}

void Scene::RemoveGo(GameObject* go)
{
	removeGameObjects.push_back(go);
	//delete go; 바깥에서 하는 것으로 수정
}

void Scene::SortGos()
{
	gameObjects.sort([](GameObject* lhs, GameObject* rhs) {
		if (lhs->sortLayer != rhs->sortLayer)
			return lhs->sortLayer < rhs->sortLayer;
		return lhs->sortOrder< rhs->sortOrder; 
		});
	//람다표현식을 사용해서 정렬
}

sf::Vector2f Scene::ScreenToWorldPos(sf::Vector2f screenPos)
{
	return window.mapPixelToCoords((sf::Vector2i)screenPos, worldView);
}

sf::Vector2f Scene::ScreenToUiPos(sf::Vector2f screenPos)
{
	return window.mapPixelToCoords((sf::Vector2i)screenPos, uiView);
}

sf::Vector2f Scene::WorldPosToScreen(sf::Vector2f worldPos)
{
	return (sf::Vector2f)window.mapCoordsToPixel(worldPos, worldView);
}

sf::Vector2f Scene::UiPosToScreen(sf::Vector2f uiPos)
{
	return (sf::Vector2f)window.mapCoordsToPixel(uiPos, uiView);
}

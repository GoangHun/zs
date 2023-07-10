#pragma once

class GameObject;
class Scene
{
protected:
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	SceneId sceneId;

	vector<tuple<ResourceTypes, string>> resources;
	list<GameObject*> gameObjects; 
	list<GameObject*> removeGameObjects; 

	//중복이 들어가지 않도록 관리 -> 함수를 통해서 넣고 빼기

	list<GameObject*> gameoObjects;
	sf::View worldView;
	sf::View uiView;

	sf::RenderWindow& window;

public:
	Scene(SceneId id = SceneId::None);
	virtual ~Scene();

	//씬을 생성하고 삭제할 때 호출할 함수
	//상속 받은 씬에서 각자 하기로 수정
	virtual void Init() = 0;
	virtual void Release() = 0;

	//씬을 재생하고 종료할 때 호출할 함수
	virtual void Enter();
	virtual void Exit();

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);

	GameObject* FindGo(const string& name); 
	//이름이 중복되면 제일 처음의 것 리턴

	void FindGo(list<GameObject*>& list, const string& name);
	//이름에 해당하는 모든 게임오브젝트를 리스트에 담기

	bool Exist(GameObject* go);
	GameObject* AddGo(GameObject* go);
	void RemoveGo(GameObject* go);
	void SortGos(); //정렬

	sf::Vector2f ScreenToWorldPos(sf::Vector2f screenPos); //스크린 좌표->월드좌표계 변환
	sf::Vector2f ScreenToUiPos(sf::Vector2f screenPos); //스크린->ui좌표계 변환

	sf::Vector2f WorldPosToScreen(sf::Vector2f worldPos); //스크린 좌표->월드좌표계 변환
	sf::Vector2f UiPosToScreen(sf::Vector2f uiPos);
};


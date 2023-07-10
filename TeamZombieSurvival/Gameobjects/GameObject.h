#pragma once
#include "Utils.h"

/*utils, gameobjects std::삭제 필요*/
class GameObject
{
protected:
	string name;
	sf::Vector2f position;
	bool isActive = true; //true이면 활성화된 GameObject
	Origins origin = Origins::TL;

public:

	int sortLayer = 0;
	int sortOrder = 0;

	GameObject(const string n = "");
	virtual ~GameObject();

	bool GetActive() const;
	void SetActive(bool active);

	std::string GetName();
	virtual void SetName(const std::string& n);

	sf::Vector2f GetPosition();

	virtual void SetPosition(const sf::Vector2f& p);
	virtual void SetPosition(float x, float y);

	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float x, float y);

	virtual void Init() = 0; //동적 할당(생성자)
	virtual void Release() {}; //동적 할당 해제(소멸자)

	virtual void Reset() = 0; //초기화, 동적할당 해제X

	virtual void Update(float dt) = 0;
	virtual void Draw(sf::RenderWindow& window) = 0; 
};


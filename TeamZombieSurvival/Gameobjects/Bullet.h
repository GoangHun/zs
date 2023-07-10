#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"
/*플레이어가 생성해서 날리는 것
충돌 체크: 좀비*/

class Zombie;
class Bullet : public SpriteGo
{
protected:

	sf::Vector2f direction;
	float speed = 0.f;
	float range = 2000.f;
	int damage = 25;


	const list<Zombie*>* zombies;
	
	//ObjectPool<Bullet>* pool; public으로 임시 전환
public:
	ObjectPool<Bullet>* pool;

	Bullet(const string& textureId = "", const string& n = "");
	virtual ~Bullet() override;

	void SetZombieList(const list<Zombie*>* list);
	void Fire(const sf::Vector2f pos, const sf::Vector2f direction, float speed); 
	//매개변수: 시작 위치, 방향, 속도, 일정 시간or거리가 지나면 총알이 사라지도록

	virtual void Init() override; //동적 할당(생성자)
	virtual void Release() override; //동적 할당 해제(소멸자)
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetDamage(int num) { damage = num; }
	int GetDamage() { return damage; }

};


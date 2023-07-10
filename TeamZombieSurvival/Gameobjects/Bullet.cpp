#include "stdafx.h"
#include "Bullet.h"
#include "Zombie.h"
#include "SceneMgr.h"
#include "Scene.h"

Bullet::Bullet(const string& textureId, const string& n)
	:SpriteGo(textureId, n)
{
}

Bullet::~Bullet()
{
}

void Bullet::SetZombieList(const list<Zombie*>* list)
{
	zombies = list;
}

void Bullet::Fire(const sf::Vector2f pos, const sf::Vector2f direction, float speed)
{
	sprite.setRotation(Utils::Angle(direction));
	SetPosition(pos);
	
	this->direction = direction;
	this->speed = speed;

}

void Bullet::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
}

void Bullet::Release()
{
	SpriteGo::Release();
}

void Bullet::Reset()
{
	SpriteGo::Reset();

	sprite.setRotation(0.f);
	SetPosition(0.f, 0.f);
	speed = 0.f;
	direction = { 0.f, 0.f };
	range = 2000.f;
}

void Bullet::Update(float dt)
{
	SpriteGo::Update(dt);

	//회전은 고정
	range -= speed * dt; // 이동량 빼기
	if (range <= 0.f) //사거리까지 이동하면 총알 삭제
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(this);
		pool->Return(this); 
		return;
	}

	position += direction * speed * dt;
	SetPosition(position);

	if (zombies != nullptr)
	{
		for (Zombie* zombie : *zombies)
		{
			if (sprite.getGlobalBounds().intersects(zombie->sprite.getGlobalBounds()))
			{
				zombie->OnHitBullet(damage);
				//총알과 좀비가 충돌하면 총알 삭제
				SCENE_MGR.GetCurrScene()->RemoveGo(this);
				pool->Return(this);
				break;
			}
		}
	}
}

void Bullet::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"
#include "Bullet.h"
#define DASH INPUT_MGR.GetKey(sf::Keyboard::LShift) 
#define MOVEUP INPUT_MGR.GetKey(sf::Keyboard::W) 
#define MOVERIGHT INPUT_MGR.GetKey(sf::Keyboard::D) 
#define MOVELEFT INPUT_MGR.GetKey(sf::Keyboard::A) 
#define MOVEDOWN INPUT_MGR.GetKey(sf::Keyboard::S)

#include "Item.h"
class Zombie;

class Player : public SpriteGo
{
protected:
	sf::Vector2f look; //바라보는 방향
	sf::Vector2f direction;
	float speed;
	int maxHp = 100;
	int hp = 0;
	int maxAmmo = 5;
	int ammo = 0;
	//스태미너 추가
	float maxStamina = 10.f;
	float stamina = 0.f;
	float useStamina = 5.f;
	float healStamina = 2.f;
	float shotDelay = 0.5f;
	float timer = 0.f;
	bool rebound = false;
	int shotBulletNum = 1;


	float CollisionTimer = 0.0f; 
	float Interval = 1.f; //체력 깎을 간격

	sf::FloatRect wallBounds;
	sf::Vector2f wallBoundsLT;
	sf::Vector2f wallBoundsRB;

	// 순국 추가
	sf::SoundBuffer soundBufferShoot;
	sf::SoundBuffer soundBufferHit;
	sf::SoundBuffer soundBufferReloadFail;
	sf::SoundBuffer soundBufferReload;

	sf::Sound soundShoot;
	sf::Sound soundHit;
	sf::Sound soundReloadFail;
	sf::Sound soundReload;

	sf::Clock clock; //경과시간을 측정하는 객체
	const sf::Time reloadTime = sf::seconds(3.f); //3초 간격 장전
	bool reloadTimeCheck = false;

	//추가 끝

	ObjectPool<Bullet> poolBullets;
public:
	Player(const string textureId = "", const string n = "");
	virtual ~Player() override {};

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetZombieList(list<Zombie*>* list);
	void OnHitZombie(int damage);
	void TakeItem(Item::ItemTypes type);

	void OnDie();

	void SetWallBounds(const sf::FloatRect& bounds);
	int GetMaxAmmo();
	int GetAmmo();
	int GetHp();
	int GetshotBulletNum();

	void StatUp(int num);
	void DamageUp(int damage);
	void ShotSpeedUp(float shotDelay);
	void AddShotBullet();

	ObjectPool<Bullet> GetBulletPool() { return poolBullets; }

	int GetMaxHp();
	float GetStamina();
	float GetMaxStamina();
	bool isAlive = false;
};


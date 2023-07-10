#pragma once
#include "SpriteGo.h"

class Player;
class Zombie : public SpriteGo
{
public:
	//파일 입출력으로 대체 가능
	enum class Types
	{
		Bloater,
		Chaser,
		Crawler
	};

	static const int TotalTypes = 3;
	static const string textureIds[3];
	static const float speedStats[3];
	static const int hpStats[3];
	static const int damages[3];
	static const float attackRates[3];
	static const int scores[3];
	
protected:
	Types zombieType;

	sf::Vector2f look; //바라보는 방향
	sf::Vector2f direction; //이동하는 방향
	float speed = 0.f; //이동 속도
	int maxHp = 0;
	int hp = 0;

	int damage = 0;
	float attackRate = 0.f; //공격 주기
	float attackTimer = 0.f; //timer가 rate보다 커지면 공격 타이밍
	int score = 0;

	Player* player = nullptr; //플레이어를 쫓아가야하기 때문에 필요

	//순국 추가
	sf::SoundBuffer soundBufferSplat;

	sf::Sound soundSplat;
	// 추가 끝

public:

	Zombie(const string& n = "");
	virtual ~Zombie() override;

	virtual void Init() override; //동적 할당(생성자)
	virtual void Release() override; //동적 할당 해제(소멸자)
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetType(Types t);
	Types GetType() const;

	void SetPlayer(Player* p);

	void OnHitBullet(int damage); //총알에 맞았을 때
	int GetDamage();

	int GetScore();
};


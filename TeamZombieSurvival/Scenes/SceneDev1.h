#pragma once
#include "Scene.h"
#include "ObjectPool.h"

class Player;
class VertexArrayGo;
class Zombie;
class SpriteEffect;
class Item;
class TextGo;
class SpriteGo;

class SceneDev1 :public Scene
{
protected:
	Player* player;
	
	VertexArrayGo* background;
	sf::FloatRect wallBounds;
	TextGo* fpsGo;
	TextGo* HpGo;
	TextGo* NoBulletGo;
	TextGo* waveGo;
	sf::Clock clock;
	const sf::Time blinkTime = sf::seconds(0.3f);

	ObjectPool<Zombie> zombiePool;
	ObjectPool<SpriteEffect> bloodEffectPool;
	ObjectPool<Item> itemPool;

	bool isGameOver;
	int score = 0;
	int HiScore = 0;
	int wave = 0;
	//bool isPause = true;
	int fps = 0;
	float totalDt = 0.f;
	float duration = 1.5f;
	float timer = 0.f;
	bool transparency = false;
	int countDown = 3;

	sf::Vector2f staminaSize = { 100.f, 5.f };
public:
	SceneDev1();
	virtual ~SceneDev1() override;
	virtual void Init() override;
	virtual void Release() override;

	//씬을 재생하고 종료할 때 호출할 함수
	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	VertexArrayGo* CreateBackGround(sf::Vector2i size, sf::Vector2f tileSize, sf::Vector2f texSize, string textureId);
	
	void SpawnZombies(int count, sf::Vector2f center, float radius);

	template <typename T>
	void ClearObjectPool(ObjectPool<T>& pool);

	void OnDieZombie(Zombie* zombie); //좀비가 죽었을 때 씬에서 해야할 일
	void TakeItem(Item* item);

	const list<Zombie*>* GetZombieList() const;

	void OnDiePlayer();
	VertexArrayGo* GetBackground();

	void StaminaControl(float dt);
	void StaminaSet();
	void NextWave(SpriteGo* statUpBg, TextGo* statUpText);
};

template<typename T>
inline void SceneDev1::ClearObjectPool(ObjectPool<T>& pool)
{
	for (auto obj : pool.GetUseList())
	{
		RemoveGo(obj);
	}
	pool.Clear();
}

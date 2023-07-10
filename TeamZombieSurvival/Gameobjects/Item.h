#pragma once
#include "SpriteGo.h"

class Player;

class Item : public SpriteGo
{
public:
	enum class ItemTypes
	{
		Ammo, //총알
		Potion //체력키트
	};
protected:

	bool isSpawn = false;
	ItemTypes type;
	Player* player = nullptr;

public:
	
	static const int TotalTypes = 2;
	Item(const string& textureId = "", const string& n = "");
	virtual ~Item() override;

	void SetType(ItemTypes t);
	ItemTypes GetType() const;
	void SetPlayer(Player* p);

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};


#include "stdafx.h"
#include "Item.h"
#include "Player.h"
#include "SceneDev1.h"
#include "SceneMgr.h"

Item::Item(const string& textureId, const string& n)
	:SpriteGo(textureId, n)
{
}

Item::~Item()
{
}

void Item::SetType(ItemTypes t)
{
	type = t;
}

Item::ItemTypes Item::GetType() const
{
	return Item::ItemTypes();
}

void Item::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
}

void Item::Release()
{
	SpriteGo::Release();
}

void Item::Reset()
{
	SpriteGo::Reset();
}

void Item::SetPlayer(Player* p)
{
	player = p;
}

void Item::Update(float dt)
{
	SpriteGo::Update(dt);

	if (player->isAlive && sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds()))
	{
		player->TakeItem(type);

		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene); //c++의 형변환 연산자
		if (sceneDev1 != nullptr)
		{
			sceneDev1->TakeItem(this);
		}
	}

}

void Item::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

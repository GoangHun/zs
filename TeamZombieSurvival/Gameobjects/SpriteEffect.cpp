#include "stdafx.h"
#include "SpriteEffect.h"
#include "SceneMgr.h"
#include "Utils.h"


SpriteEffect::SpriteEffect(const string& textureId, const string& n)
	:SpriteGo(textureId, n)
{
}

void SpriteEffect::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
}

void SpriteEffect::Reset()
{
	SpriteGo::Reset();
	timer = 0.f;
	sprite.setColor({255, 255, 255, 255}); //색상을 기본으로 돌리기 ,255가 기본값
}


void SpriteEffect::Update(float dt)
{
	timer += dt;

	//UINT8 a = ((duration - timer) / duration) * 255;
	
	//--float lerp 사용--
	//UINT8 a = Utils::Lerp(0, 255, (duration - timer) / duration);
	//sprite.setColor({ 255, 255, 255, a });

	//--color lerpo 사용---
	sf::Color a = { 255,255,255,0 };
	sf::Color b = sf::Color::White;
	sf::Color color = Utils::Lerp(a, b, (duration - timer) / duration);
	sprite.setColor(color); //점점 투명해지도록

	//--vecter2f lerp 사용--
	//UINT8 a = Utils::Lerp(255, 0, (timer / duration));
	//SetPosition(Utils::Lerp(position, { 0, 0 }, (timer / duration), false));


	if (timer > duration) //일정 시간이 지나면 삭제
	{
		if (pool != nullptr)
		{
			SCENE_MGR.GetCurrScene()->RemoveGo(this);
			pool->Return(this);
		}
		else
		{
			SetActive(false);
		}
	}
}

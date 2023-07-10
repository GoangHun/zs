#pragma once
#include "Scene.h"

class TextGo;
class SceneTitle : public Scene
{
protected:
	sf::View titleView;
	sf::Vector2f screenSize;
	TextGo* title;
	sf::Clock clock;
	const sf::Time blinkTime = sf::seconds(0.4f);
	bool blinkTimeCheck = false;
public:
	SceneTitle();
	virtual ~SceneTitle() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void BlinkTitle();
};
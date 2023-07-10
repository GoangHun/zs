#include "stdafx.h"
#include "RectangleGo.h"

RectangleGo::RectangleGo(sf::Vector2f& size, const string& n)
	:GameObject(n)
{
	rectangle.setSize(size);
}

RectangleGo::~RectangleGo()
{
}

void RectangleGo::SetPosition(const sf::Vector2f& p)
{
	position = p;
	rectangle.setPosition(p);
}

void RectangleGo::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	rectangle.setPosition(position);
}

void RectangleGo::SetOrigin(Origins origin)
{
	Utils::SetOrigin(rectangle, origin);
}

void RectangleGo::SetOrigin(float x, float y)
{
	rectangle.setOrigin(x, y);
}

void RectangleGo::Init()
{
}

void RectangleGo::Release()
{
}

void RectangleGo::Reset()
{
}

void RectangleGo::Update(float dt)
{
}

void RectangleGo::Draw(sf::RenderWindow& window)
{
	window.draw(rectangle);
}

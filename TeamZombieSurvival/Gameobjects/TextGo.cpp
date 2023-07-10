#include "stdafx.h"
#include "TextGo.h"
#include "ResourceMgr.h"
#include <sstream>

TextGo::TextGo(const string& n, const std::string& rId)
	:GameObject(n), resourceId(rId)
{
}

TextGo::~TextGo()
{
}

void TextGo::SetPosition(const sf::Vector2f& p)
{
	position = p;
	text.setPosition(p);
}

void TextGo::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	text.setPosition(position);
}

void TextGo::SetOrigin(Origins origin)
{
	Utils::SetOrigin(text, origin);
}

void TextGo::SetOrigin(float x, float y)
{
	text.setOrigin(x, y);
}

void TextGo::SetSize(float w, float h)
{
	text.setScale(w, h);
}

void TextGo::Init()
{
}

void TextGo::Release()
{
}

void TextGo::Reset()
{
	auto font = RESOURCE_MGR.GetFont(resourceId);
	if (font != nullptr)
	{
		text.setFont(*font);
	}
}

void TextGo::Update(float dt)
{
}

void TextGo::Draw(sf::RenderWindow& window)
{
	window.draw(text);
}

void TextGo::SetText(const std::string& ss, const int& size, sf::Color color, Origins origin, int soltLayer, float x, float y)
{
	text.setCharacterSize(size);
	text.setString(ss);
	text.setFillColor(color);
	text.setPosition(x, y);
	Utils::SetOrigin(text, origin);
	this->sortLayer = soltLayer;
}

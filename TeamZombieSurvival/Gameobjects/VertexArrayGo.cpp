#include "stdafx.h"
#include "VertexArrayGo.h"
#include "ResourceMgr.h"

VertexArrayGo::VertexArrayGo(const string& textureId, const string& n)
	:GameObject(n), textureId(textureId), texture(nullptr)
{
}

VertexArrayGo::~VertexArrayGo()
{
}

void VertexArrayGo::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);
	
	if (origin == Origins::CUSTOM)
		return;

	/*prevOrigin: 기존의 오리진, originPosition: 변경할 오리진*/
	sf::Vector2f prevOrigin = originPosition;
	sf::FloatRect bounds = vertexArray.getBounds();
	originPosition.x = bounds.width * ((int)origin % 3) * 0.5f;
	originPosition.y = bounds.height * ((int)origin / 3) * 0.5f;

	sf::Vector2f diff = prevOrigin - originPosition; //기존-변경으로 변화량 구하기
	for (int i = 0; i < vertexArray.getVertexCount(); i++)
	{
		vertexArray[i].position += diff; //모든 정점마다 변화량 더해주기
	}
}

void VertexArrayGo::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);

	sf::Vector2f prevOrigin = originPosition;
	originPosition.x = x;
	originPosition.y = y;

	sf::Vector2f delta = prevOrigin - originPosition;
	for (int i = 0; i < vertexArray.getVertexCount(); i++)
	{
		vertexArray[i].position += delta;
	}
}

void VertexArrayGo::SetPosition(const sf::Vector2f& p)
{
	/*편재 포지션에서 변경할 포지션 사이의 차를 구하면 변화량을 알 수 잇음.
	이 변화량을 각 정점에 적용해주면 포지션이 변경됨.*/
	sf::Vector2f delta = p - position; //변화량
	position = p;

	for (int i = 0; i < vertexArray.getVertexCount(); i++)
	{
		vertexArray[i].position += delta;
	}

}

void VertexArrayGo::SetPosition(float x, float y)
{
	SetPosition({x, y});
}

void VertexArrayGo::Init()
{
}

void VertexArrayGo::Release()
{
}

void VertexArrayGo::Reset()
{
	texture = RESOURCE_MGR.GetTexture(textureId);
}

void VertexArrayGo::Update(float dt)
{
	texture = RESOURCE_MGR.GetTexture(textureId);
}

void VertexArrayGo::Draw(sf::RenderWindow& window)
{
	window.draw(vertexArray, texture);
}

//sf::Vector2f VertexArrayGo::GetOriginOffset()
//{
//	sf::FloatRect bounds = vertexArray.getBounds();
//	sf::Vector2f originOffset;
//
//	switch (origin)
//	{
//	case Origins::TL:
//		originOffset = sf::Vector2f(0, 0);
//		break;
//	case Origins::TC:
//		originOffset = sf::Vector2f(bounds.width / 2.0f, 0);
//		break;
//	case Origins::TR:
//		originOffset = sf::Vector2f(bounds.width, 0);
//		break;
//	case Origins::ML:
//		originOffset = sf::Vector2f(0, bounds.height / 2.0f);
//		break;
//	case Origins::MC:
//		originOffset = sf::Vector2f(bounds.width / 2.0f, bounds.height / 2.0f);
//		break;
//	case Origins::MR:
//		originOffset = sf::Vector2f(bounds.width, bounds.height / 2.0f);
//		break;
//	case Origins::BL:
//		originOffset = sf::Vector2f(0, bounds.height);
//		break;
//	case Origins::BC:
//		originOffset = sf::Vector2f(bounds.width / 2.0f, bounds.height);
//		break;
//	case Origins::BR:
//		originOffset = sf::Vector2f(bounds.width, bounds.height);
//		break;
//	default:
//		originOffset = sf::Vector2f(0, 0);
//		break;
//	}
//
//	return originOffset;
//}

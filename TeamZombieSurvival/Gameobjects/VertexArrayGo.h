#pragma once
#include "GameObject.h"
class VertexArrayGo : public GameObject
{
protected:
	string textureId;
	sf::Texture* texture;
	sf::Vector2f originPosition;
public:
	sf::VertexArray vertexArray; //±×¸®±â °´Ã¼

	VertexArrayGo(const string& textureId = "", const string& n = "");
	virtual ~VertexArrayGo() override;

	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float x, float y);

	virtual void SetPosition(const sf::Vector2f& p);
	virtual void SetPosition(float x, float y);

	virtual void Init(); 
	virtual void Release(); 

	virtual void Reset(); 

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);

	//sf::Vector2f GetOriginOffset();
};


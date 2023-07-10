#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "Defines.h"


class Utils
{
private:
	static std::random_device rd;
	static std::mt19937 gen;

public:
	static int RandomRange(int min, int maxExclude);
	static float RandomRange(float min, float max);

	static sf::Vector2f RandomOnCircle(float radius);
	static sf::Vector2f RandomInCircle(float radius);
	static float RandomValue(); //0.0f~1.0f 값 리턴

	static void SetOrigin(sf::Sprite& sprite, Origins origin);
	static void SetOrigin(sf::Text& text, Origins origin);
	static void SetOrigin(sf::Shape& shape, Origins origin);
	static void SetOrigin(sf::Transformable& obj, Origins origin, const sf::FloatRect& rect);

	/*---클램프 연산---*/
	static float Clamp(float v, float min, float max);
	static sf::Vector2f Clamp(const sf::Vector2f& v, const sf::Vector2f& min, const sf::Vector2f& max);

	//선형보간
	static float Lerp(float min, float max, float t, bool clamping = true);
	static sf::Vector2f Lerp(const sf::Vector2f& min, const sf::Vector2f& max, float t, bool clamping = true);
	static sf::Color Lerp(const sf::Color& min, const sf::Color& max, const float t, bool clamping = true);

	/*----벡터 관련 함수----*/
	static float SqrMagnitude(const sf::Vector2f& vec); //양변 제곱의 합
	static float Magnitude(const sf::Vector2f& vec); //양변 제곱의 합의 루트 = 벡터의 길이

	//정규화, 노말라이즈
	static sf::Vector2f Normalize(const sf::Vector2f& vec);

	//2개의 포지션 사이의 길이를 구하는 함수
	static float Distance(const sf::Vector2f& pos1, const sf::Vector2f& pos2);

	//사이각을 구하는 함수
	//대부분은 라디안 단위(180도가 파이)이므로 이를 degree 단위로 변환한 후 반환
	static float Angle(const sf::Vector2f& dir); 
	static float Angle(const sf::Vector2f& start, const sf::Vector2f& end);
	
	static sf::Vector2f Direction(const float angle);
};




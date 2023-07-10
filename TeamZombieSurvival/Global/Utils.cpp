#include "stdafx.h"
#include "Utils.h"


std::random_device Utils::rd;
std::mt19937 Utils::gen(Utils::rd());

int Utils::RandomRange(int min, int maxExclude)
{
	std::uniform_int_distribution<> dist(min, maxExclude - 1);
	return dist(gen);
}

float Utils::RandomRange(float min, float max)
{
	std::uniform_real_distribution<> dist(min, max);
	return (float)dist(gen);
}

sf::Vector2f Utils::RandomOnCircle(float radius)
{
	sf::Vector2f vec(RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f)); //중점을 기준으로 가로세로가 2인 정사각형
	vec = Normalize(vec) * radius; //정규화, 중점을 기준으로 반지름이 1인 원으로 만들어서 반경만큼 곱하기
	return vec;
}

sf::Vector2f Utils::RandomInCircle(float radius)
{
	return RandomOnCircle(radius) * RandomValue();
}

float Utils::RandomValue()
{
	return RandomRange(0.0f, 1.0f);
}

void Utils::SetOrigin(sf::Sprite& sprite, Origins origin)
{
	SetOrigin(sprite, origin, sprite.getLocalBounds());
}

void Utils::SetOrigin(sf::Text& text, Origins origin)
{
	SetOrigin(text, origin, text.getLocalBounds());
}

void Utils::SetOrigin(sf::Shape& shape, Origins origin)
{
	SetOrigin(shape, origin, shape.getLocalBounds());
}

void Utils::SetOrigin(sf::Transformable& obj, Origins origin, const sf::FloatRect& rect)
{
	sf::Vector2f originPos(rect.width, rect.height);
	originPos.x *= ((int)origin % 3) * 0.5f;
	originPos.y *= ((int)origin / 3) * 0.5f;
	obj.setOrigin(originPos);
}

float Utils::Clamp(float v, float min, float max)
{
	//ver1
	return std::max(min, std::min(max, v));


	/*ver2
	if (v < min)
	{
		return min;
	}
	if (v > max)
	{
		return max;
	}
	return v;*/
}

sf::Vector2f Utils::Clamp(const sf::Vector2f& v, const sf::Vector2f& min, const sf::Vector2f& max)
{
	sf::Vector2f result;
	result.x = Clamp(v.x, min.x, max.x);
	result.y = Clamp(v.y, min.y, max.y);

	return result;
}

float Utils::Lerp(float a, float b, float t, bool clamping)
{
	float v = a + (b - a) * t;
	if (clamping)
	{
		v = Utils::Clamp(v, std::min(a, b), std::max(a, b));
	}
	return v;
}

sf::Vector2f Utils::Lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t, bool clamping)
{
	if (clamping)
	{
		t = Utils::Clamp(t, 0.f, 1.f);
	}
	return a + Utils::Normalize(b - a) * (Utils::Distance(a, b) * t);
}

sf::Color Utils::Lerp(const sf::Color& a, const sf::Color& b, const float t, bool clamping)
{
	sf::Color v;
	v.r = Lerp(a.r, b.r, t, clamping);
	v.g = Lerp(a.g, b.g, t, clamping);
	v.b = Lerp(a.b, b.b, t, clamping);
	v.a = Lerp(a.a, b.a, t, clamping);
	return v;
}

float Utils::SqrMagnitude(const sf::Vector2f& vec)
{
	return vec.x * vec.x + vec.y * vec.y;
}

float Utils::Magnitude(const sf::Vector2f& vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}

sf::Vector2f Utils::Normalize(const sf::Vector2f& vec)
//특정한 값을 일정한 범위 안에 맵핑
//매개 변수로 넘어온 벡터의 길이를 1로 만든다.
//방향과 길이 중 방향만 의미있게 하는 것
{
	float mag = Magnitude(vec);
	if (mag == 0.f)
	{
		return vec;
	}
	return vec / mag;
}

float Utils::Distance(const sf::Vector2f& pos1, const sf::Vector2f& pos2)
{
	return Magnitude(pos2 - pos1);
}

float Utils::Angle(const sf::Vector2f& dir)
{
	return atan2(dir.y, dir.x) * (180.f / M_PI);
	//아크 탄젠트2 함수, 라디안 단위로 리턴
	//m_pi는 파이값
}

float Utils::Angle(const sf::Vector2f& start, const sf::Vector2f& end)
{
	return Angle(Normalize(end - start)); 
	//start에서 end로 바라보는 방향을 구해서 넘김
}

//각도를 넘겨서 x, y좌표 계산
sf::Vector2f Utils::Direction(const float angle)
{
	//PI는 원의 지름 / 원의 둘레 == 반지름 / (원의 둘레/2) 
	//각도 단위는 반지름이 1인 원의 라디안
	float radians = angle * (M_PI / 180.f);

	// x와 y 좌표 계산
	float x = cos(radians);
	float y = sin(radians);

	return sf::Vector2f(x, y);
}

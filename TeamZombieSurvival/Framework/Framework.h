#pragma once
#include "Singleton.h"
//singleton 적용 방법, 이것이 기본 틀
class Framework : public Singleton<Framework>
{
	friend Singleton<Framework>;
protected:
	Framework() = default;
	Framework(int w, int h, const string& t);
	virtual ~Framework() override = default;

	sf::RenderWindow window; //화면
	sf::Clock clock; //시간

	int screenWidth = 1920;
	int screenHeight = 1080;
	string title = "Zombie Survival";

public:
	//화면의 가로 세로, 타이틀 화면에 뜰 글 인자로 전달
	virtual void Init(int width, int height, const string& title);
	virtual void Release();

	virtual void Update(float dt);
	virtual void Draw(); //window가 멤버 변수이기 때문에 매개변수 필요x
	virtual void Run(); //메인 루프 실행
	
	sf::Vector2f GetWindowSize();
	sf::RenderWindow& GetWindow();
};

#define FRAMEWORK (Framework::Instance())

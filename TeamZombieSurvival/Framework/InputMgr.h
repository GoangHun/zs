#pragma once
#include "Singleton.h"

enum class Axis //축
{
	Horizontal, //가로축
	Vertical //세로축
};

struct AxisInfo //축 마다 축에 대한 정보를 저장
{
	Axis axis;
	list<int> positivies;
	list<int> negatives;

	float sensi = 3.f; //값이 바뀌는 속도 조절, 클수록 빠르게
	float value = 0.f; //해당하는 축의 현재 입력값 -1.0~ 1.0 사이의 값
};

class InputMgr :public Singleton<InputMgr>
{
	friend Singleton<InputMgr>;
protected:
	InputMgr();
	virtual ~InputMgr() override = default;

	/*키보드 입력과 마우스 입력 리스트를 하나로 사용.
	앞부분은 키보드, 뒷부분은 마우스 입력으로 사용*/
	std::list<int> downList;
	std::list<int> upList;
	std::list<int> ingList;

	sf::Vector2f mousePos; //마우스 커서 위치
	map<Axis, AxisInfo> axisInfoMap; //vector를 사용하는 것이 더 좋지만 map을 공부하는 김에
	
public:
	void Update(float dt);
	void UpdateEvent(const sf::Event& ev);

	//키보드 입력
	bool GetKeyDown(sf::Keyboard::Key key);
	bool GetKey(sf::Keyboard::Key key);
	bool GetKeyUp(sf::Keyboard::Key key);

	/*---마우스----*/
	const sf::Vector2f GetMousePos(); //마우스 위치 좌표
	//마우스 입력 관련
	bool GetMouseButtonDown(sf::Mouse::Button button);
	bool GetMouseButton(sf::Mouse::Button button);
	bool GetMouseButtonUp(sf::Mouse::Button button);



	//축 입력 검사
	float GexAxis(Axis axis); //-1~1 사이의 실수 반환
	float GexAxisRaw(Axis axis); //-1, 0 , 1 중 1개 반환
};

#define INPUT_MGR (InputMgr::Instance())
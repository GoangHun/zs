#include "stdafx.h"
#include "InputMgr.h"
#include "Framework.h"
#include "Utils.h"

InputMgr::InputMgr()
{
	/*원래 파일에 쓰고 파일의 내용을 로드해서 하는 것이 바람직함.
	프로젝트 시에는 변경할 것.*/
	AxisInfo infoH; //가로축 입력에 관한
	infoH.axis = Axis::Horizontal;
	infoH.positivies.push_back((int)sf::Keyboard::D);
	infoH.positivies.push_back((int)sf::Keyboard::Right);
	infoH.negatives.push_back((int)sf::Keyboard::A);
	infoH.negatives.push_back((int)sf::Keyboard::Left);
	//infoH.sensi = 0.1f;
	axisInfoMap.insert({ infoH.axis, infoH });

	AxisInfo infoV;
	infoV.axis = Axis::Vertical;
	infoV.positivies.push_back((int)sf::Keyboard::S);
	infoV.positivies.push_back((int)sf::Keyboard::Down);
	infoV.negatives.push_back((int)sf::Keyboard::W);
	infoV.negatives.push_back((int)sf::Keyboard::Up);
	//infoV.sensi = 0.1f;
	axisInfoMap.insert({ infoV.axis, infoV });
}

void InputMgr::Update(float dt) //frame마다 호출
{
	downList.clear();
	upList.clear();

	for (auto& it : axisInfoMap)
	{
		auto& axisInfo = it.second;
		float raw = GexAxisRaw(axisInfo.axis); //-1. 0 .1 중 1
		//cout << raw << endl;
		if(raw == 0.f && axisInfo.value != 0.f) //일반적인 경우에 float는 0으로 == 비교 불가능
		//좀전까지는 양의 입력이 있었는데 방금 양의 입력이 없어진 경우 -> 값은 0으로 가야함.
		//raw의 값은 음수로
		{
			raw = axisInfo.value > 0.f ? -1.f : 1.f;
		}
		float diff = axisInfo.sensi * dt;
		axisInfo.value = Utils::Clamp(axisInfo.value + raw * diff, -1.0f, 1.0f);
		if (abs(axisInfo.value) < diff * 0.5f)
		{
			axisInfo.value = 0.f;
		}
	}
}

void InputMgr::UpdateEvent(const sf::Event& ev) //한 프레임의 event마다 호출
{
	switch (ev.type)
	{
	case sf::Event::KeyPressed:
		if (std::find(ingList.begin(), ingList.end(), ev.key.code) == ingList.end())
		{
			downList.push_back(ev.key.code);
			ingList.push_back(ev.key.code);
		}
		break;
	case sf::Event::KeyReleased:
		ingList.remove(ev.key.code);
		upList.push_back(ev.key.code);
		break;
	case sf::Event::MouseButtonPressed:
	{
		int code = sf::Keyboard::KeyCount + ev.mouseButton.button; //sf::mouse 버튼 열거형

		if (std::find(ingList.begin(), ingList.end(), code) == ingList.end())
		{
			downList.push_back(code);
			ingList.push_back(code);
		}
	}
		break;
	case sf::Event::MouseButtonReleased:
	{
		int code = sf::Keyboard::KeyCount + ev.mouseButton.button;
		ingList.remove(code);
		upList.push_back(code);
		break;
	}
	}

	mousePos = (sf::Vector2f)sf::Mouse::getPosition(FRAMEWORK.GetWindow());
	//윈도우에 맞는 마우스 커서의 위치 갱신
}

bool InputMgr::GetKeyDown(sf::Keyboard::Key key)
{
	return std::find(downList.begin(), downList.end(), key) != downList.end();
}

bool InputMgr::GetKey(sf::Keyboard::Key key)
{
	return std::find(ingList.begin(), ingList.end(), key) != ingList.end();
}

bool InputMgr::GetKeyUp(sf::Keyboard::Key key)
{
	return std::find(upList.begin(), upList.end(), key) != upList.end();
}

const sf::Vector2f InputMgr::GetMousePos()
{
	return mousePos;
}

bool InputMgr::GetMouseButtonDown(sf::Mouse::Button button)
{
	int code = (int)sf::Keyboard::KeyCount + (int)button; //키보드 입력 뒤에 마우스 입력
	return std::find(downList.begin(), downList.end(), code) != downList.end();
}

bool InputMgr::GetMouseButton(sf::Mouse::Button button)
{
	int code = (int)sf::Keyboard::KeyCount + (int)button; //키보드 입력 뒤에 마우스 입력
	return std::find(ingList.begin(), ingList.end(), code) != ingList.end();
}

bool InputMgr::GetMouseButtonUp(sf::Mouse::Button button)
{
	int code = (int)sf::Keyboard::KeyCount + (int)button; //키보드 입력 뒤에 마우스 입력
	return std::find(upList.begin(), upList.end(), code) != upList.end();
}

float InputMgr::GexAxis(Axis axis)
{
	const auto& it = axisInfoMap.find(axis);
	if (it == axisInfoMap.end())
	{
		return 0.0f;
	}
	return it->second.value;
}

float InputMgr::GexAxisRaw(Axis axis)
{
	const auto& it = axisInfoMap.find(axis);
	if (it == axisInfoMap.end())
	{
		return 0.0f;
	}
	
	const AxisInfo& info = it->second;

	auto rit = ingList.rbegin();

	while (rit!=ingList.rend())
	{
		int code = *rit;
		if (find(info.positivies.begin(), info.positivies.end(), code) != info.positivies.end())
		{
			return 1.f;

		}
		if (find(info.negatives.begin(), info.negatives.end(), code) != info.negatives.end())
		{
			return -1.f;
		}
		++rit;
	}
	return 0.0f;
}

/*역방향 ㅇ이터레이터*/

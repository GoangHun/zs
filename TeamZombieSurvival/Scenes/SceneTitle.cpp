#include "stdafx.h"
#include "SceneTitle.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "ResourceMgr.h"
#include "SpriteGo.h"
#include "Framework.h"
#include "TextGo.h"
#include "StringTable.h"
#include "DataTableMgr.h"
//#include <locale>
#include <codecvt>


#define BLOCKS 4
#define SQUARE 30.0


SceneTitle::SceneTitle()
	:Scene(SceneId::Title)
{
	screenSize = FRAMEWORK.GetWindowSize();
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/background.png"));
	resources.push_back(make_tuple(ResourceTypes::Font, "fonts/zombiecontrol.ttf"));
	resources.push_back(make_tuple(ResourceTypes::Font, "fonts/Dobong_Cultural_Routes.ttf"));
}

void SceneTitle::Init()
{
	Release();

	
	sf::Vector2f centerPos = screenSize * 0.5f;

	titleView.setSize(screenSize);
	titleView.setCenter(centerPos);

	/*SpriteGo* Title = new SpriteGo("BackGround", "graphics/background.png");
	AddGo(Title);*/
	SpriteGo* Title = (SpriteGo*)AddGo(new SpriteGo("graphics/background.png"));
	Title->SetOrigin(Origins::TL);
	Title->SetPosition(0.f, 0.f);
	Title->sprite.setScale(0.55f, 1.f);
	Title->sortLayer = 100;
	AddGo(Title);

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	StringTable* stringTable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	std::wstring titile = converter.from_bytes(stringTable->Get("TITLE"));

	
	AddGo(new TextGo("titleMessage", "fonts/Dobong_Cultural_Routes.ttf"));
	AddGo(new TextGo("BlinkMessage", "fonts/zombiecontrol.ttf"));
	TextGo* title = (TextGo*)FindGo("titleMessage");
	title->text.setCharacterSize(120);
	title->text.setString(titile);
	title->text.setOutlineThickness(5.f);
	title->text.setFillColor(sf::Color::Red);
	title->text.setOutlineColor(sf::Color::White);
	title->SetOrigin(Origins::MC);
	title->SetPosition(screenSize.x * 0.05f, screenSize.y * 0.35f);
	title->sortLayer = 101;
	AddGo(title);


	TextGo* title1 = (TextGo*)FindGo("BlinkMessage");
	title1->text.setCharacterSize(60);
	title1->text.setString("Press \"Enter\" to Start!");
	title1->text.setOutlineThickness(3.f);
	title1->text.setFillColor(sf::Color::White);
	title1->text.setOutlineColor(sf::Color::Black);
	title1->SetOrigin(Origins::MC);
	title1->SetPosition(screenSize.x * 0.1f, screenSize.y * 0.80f);
	title1->sortLayer = 102;
	AddGo(title1);
	title1->SetActive(false);

	for (auto go : gameObjects)
	{
		go->Init();
	}

}

void SceneTitle::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneTitle::Enter()
{
	Scene::Enter();
	/*SpriteGo* Title = (SpriteGo*)FindGo("BackGround");
	Title->sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/background.png"));*/
}

void SceneTitle::Exit()
{
	Scene::Exit();
}

void SceneTitle::Update(float dt)
{
	Scene::Update(dt);

	BlinkTitle();

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Return))
	{
		SCENE_MGR.ChangeScene(SceneId::Dev1);
	}
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);

	window.setView(titleView);
	for (auto go : gameObjects)
	{
		if (go->sortLayer >= 100)
			continue;

		if (go->GetActive())
		{
			go->Draw(window);
		}
	}
}

void SceneTitle::BlinkTitle()
{
	TextGo* title1 = (TextGo*)FindGo("BlinkMessage");
	if (!blinkTimeCheck&&clock.getElapsedTime() >= blinkTime)
	{
		clock.restart();
		blinkTimeCheck = true;
		title1->SetActive(false);
	}

	if (clock.getElapsedTime() >= blinkTime)
	{
		clock.restart();
		title1->SetActive(true);
		blinkTimeCheck = false;
	}
}

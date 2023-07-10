#include "stdafx.h"
#include "Framework.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "DataTableMgr.h"
#include "ResourceMgr.h"
#include "Scene.h"

Framework::Framework(int w, int h, const string& t)
    :screenWidth(w), screenHeight(h), title(t)
{

}

void Framework::Init(int width, int height, const string& title)
{
	window.create(sf::VideoMode(width, height), title); //window 생성

    DATATABLE_MGR.LoadAll();
    //Resource(전역으로 쓸 리소스 로드)
    SCENE_MGR.Init();
}

void Framework::Release()
{
    SCENE_MGR.Release();
    DATATABLE_MGR.ReleaseAll();
}

void Framework::Update(float dt)
{
    SCENE_MGR.Update(dt);
}

void Framework::Draw()
{
    SCENE_MGR.Draw(window);
}

void Framework::Run()
{
    Init(screenWidth, screenHeight, title); //초기화

    clock.restart(); //init에 걸린 시간 초기화

    //----커서 변경----
    sf::Image cursorImg;
    cursorImg.loadFromFile("graphics/crosshair.png");

    sf::Cursor cursor;
    if (cursor.loadFromPixels(cursorImg.getPixelsPtr(), cursorImg.getSize(),
        { cursorImg.getSize().x / 2, cursorImg.getSize().y / 2 }))
    {
        window.setMouseCursor(cursor);
    }

    while (window.isOpen())
    {
        sf::Time deltaTime = clock.restart(); //현재까지 걸린 시간 리턴
        float dt = deltaTime.asSeconds();

        INPUT_MGR.Update(dt);

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                SCENE_MGR.GetCurrScene()->Exit(); //종료될 때 현지 씬 정리
                window.close();
                break;

            case sf::Event::GainedFocus:
                window.setMouseCursor(cursor);
                break;

            };
            INPUT_MGR.UpdateEvent(event);
        }
        if (window.isOpen())
        {
            Update(dt); //framework의 update 호출

            window.clear();
            Draw();
            window.display();
        }
    }
    Release();
}

sf::Vector2f Framework::GetWindowSize()
{
    return sf::Vector2f(screenWidth, screenHeight);
}

sf::RenderWindow& Framework::GetWindow()
{
    return window;
}

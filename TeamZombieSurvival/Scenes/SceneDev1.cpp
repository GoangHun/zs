#include "stdafx.h"
#include "SceneDev1.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "Player.h"
#include "VertexArrayGo.h"
#include "Framework.h"
#include "Zombie.h"
#include "SpriteEffect.h"
#include "TextGo.h"
#include "RectangleGo.h"
#include "item.h"
#include <sstream>



SceneDev1::SceneDev1() : Scene(SceneId::Dev1), player(nullptr)
{
	resources.push_back(make_tuple( ResourceTypes::Texture, "graphics/player.png"));
	resources.push_back(make_tuple(ResourceTypes::Texture, "graphics/background_sheet.png"));
	resources.push_back(make_tuple(ResourceTypes::Texture, "graphics/bloater.png"));
	resources.push_back(make_tuple(ResourceTypes::Texture, "graphics/chaser.png"));
	resources.push_back(make_tuple(ResourceTypes::Texture, "graphics/crawler.png"));
	resources.push_back(make_tuple(ResourceTypes::Texture, "graphics/bullet.png"));
	resources.push_back(make_tuple(ResourceTypes::Texture, "graphics/blood.png"));
	resources.push_back(make_tuple(ResourceTypes::Texture, "graphics/health_pickup.png"));
	resources.push_back(make_tuple(ResourceTypes::Texture, "graphics/ammo_pickup.png"));
	resources.push_back(make_tuple(ResourceTypes::Font, "fonts/zombiecontrol.ttf"));
	resources.push_back(make_tuple(ResourceTypes::Texture, "graphics/background.png"));
}

SceneDev1::~SceneDev1()
{
	Release();
}

void SceneDev1::Init()
{
	Release();

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;

	worldView.setSize(windowSize);
	uiView.setSize(windowSize);
	uiView.setCenter(centerPos);

	player = (Player*)AddGo(new Player("graphics/player.png", "Player"));
	player->sortLayer = 2;

	//텍스트 생성
	AddGo(new TextGo("Score", "fonts/zombiecontrol.ttf"));
	AddGo(new TextGo("HiScore", "fonts/zombiecontrol.ttf"));
	AddGo(new TextGo("Wave", "fonts/zombiecontrol.ttf"));
	AddGo(new TextGo("ZombieCount", "fonts/zombiecontrol.ttf"));
	AddGo(new TextGo("AmmoCount", "fonts/zombiecontrol.ttf"));
	AddGo(new TextGo("Fps", "fonts/zombiecontrol.ttf"));
	AddGo(new TextGo("HpText", "fonts/zombiecontrol.ttf")); //체력수치 추가
	AddGo(new TextGo("NoBullet", "fonts/zombiecontrol.ttf")); //총알없다.
	AddGo(new TextGo("StatUpText", "fonts/zombiecontrol.ttf"));
	AddGo(new TextGo("GameOver", "fonts/zombiecontrol.ttf"));


	//스탯 업 백그라운드 생성
	AddGo(new SpriteGo("graphics/background.png", "StatUpBg"));
	

	//총알 아이콘
	AddGo(new SpriteGo("graphics/ammo_icon.png", "AmmoIcon"));

	//체력바 생성
	sf::Vector2f size(300.f, 30.f);
	
	AddGo(new RectangleGo(size, "HpBar"));
	AddGo(new RectangleGo(size, "MaxHpBar"));
	AddGo(new RectangleGo(staminaSize, "StaminaBar"));
	AddGo(new RectangleGo(staminaSize, "MaxStaminaBar"));

	sf::Vector2f tileWorldSize = { 50.f, 50.f };
	sf::Vector2f tileTexSize = { 50.f ,50.f };
	background = CreateBackGround({ 100, 100 }, { 50.f, 50.f }, { 50.f, 50.f }, "graphics/background_sheet.png");
	AddGo(background);

	for (auto go : gameObjects)
	{
		go->Init();
	}
	
	background->sortLayer = -1;
	background->SetOrigin(Origins::MC);
	background->SetPosition(0.f, 0.f);

	wallBounds = background->vertexArray.getBounds();
	wallBounds.width -= tileWorldSize.x * 2.f;
	wallBounds.height -= tileWorldSize.y * 2.f;
	wallBounds.left += tileWorldSize.x;
	wallBounds.top += tileWorldSize.y;

	player->SetWallBounds(wallBounds);

	//좀비 생성
	zombiePool.OnCreate = [this](Zombie* zombie)
	{
		Zombie::Types zombieType = (Zombie::Types)Utils::RandomRange(0, Zombie::TotalTypes);
		zombie->SetType(zombieType);
		zombie->SetPlayer(player);
		zombie->sortLayer = 2;
	};
	zombiePool.Init();

	//핏자국 생성
	bloodEffectPool.OnCreate = [this](SpriteEffect* effect)
	{
		effect->textureId = "graphics/blood.png";
		effect->SetDuration(3.f); //3초 뒤에 삭제
		effect->SetPool(&bloodEffectPool);
		effect->sortLayer = 1;
	};
	bloodEffectPool.Init();

	//아이템 생성
	itemPool.OnCreate = [this](Item* item)
	{
		Item::ItemTypes itemtype = (Item::ItemTypes)Utils::RandomRange(0, Item::TotalTypes);
		item->SetType(itemtype);
		if (itemtype == Item::ItemTypes::Ammo)
		{
			item->textureId = "graphics/ammo_pickup.png";
		}
		else if (itemtype == Item::ItemTypes::Potion)
		{
			item->textureId = "graphics/health_pickup.png";
		}
		item->SetPlayer(player);
		item->sortLayer = 2;
	};
	itemPool.Init();
	//crosshair->sortLayer = 2; 
	//100이후 부터는 ui레이어로 사용, 커서는 가장 위에 그려야한다, ui좌표계로 관리하면 100번보다 뒤의 레이어로 해야함
	

	
}

void SceneDev1::Release()
{
	zombiePool.Release();
	bloodEffectPool.Release();
	itemPool.Release();

	for (auto go : gameObjects)
	{
		//go->Release(); 소멸자에서 호출
		delete go;
	}
}

void SceneDev1::Enter()
{
	Scene::Enter();

	worldView.setCenter(0.f, 0.f);
	player->SetPosition(0.f, 0.f);
	
	isGameOver = false;
	//isPause = true;
	score = 0;
	wave = 0;
	fps = 0;
	totalDt = 0;

	/*----UI----*/
	sf::Vector2f textPos = uiView.getSize();

	TextGo* findText = (TextGo*)FindGo("Score");
	findText->text.setCharacterSize(50);
	findText->text.setString("SCORE: " + to_string(score));
	findText->text.setFillColor(sf::Color::White);
	findText->SetOrigin(Origins::TL);
	findText->SetPosition(textPos.x * 0.05f, textPos.y * 0.03f); //setposition 위치 조심
	findText->sortLayer = 100;

	findText = (TextGo*)FindGo("HiScore");
	findText->text.setCharacterSize(50);
	findText->text.setString("HI SCORE: " + to_string(HiScore));
	findText->text.setFillColor(sf::Color::White);
	findText->SetOrigin(Origins::TR);
	findText->SetPosition(textPos.x * 0.95f, textPos.y * 0.03f); //setposition 위치 조심
	findText->sortLayer = 100;

	findText = (TextGo*)FindGo("Wave");
	findText->text.setCharacterSize(40);
	findText->text.setString("WAVE: " + to_string(wave));
	findText->text.setFillColor(sf::Color::White);
	findText->SetOrigin(Origins::BR);
	findText->SetPosition(textPos.x * 0.7f, textPos.y * 0.97f); //setposition 위치 조심
	findText->sortLayer = 100;

	findText = (TextGo*)FindGo("ZombieCount");
	findText->text.setCharacterSize(40);
	findText->text.setString("ZombieCount: " + to_string(zombiePool.GetUseList().size()));
	findText->text.setFillColor(sf::Color::White);
	findText->SetOrigin(Origins::BR);
	findText->SetPosition(textPos.x * 0.95f, textPos.y * 0.97f); //setposition 위치 조심
	findText->sortLayer = 100;

	//체력 수치
	
	NoBulletGo= (TextGo*)FindGo("NoBullet");
	NoBulletGo->text.setCharacterSize(60);
	NoBulletGo->text.setString("Not Enough Bullet");
	NoBulletGo->text.setFillColor(sf::Color::Red);
	NoBulletGo->SetOrigin(Origins::MC);
	NoBulletGo->SetPosition(textPos.x * 0.5f, textPos.y * 0.65f);
	NoBulletGo->sortLayer = 103;
	NoBulletGo->SetActive(false);

	findText = (TextGo*)FindGo("StatUpText");
	std::string ss = "Num4: Increase Damage\nNum5: Increase Shot speed\nNum6: Increase shot number";
	findText->SetText(ss, 50, sf::Color::Red, Origins::MC, 100, textPos.x * 0.5f, textPos.y * 0.5f);
	findText->SetActive(false);

	fpsGo = (TextGo*)FindGo("Fps");
	ss = "FPS: " + to_string(fps);
	fpsGo->SetText(ss, 17, sf::Color::Green, Origins::TL, 100, 10.f, 10.f);
	fpsGo->SetActive(false);

	findText = (TextGo*)FindGo("GameOver");
	ss = "PRESS ENTER TO CONTINUE";
	findText->SetText(ss, 50, sf::Color::White, Origins::MC, 100, textPos.x * 0.5f, textPos.y * 0.5f);
	findText->SetActive(false);

	SpriteGo* findGo = (SpriteGo*)FindGo("AmmoIcon");
	findGo->SetOrigin(Origins::BL);
	findGo->SetPosition(textPos.x * 0.05f, textPos.y * 0.99f);
	findGo->sortLayer = 100;

	findGo = (SpriteGo*)FindGo("StatUpBg");
	findGo->sortLayer = 100;
	findGo->sortOrder = -1;
	findGo->SetOrigin(Origins::MC);
	findGo->SetPosition(textPos.x * 0.5f, textPos.y * 0.5f);
	findGo->SetActive(false);

	findText = (TextGo*)FindGo("AmmoCount");
	findText->text.setCharacterSize(40);
	findText->text.setString(to_string(player->GetAmmo()) + "/" + to_string(player->GetMaxAmmo()));
	findText->text.setFillColor(sf::Color::White);
	findText->SetOrigin(Origins::BR);
	findText->SetPosition(textPos.x * 0.2f, textPos.y * 0.97f); //setposition 위치 조심
	findText->sortLayer = 100;

	RectangleGo* recGo = (RectangleGo*)FindGo("HpBar");
	recGo->rectangle.setFillColor(sf::Color::Red);
	recGo->SetOrigin(Origins::BL);
	recGo->SetPosition(textPos.x * 0.25f, textPos.y * 0.97f);
	recGo->sortLayer = 101;

	recGo = (RectangleGo*)FindGo("MaxHpBar");
	recGo->rectangle.setFillColor(sf::Color::White);
	recGo->SetOrigin(Origins::BL);
	recGo->SetPosition(textPos.x * 0.25f, textPos.y * 0.97f);
	recGo->sortLayer = 100;
	
	HpGo = (TextGo*)FindGo("HpText");
	HpGo->text.setCharacterSize(25);
	HpGo->text.setString(to_string(player->GetHp()) + "/" + to_string(player->GetMaxHp()));
	HpGo->text.setFillColor(sf::Color::Black);
	HpGo->SetOrigin(Origins::BC);
	HpGo->SetPosition(textPos.x * 0.25f + 150.f, textPos.y * 0.965f);
	HpGo->sortLayer = 102;

	recGo = (RectangleGo*)FindGo("StaminaBar");
	recGo->rectangle.setFillColor(sf::Color::Yellow);
	recGo->SetOrigin(Origins::BL);
	recGo->SetPosition(textPos.x * 0.55f, textPos.y * 0.5f);
	recGo->sortLayer = 103; //101
	recGo->rectangle.setSize({ 0.f,0.f });
	
	recGo = (RectangleGo*)FindGo("MaxStaminaBar");
	recGo->rectangle.setFillColor(sf::Color::Black);
	recGo->rectangle.setOutlineColor(sf::Color::Black);
	recGo->rectangle.setOutlineThickness(0.5f);
	recGo->SetOrigin(Origins::BL);
	recGo->SetPosition(textPos.x * 0.55f, textPos.y * 0.5f);
	recGo->sortLayer = 102; //100
	recGo->rectangle.setSize({ 0.f,0.f });
}

void SceneDev1::Exit()
{
	ClearObjectPool(zombiePool);
	ClearObjectPool(bloodEffectPool);
	ClearObjectPool(itemPool);

	player->Reset();
	Scene::Exit();
}

void SceneDev1::Update(float dt)
{
	totalDt += dt;
	fps++;
	if (totalDt > 1.f)
	{
		std::string ss = "FPS: " + to_string(fps);
		fpsGo->text.setString(ss);

		totalDt = 0;
		fps = 0.f;
	}

	worldView.setCenter(player->GetPosition()); //플레이어의 위치로 센터로 계속 바꾼다.

	if (isGameOver)
	{
		TextGo* findText = (TextGo*)FindGo("GameOver");
		findText->SetActive(true);

		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Return))
		{
			findText->SetActive(false);
			isGameOver = true;
			SCENE_MGR.ChangeScene(sceneId);
			//waveGo->SetActive(false);
			//}
		}
		return;
	}

	Scene::Update(dt);

	/*좀비 스폰, 디스폰 test code*/
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
	{
		SpawnZombies(10, player->GetPosition(), 500.f);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
	{
		ClearObjectPool(zombiePool);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num3))
	{
		//isPause = true;
	}
	if (zombiePool.GetUseList().size() == 0)
	{
		//웨이브 바뀔 때마다 아이템 삭제
		std::list<Item*> items = itemPool.GetUseList();
		if (!items.empty())
		{
			for (auto item : items)
			{
				RemoveGo(item);
				itemPool.Return(item);
			}
		}

		if (wave > 0)
		{
			SpriteGo* findGo = (SpriteGo*)FindGo("StatUpBg");
			findGo->SetActive(true);
			TextGo* findTgo = (TextGo*)FindGo("StatUpText");
			findTgo->SetActive(true);
			player->SetActive(false);

			//스탯업 기능 테스트
			if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num4))
			{
				player->StatUp(1);
				NextWave(findGo, findTgo);
			}
			else if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num5))
			{
				player->StatUp(2);
				NextWave(findGo, findTgo);
			}
			else if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num6))
			{
				player->StatUp(3);
				NextWave(findGo, findTgo);
			}
		}
		else
		{
			SpawnZombies(10, player->GetPosition(), 800.f);
			wave++;
			TextGo* findText = (TextGo*)FindGo("Wave");
			findText->text.setString("WAVE: " + to_string(wave));
		}	
	}
	

	if (!isGameOver) //hp 수치 갱신
	{
		//std::string ss1 = to_string(player->GetHp()) + "/" + to_string(player->GetMaxHp());
		//HpGo->text.setString(ss1);

		RectangleGo* staminaBar = (RectangleGo*)FindGo("StaminaBar");
		staminaBar->rectangle.setSize({ player->GetStamina() * 10.f,5.f });
		RectangleGo* maxStaminaBar = (RectangleGo*)FindGo("MaxStaminaBar");
		maxStaminaBar->rectangle.setSize(staminaSize);
		/*RectangleGo* MaxstaminaBar = (RectangleGo*)FindGo("StaminaBar");
		MaxstaminaBar->rectangle.setSize({ player->GetStamina() * 10.f,5.f });*/
		if ((player->GetStamina() == player->GetMaxStamina()))
		{
			if (duration <= 0.f)
			{
				duration = 1.5f;
			}

			if (dt > 1.5f)
				dt = 0.f;
			StaminaControl(dt);
		}
		else if ((player->GetStamina() != player->GetMaxStamina()))
		{
			StaminaSet();
		}
	}

	if (player->GetAmmo() < player->GetshotBulletNum()) //총알 없을때 메시지
	{
		TextGo* NoBulletGo = (TextGo*)FindGo("NoBullet");

		NoBulletGo->SetActive(true);
		if (clock.getElapsedTime() <= blinkTime)
		{
			NoBulletGo->text.setFillColor(sf::Color::White);
		}
		else {
			NoBulletGo->text.setFillColor(sf::Color::Red);
			if (clock.getElapsedTime() >= sf::seconds(0.6f))
				clock.restart();
		}
	}
	else
	{
		NoBulletGo->SetActive(false);
	}

	if (INPUT_MGR.GetKey(sf::Keyboard::LControl) && INPUT_MGR.GetKeyDown(sf::Keyboard::G))
	{
		fpsGo->SetActive(!fpsGo->GetActive());
	}
	RectangleGo* hpBar = (RectangleGo*)FindGo("HpBar");
	hpBar->rectangle.setSize({ player->GetHp() * 3.f, 30.f });

	TextGo* findText = (TextGo*)FindGo("AmmoCount");
	findText->text.setString(to_string(player->GetAmmo()) + "/" + to_string(player->GetMaxAmmo()));

	std::string ss1 = to_string(player->GetHp()) + "/" + to_string(player->GetMaxHp());
	HpGo->text.setString(ss1);
	
}

void SceneDev1::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

VertexArrayGo* SceneDev1::CreateBackGround(sf::Vector2i size, sf::Vector2f tileSize, sf::Vector2f texSize, string textureId)
{
	VertexArrayGo* background = new VertexArrayGo(textureId, "Background");
	background->vertexArray.setPrimitiveType(sf::Quads);
	background->vertexArray.resize(size.x * size.y * 4); //매개변수: 필요한 정점의 수
	
	sf::Vector2f startPos = { 0, 0 }; //시작 좌표
	sf::Vector2f offsets[4] =
	{
		//정점의 좌표
		{0.f, 0.f},
		{tileSize.x, 0.f},
		{tileSize.x, tileSize.y},
		{0.f, tileSize.y}
	}; //정점의 순서, 순서는 제공된 레퍼런스 페이지 참조하기
	sf::Vector2f texOffsets[4] =
	{
		{0.f, 0.f},
		{texSize.x, 0.f},
		{texSize.x, texSize.y},
		{0.f, texSize.y}
	};

	sf::Vector2f currPos = startPos;
	/*외곽은 벽돌로, 내부는 나머지 3개중 1개 랜덤으로 선택하기*/
	for (int i = 0; i < size.y; i++)
	{
		for (int j = 0; j < size.x; j++)
		{
			int texIndex = 3;
			if (i != 0 && i != size.y - 1 && j != 0 && j != size.x - 1) 
				//외곽이 아닐 때, 윗줄, 아랫줄, 왼쪽, 오른쪽 순서로 외곽 검사
			{
				texIndex = Utils::RandomRange(0, 3); //타일 3개 중 1개 랜덤으로
			}

			int tileIndex = size.x * i + j; //2차원 배열의 인덱스를 1차원 배열 인덱스로 변경
			for (int k = 0; k < 4; k++) //4각형의 정점을 하나씩 도는 for문
			{
				int vertexIndex = tileIndex * 4 + k; //정점의 인덱스
				background->vertexArray[vertexIndex].position = currPos + offsets[k];
				background->vertexArray[vertexIndex].texCoords = texOffsets[k];
				background->vertexArray[vertexIndex].texCoords.y += texSize.y * texIndex;
			}
			currPos.x += tileSize.x;
		}
		currPos.x = startPos.x;
		currPos.y += tileSize.y;
	}
	return background;
}

void SceneDev1::SpawnZombies(int count, sf::Vector2f center, float radius)
{
	for (int i = 0; i < count; i++)
	{
		Zombie* zombie = zombiePool.Get();
		sf::Vector2f pos;
		sf::FloatRect bounds = background->vertexArray.getBounds();
		sf::Vector2f halfSize = { zombie->sprite.getLocalBounds().width / 2, zombie->sprite.getLocalBounds().height / 2 };
		do
		{
			pos = center + Utils::RandomInCircle(radius);
		} 
		while (Utils::Distance(center, pos) < 100.f && radius > 100.f
			|| pos.x < bounds.left + halfSize.x + 50.f || pos.x > bounds.left + bounds.width - halfSize.x - 50.f ||
			pos.y < bounds.top + halfSize.y + 50.f || pos.y > bounds.top + bounds.height - halfSize.y - 50.f);

		//여기서 좀비를 원하는 타입으로 변경시킬 수 있음

		zombie->SetPosition(pos);
		AddGo(zombie);
	}

	TextGo* findText = (TextGo*)FindGo("ZombieCount");
	findText->text.setString("ZombieCount: " + to_string(zombiePool.GetUseList().size()));

}

void SceneDev1::OnDieZombie(Zombie* zombie)
{
	//점수 증가
	score += zombie->GetScore();
	TextGo* findText = (TextGo*)FindGo("Score");
	findText->text.setString("SCORE: " + to_string(score));
	if (score > HiScore)
	{
		HiScore = score;
		TextGo* findText = (TextGo*)FindGo("HiScore");
		findText->text.setString("HI SCORE: " + to_string(HiScore));
	}
	//핏자국 생성
	SpriteEffect* blood = bloodEffectPool.Get();
	blood->SetPosition(zombie->GetPosition());
	AddGo(blood);

	//아이템 생성
	int random = Utils::RandomRange(0, 9);
	if (random >= 5)
	{
		Item* item = itemPool.Get();
		item->SetPosition(zombie->GetPosition());
		AddGo(item);
	}

	RemoveGo(zombie); //리스트에서 삭제
	zombiePool.Return(zombie);

	//남은 좀비 수 변경
	findText = (TextGo*)FindGo("ZombieCount");
	findText->text.setString("ZombieCount: " + to_string(zombiePool.GetUseList().size()));
}

void SceneDev1::TakeItem(Item* item)
{
	RemoveGo(item); //리스트에서 삭제
	itemPool.Return(item);
}

const list<Zombie*>* SceneDev1::GetZombieList() const
{
	return &zombiePool.GetUseList();
}

void SceneDev1::OnDiePlayer()
{
	isGameOver = true;
}
VertexArrayGo* SceneDev1::GetBackground()
{
	return background;
}

void SceneDev1::StaminaControl(float dt)
{
		timer += +dt;

		RectangleGo* staminaBar = (RectangleGo*)FindGo("StaminaBar");
		RectangleGo* maxStaminaBar = (RectangleGo*)FindGo("MaxStaminaBar");
		sf::Color start1 = sf::Color::Yellow;
		sf::Color start2 = sf::Color::Black;
		sf::Color end = { 255,255,255,0 };
	
		sf::Color color1 = Utils::Lerp(end, start1, duration - timer);//duration - timer<0 투명화
		
		sf::Color color2 = Utils::Lerp(end, start2, duration - timer);
	
		staminaBar->rectangle.setFillColor(sf::Color::Color(color1));
		maxStaminaBar->rectangle.setFillColor(sf::Color::Color(color2));
		//transparency = true;
	
		maxStaminaBar->rectangle.setOutlineThickness(0.f);
}

void SceneDev1::StaminaSet()
{
	if (timer > 1.5f)
		timer = 0.f;
	RectangleGo* staminaBar = (RectangleGo*)FindGo("StaminaBar");
	RectangleGo* maxStaminaBar = (RectangleGo*)FindGo("MaxStaminaBar");
	staminaBar->rectangle.setFillColor(sf::Color::Yellow);
	maxStaminaBar->rectangle.setFillColor(sf::Color::Black);
	maxStaminaBar->rectangle.setOutlineThickness(0.5f);
	//transparency = false;
}


void SceneDev1::NextWave(SpriteGo* statUpBg, TextGo* statUpText)
{
	SpawnZombies(10, player->GetPosition(), 800.f);
	wave++;
	TextGo* findText = (TextGo*)FindGo("Wave");
	findText->text.setString("WAVE: " + to_string(wave));

	statUpBg->SetActive(false);
	statUpText->SetActive(false);
	player->SetActive(true);
}

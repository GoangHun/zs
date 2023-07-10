#include "stdafx.h"
#include "Zombie.h"
#include "InputMgr.h"
#include "Player.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "SceneDev1.h"
#include "VertexArrayGo.h"
#include "DataTableMgr.h"
#include "ZombieTable.h"

//파일 입출력으로 대치 되어야하는 부분 stakrt
const string Zombie::textureIds[3] = {
        "graphics/bloater.png",
        "graphics/chaser.png",
        "graphics/crawler.png"
};

const float Zombie::speedStats[3] = { 40.f, 70.f, 20.f };
const int Zombie::hpStats[3] = { 100, 75, 50 };
const int Zombie::damages[3] = { 15, 10, 5 };
const float Zombie::attackRates[3] = { 2.f, 0.5f, 1.f };
const int Zombie::scores[3] = { 10, 5, 3 };

Zombie::Zombie(const string& n)
    :SpriteGo("", n)
{
    soundBufferSplat.loadFromFile("sound/splat.wav");

    soundSplat.setBuffer(soundBufferSplat);
}

Zombie::~Zombie()
{
}

void Zombie::Init()
{
    SpriteGo::Init();
    SetOrigin(Origins::MC);
}

void Zombie::Release()
{
    SpriteGo::Release();
}

void Zombie::Reset()
{
    SpriteGo::Reset();

    hp = maxHp; //체력 초기화
    attackTimer = attackRate;
}

void Zombie::Update(float dt)
{
    SpriteGo::Update(dt);


    /*플레이어를 추격*/
    if (player == nullptr)
        return;
    else
    {
        sf::Vector2f playerPos = player->GetPosition();

        float distance = Utils::Distance(playerPos, position);
        look = direction = Utils::Normalize(playerPos - position); //목적지-내 위치: 방향 구할 수 있음
        sprite.setRotation(Utils::Angle(look));

        if (distance > 25.f) //일정 거리에 가까워지면 도착으로 처리
        {
            position += direction * speed * dt;
            SetPosition(position);
        }
    }
    /*플레이어와 충돌*/
    attackTimer += dt;
    if(attackTimer > attackRate)
    {
        if (player->isAlive && sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds()))
        {
            attackTimer = 0.f;
            player->OnHitZombie(damage);
        }
    }
}

void Zombie::Draw(sf::RenderWindow& window)
{
    SpriteGo::Draw(window);
}

void Zombie::SetType(Types t)
{
    zombieType = t;

    const ZombieInfo& info = DATATABLE_MGR.Get<ZombieTable>(DataTable::Ids::Zombie)->Get(t);


    int index = (int)zombieType;

    textureId = info.textureId; //이미지 설정


    speed = speedStats[index]; //속도 설정
    maxHp = hpStats[index]; //최대 체력 설정
    damage = damages[index];
    attackRate = attackRates[index];
    score = scores[index];
}

Zombie::Types Zombie::GetType() const
{
    return zombieType;
}

void Zombie::SetPlayer(Player* p)
{
    player = p;
}

void Zombie::OnHitBullet(int damage)
{
    hp -= damage;
    if (hp <= 0) //hp가 0이 되면 죽는다
    {
        Scene* scene = SCENE_MGR.GetCurrScene();
        SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene); //c++의 형변환 연산자
        if (sceneDev1 != nullptr)
        {
            sceneDev1->OnDieZombie(this);
            soundSplat.play();
            soundSplat.setVolume(5.f);
        }
    }
}

int Zombie::GetDamage()
{
    return damage;
}

int Zombie::GetScore()
{
    return score;
}

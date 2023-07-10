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

    textureId = info.textureId; //이미지 설정
    speed = info.speed; //속도 설정
    maxHp = info.maxHp; //최대 체력 설정
    damage = info.damage;
    attackRate = info.attackRate;
    score = info.score;
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

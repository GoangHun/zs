#pragma once
#include "DataTable.h"
#include "Zombie.h"
struct ZombieInfo
{
    Zombie::Types zombieType;
    std::string textureId;
    float speed;
    int maxHp;
    int damage;
    float attackRate;
    int score;
};

class ZombieTable :
    public DataTable
{
protected:
    std::unordered_map<Zombie::Types, ZombieInfo> table;

public:
    ZombieTable() : DataTable(DataTable::Ids::Zombie) {};
    virtual ~ZombieTable() override = default;

    virtual bool Load() override;
    virtual void Release() override;

    const ZombieInfo& Get(Zombie::Types id);
};


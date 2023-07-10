#include "stdafx.h"
#include "ZombieTable.h"
#include "rapidcsv.h"


bool ZombieTable::Load()
{
	std::string filename = "tables/ZombiesTable.csv";

	rapidcsv::Document doc(filename);	//기본값 const int pColumnNameIdx = 0, const int pRowNameIdx = -1
	std::vector<int> ids = doc.GetColumn<int>(0);	//열의 인덱스를 넘기면 tamplate<> 형으로 반환함
	std::vector<std::string> texturePath = doc.GetColumn<std::string>(1);
	std::vector<float> speed = doc.GetColumn<float>(2);
	std::vector<int> max_hp = doc.GetColumn<int>(3);
	std::vector<int> damage = doc.GetColumn<int>(4);
	std::vector<float> attack_rate = doc.GetColumn<float>(5);
	std::vector<int> score = doc.GetColumn<int>(6);

	for (int i = 0; i < ids.size(); ++i)
	{
		ZombieInfo info;
		info.zombieType = (Zombie::Types)ids[i];
		info.textureId = texturePath[i];
		info.speed = speed[i];
		info.maxHp = max_hp[i];
		info.damage = damage[i];
		info.attackRate = attack_rate[i];
		info.score = score[i];

		table.insert({ (Zombie::Types)ids[i], info });
	}
	
	return true;
}

void ZombieTable::Release()
{
	//table.second delete
	table.clear();
}

const ZombieInfo& ZombieTable::Get(Zombie::Types id)
{
	auto find = table.find(id);
	if (find == table.end())
	{
		std::cout << "Find failed in ZombieTable::table";
	}
	return find->second;
}

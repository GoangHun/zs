#include "stdafx.h"
#include "ZombieTable.h"
#include "rapidcsv.h"


bool ZombieTable::Load()
{
	std::string filename = "tables/ZombiesTable.csv";

	rapidcsv::Document doc(filename);
	std::vector<std::string> ids = doc.GetColumn<std::string>(0);	//열의 인덱스를 넘기면 tamplate<> 형으로 반환함
	std::vector<std::string> texturePath = doc.GetColumn<std::string>(1);
	std::vector<std::string> speed = doc.GetColumn<std::string>(2);
	std::vector<std::string> ids = doc.GetColumn<std::string>(3);
	std::vector<std::string> ids = doc.GetColumn<std::string>(4);
	std::vector<std::string> ids = doc.GetColumn<std::string>(5);
	std::vector<std::string> ids = doc.GetColumn<std::string>(6);

	for (int i = 0; i < ids.size(); ++i)
	{

	}

	
	{
		std::vector<std::string> zombieInfo = doc.GetColumn<std::string>(1);
	}
	


	for (int j = 0; j < ids.size(); ++j)
	{
		table.insert({ ids[j], values[j] });
	}
	return true;
}

void ZombieTable::Release()
{
}

const ZombieInfo& ZombieTable::Get(Zombie::Types id)
{
	auto find = table.find(id);
	if (find == table.end())
	{
		return;
	}
	return find->second;
}

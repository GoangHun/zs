#pragma once
#include "Singleton.h"

class ResourceMgr : public Singleton<ResourceMgr>
{
	friend Singleton<ResourceMgr>;

protected:
	ResourceMgr() = default;
	virtual ~ResourceMgr() override;

	map<string, sf::Texture*> mapTexture;
	map<string, sf::Font*> mapFont;
	map<string, sf::SoundBuffer*> mapSoundBuffer;

public:
	//loadm unload를 타입별로 다른 함수를 사용할 수도 있음
	void Load(ResourceTypes t, const string path); //map에 데이터를 넣는 함수
	void Load(const vector<tuple<ResourceTypes, string >>& array);


	void UnLoad(ResourceTypes t, const string path); //경로 path를 id로 사용
	void UnLoad(const vector<tuple<ResourceTypes, string >>& array);

	void UnLoadAll();

	sf::Texture* GetTexture(const string& id);
	sf::Font* GetFont(const string& id);
	sf::SoundBuffer* GetSoundBuffer(const string& id);
};

#define RESOURCE_MGR (ResourceMgr::Instance())

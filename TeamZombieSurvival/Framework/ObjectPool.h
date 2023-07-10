#pragma once
#include "stdafx.h"

template <typename T>
class ObjectPool
{
protected:
	list<T*> pool; //비활성화 리스트
	list<T*> useList; //활성화 리스트
	
	void CreateObjs(int count = 100);

public:
	ObjectPool() = default;
	~ObjectPool();

	void Init(int cacheSize = 100); //매개변수로 미리 만들어둘 갯수 전달
	void Release(); //new한 것 delete
	void Clear(); //pool로 전체 회수

	T* Get(); //pool에서 비활성화된 것 하나 리턴
	void Return(T* obj); //활성화된 것 비활성화

	const list<T*>& GetPool() const;
	const list<T*>& GetUseList() const;
	
	std::function <void(T*)> OnCreate; //반환형이 void이고 매개변수가 T*인 무명함수
};

template<typename T>
inline void ObjectPool<T>::CreateObjs(int count)
{
	for (int i = 0; i < count; i++)
	{
		T* obj = new T();
		if (OnCreate != nullptr)
		{
			OnCreate(obj);
		}
		obj->Init();
		pool.push_back(obj);
	}
}

template<typename T>
inline ObjectPool<T>::~ObjectPool()
{
	Release();
}

template<typename T>
inline void ObjectPool<T>::Init(int cacheSize)
{
	//pool에 T형을 cacheSize 만큼 new-동적할당
	Release();
	CreateObjs(cacheSize);

}

template<typename T>
inline void ObjectPool<T>::Release()
{
	//pool, useList 등의 동적할당한 것 모두 해제-delete
	Clear();

	for(auto obj : pool)
	{
		delete obj;
	}
	pool.clear();
}

template<typename T>
inline void ObjectPool<T>::Clear()
{
	for (auto obj : useList)
	{
		obj->SetActive(false);
		pool.push_back(obj);
	}
	useList.clear();
}

template<typename T>
inline T* ObjectPool<T>::Get()
{
	//poo에서 하나 뽑아서 활성화 한 후 리턴
	if (pool.empty())
	{
		CreateObjs();
	}
	T* obj = pool.front();
	pool.pop_front();

	obj->SetActive(true);
	obj->Reset();

	useList.push_back(obj);
	return obj;

}

template<typename T>
inline void ObjectPool<T>::Return(T* obj)
{
	if (find(useList.begin(), useList.end(), obj) == useList.end())
	{
		cout << " Return error" << endl;
		return;
	}
	useList.remove(obj);
	obj->SetActive(false);
	pool.push_back(obj);
}

template<typename T>
inline const list<T*>& ObjectPool<T>::GetPool() const
{
	return pool;
}

template<typename T>
inline const list<T*>& ObjectPool<T>::GetUseList() const
{
	return useList;
}

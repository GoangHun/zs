#pragma once

template <typename T>

//가장 간단한 형태로 singleton을 만족하는 형태
class Singleton
{
public:
    static T& Instance()
    {
        static T instance; 
        
        return instance;
    }
 
    Singleton(const Singleton&) = delete; //복사 생성자 
    Singleton& operator=(const Singleton&) = delete; 

protected: //protected에서 변경
    Singleton() = default;
    virtual ~Singleton() = default;
};
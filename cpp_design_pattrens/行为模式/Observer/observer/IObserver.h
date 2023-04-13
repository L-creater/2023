#pragma once
#include <iostream>

class IObserver
{
private:
    
public:
   
    virtual ~IObserver(){};
    virtual void update(const std::string &message_from_subject) = 0;
};




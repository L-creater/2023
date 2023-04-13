#pragma once

class IObserver;
class ISubject
{
private:
public:
    ISubject(){};
    virtual ~ISubject(){};
    virtual void Attach(IObserver *observer) = 0;
    virtual void Detach(IObserver *observer) = 0;
    virtual void Notify() = 0;

};




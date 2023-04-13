#pragma once

#include "IObserver.h"
#include "ISubject.h"
#include <iostream>
#include <list>
class Subject : public ISubject     
{
private:
    std::list<IObserver *> m_list_observer;
    std::string m_message;
public:
    virtual ~Subject(){
        std::cout<< "goodbye, I was the subject.\n";
    }
    virtual  void Attach(IObserver *observer)
    {
        m_list_observer.push_back(observer);
    }
    virtual void Detach(IObserver *observer)
    {
        m_list_observer.remove(observer);
    }
    virtual void Notify(){
        std::cout<<"Notify:" << std::endl;
        std::list<IObserver*>::iterator iterator = m_list_observer.begin();
        HowManyObserver();
        while(iterator != m_list_observer.end())
        {
            (*iterator) ->update(m_message);
            ++iterator;
        }
    }
    void CreatMessage(std::string message = "empty"){
        std::cout<<"CreatMessage:" << std::endl;
        this->m_message = message;

        Notify();
    }
    void HowManyObserver() {
        std::cout<<"HowManyObserver:" << std::endl;
        std::cout << "There are " << m_list_observer.size() << " observers in the list.\n";
  }

    /**
   * Usually, the subscription logic is only a fraction of what a Subject can
   * really do. Subjects commonly hold some important business logic, that
   * triggers a notification method whenever something important is about to
   * happen (or after it).
   */
  void SomeBusinessLogic() {
    this->m_message = "change message message";
    Notify();
    std::cout << "I'm about to do some thing important\n";
  }
};


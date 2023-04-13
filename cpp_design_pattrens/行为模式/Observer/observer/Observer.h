#pragma once

#include "IObserver.h"
#include "Subject.h"

class Observer : public IObserver
{
private:
    std::string m_message_from_subject;
    Subject &m_subject;
    static int m_static_number;
    int m_number;
public:
    Observer(Subject &subject) : m_subject(subject){
        this->m_subject.Attach(this);
        std::cout << "Hi, I'm the Observer \"" << ++Observer::m_static_number << "\".\n";
        this->m_number = Observer::m_static_number;
    } 
    virtual ~Observer(){};

    void update(const std::string &message_from_subject){
        std::cout<<"update:" << std::endl;
        m_message_from_subject = message_from_subject;
        PrintInfo();
    }
    void RemoveMeFromTheList() {
        m_subject.Detach(this);
        std::cout << "Observer \"" << m_number << "\" removed from the list.\n";
  }
  void PrintInfo() {
    std::cout << "Observer \"" << this->m_number << "\": a new message is available --> " << this->m_message_from_subject<< "\n";
  }
};

int Observer::m_static_number = 0;
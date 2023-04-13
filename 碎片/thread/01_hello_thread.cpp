//01_hello_thread

#include <iostream>
#include <thread>

void hello(){
    std::cout << "hello world from new thread."<<std::endl;
}

int main(){
    std::thread t(hello);
    t.join();

    return 0;
}
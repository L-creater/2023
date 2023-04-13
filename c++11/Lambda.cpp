#include <iostream>
#include <vector>
#include  <algorithm>

class Functor{
    private:
    	int id;
    public:
        void operator()(){
            std::cout << "id: "<<std::endl;
            ++id;
        }
};

int main()
{
   
    // int id = 0;
    // auto f = [id] () mutable{
    //     std::cout<<"id:"<<id<<std::endl;
    //     ++id;   //ok
    // };
    // id =42;
    // f();
    // f();
    // f();
    // std::cout<<id<<std::endl;

    int id = 0;
    auto f = [&id] (int param) mutable{
        std::cout<<"id:"<<id<<std::endl;
      
        ++id;   //ok
        ++param;
        std::cout<<"param: "<<param<<std::endl;
    };
    id =42;
    f(7);
    f(7);
    f(7);
    std::cout<<id<<std::endl;

    std::vector<int> vi{5,28,50,83,70,590,245,59,24};
    int x=30;
    int y=100;
    vi.erase(remove_if(vi.begin(),vi.end(), [x,y](int n){return x<n && n<y;}),vi.end());

    for(auto i:vi)
        std::cout<<i<<' ';
    std::cout<<std::endl;

}
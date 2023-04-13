#include <iostream>
using namespace std;

void printX()
{

}
//比较特化
template <typename T, typename... Types>
void printX(const T& firstArg,const Types&... args)
{
    cout << firstArg <<endl;
    printX(args...);
}
//永远不会调用
template <typename... Types>
void printX(const Types&... args)
{

}

int main()
{

}
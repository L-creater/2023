#include "template2_lib.cpp"
#include <iostream>

using namespace std;

//应用程序开发人员
class Application : public Library
{
  protected:
	virtual bool Step2()
	{
		//... 子类重写实现
		cout << "override Step2" << endl;
		return true;
	}

	virtual void Step4()
	{
		//... 子类重写实现
		cout << "override Step4" << endl;
	}
};

int main()
{
    //创建Application对象
	//注意这是个多态指针，声明类型是Library
	//实际类型是Application
	//调用虚函数的时候，根据虚函数的动态绑定规则调用子类方法
	//plib 继承A 的内容，只能调用 application 继承Library中的，重写的虚函数
	Library *pLib = new Application();  //多态
	pLib->Run();

	delete pLib;
}
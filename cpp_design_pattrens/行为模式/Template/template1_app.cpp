#include "template1_lib.cpp"

class Application
{
  public:
	bool Step2()
	{
		std::cout << "myStep2" << std::endl;
		return true;
	}

	void Step4()
	{
		std::cout << "myStep4" << std::endl;
	}
};

int main()
{
	Library lib;
	Application app;

	lib.Step1();

	if (app.Step2())
	{
		lib.Step3();
	}

	for (int i = 0; i < 4; i++)
	{
		app.Step4();
	}

	lib.Step5();
}
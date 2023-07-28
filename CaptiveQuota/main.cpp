#include "App.h"

int main(int argc, char argv[])
{
	App app = App();
	app.Init();
	while (app.Running())
	{
		app.Update();
		app.Draw();
	}
	//this call is backup in case the app closes incorrectly
	app.Close();
}
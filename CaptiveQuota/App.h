#pragma once

class App
{
	bool running = false;
public:
	App();
	~App();
	//No more rule of 5, app should never be copied

	void Init();
	
	void Update();
	void Draw();
	
	void Close();

	bool Running() { return running; }
};
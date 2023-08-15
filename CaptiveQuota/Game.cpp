#include "Game.h"

#include "InputManager.h"


Game::Game()
{
	map = Map();
	map.CreateMap(std::time(nullptr));
	map.SetCellSize(100);

	minimap = Minimap();
	minimap.LoadMap(map);

	camPos = v2(map.PlayerSpawn().x * map.CellSize(), map.PlayerSpawn().y * map.CellSize());
	player = Player();
	player.position = v2(map.PlayerSpawn().x, map.PlayerSpawn().y) * map.CellSize();

	background = LoadTexture("Assets\\Images\\Background.png");

	InputManager::EnableBind(InputManager::Up);
	InputManager::EnableBind(InputManager::Down);
	InputManager::EnableBind(InputManager::Left);
	InputManager::EnableBind(InputManager::Right);
}

Game::~Game()
{
	UnloadTexture(background);
}

void Game::Update(float deltaTime)
{
	player.Update(deltaTime);
	camPos = player.position - v2(WINDOWX / 2, WINDOWY / 2);
	
}

void Game::Draw()
{	
	int offsetX = - int(round(camPos.x)) % background.width;
	int offsetY = - int(round(camPos.y)) % background.height;
	
	if (offsetX > 0) offsetX -= background.width;
	if (offsetY > 0) offsetY -= background.height;

	for (int i = 0; i < 9; i++)
	{
		int x = i % 3 - 1;
		int y = i / 3 - 1;
    
		x = (WINDOWX / 2) + x * background.width + offsetX;
		y = (WINDOWY / 2) + y * background.height + offsetY;

		DrawTexture(background, x, y, WHITE);
	}


	map.DrawTiles(camPos);
	minimap.Draw();
	player.Draw();

	DrawFPS(20, 20);

}

void Game::PhysicStep()
{
	//get the surrounding 8 tiles, see if any are walls in it, check if player is colliding with any

	//move player to not collide with any

	//do the same for the NPCs
}
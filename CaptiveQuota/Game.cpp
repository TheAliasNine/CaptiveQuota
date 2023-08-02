#include "Game.h"

#include "MapMaker.h"

Game::Game()
{
	MapMaker mapMaker = MapMaker();
	bool mapMade = false;
	do
	{
		mapMade = mapMaker.MakeMap();
	} while (mapMade == false);
}


void Game::Update(float deltaTime)
{

}
void Game::Draw()
{

}
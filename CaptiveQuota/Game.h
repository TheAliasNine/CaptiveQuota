#pragma once

#include "AppState.h"
#include "Captive.h"
#include "Exit.h"
#include "Explosion.h"
#include "FireBall.h"
#include "KeyMaker.h"
#include "Lever.h"
#include "Map.h"
#include "Minimap.h"
#include "Player.h"

#include "v2.h"

#include "raylib.h"

#include <list>
#include <vector>

class Game : public AppState
{
	Music music;

	Map m_map;
	Minimap m_minimap;
	
	v2 m_camPos = v2();

	Player m_player;
	std::list<FireBall> m_fireballs;
	std::list<Explosion> m_explosions;

	std::vector<Lever> m_levers;
	std::vector<KeyMaker> m_keyMakers;
	static const int captiveCount = 15;
	static const int winCaptiveKills = 8;
	std::vector<Captive> m_captives;

	Exit m_exitObj;


	Sound m_fireballcast;
	Sound m_explosion;

	bool m_gameFinished = false;
	bool m_successful = true;
	bool m_freeCam = false;

	static const float castTime;
	bool m_casting = false;
	float m_castingTimer = 0;

	
	Texture2D m_background;
	static const int portalFrames = 4;
	static const float portalAnimTime;
	static const float portalScale;
	Texture2D m_portalFrame[portalFrames];
	float m_portalAnimTimer = 0;

	void PhysicStep();

	bool CheckMapCollisions(HitBoxObject* obj, bool resolve);
	bool CheckCaptiveCollisions(HitBoxObject* obj, bool resolve);
	bool CheckExplosionCollisions(HitBoxObject* obj, bool resolve);

	void DrawBackground();
	void DrawUI();

	void CastFireBall();
	void ShootFireBall();

	void CreateExplosion(v2 position);

	void Interact();

	void GameFinished(bool successful);

	void Restart();
	void Close();
public:

	Game();
	~Game();

	void Update(float deltaTime);
	void Draw(float deltaTime);
};
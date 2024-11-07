#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>

class PhysBody;
class PhysicEntity;


class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* A, PhysBody* B) override;
	void DrawScore();
private:
public:

	std::vector<PhysicEntity*> entities;
	std::vector<PhysicEntity*> obstacles;
	
	PhysBody* sensor;
	bool sensed;

	Texture2D circle;
	Texture2D spring;
	Texture2D flipper;
	Texture2D bouncetx;

	Texture2D background;
	Texture2D background_layer;
	Texture2D end;

	vec2<int> ray;
	bool ray_on;
private:
	int score;
	int highscore;
	int previousscore;
	bool extraBall = true;
	int lifes;
	bool endRun;

	//Music
	Music music;
	float timePlayed = 0.0f;
	int flipper_fx;
	int spring_fx;
	int bonus_fx;
};


#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

class PhysicEntity
{
protected:

	PhysicEntity(PhysBody* _body)
		: body(_body)
	{

	}

public:
	virtual ~PhysicEntity() = default;
	virtual void Update() = 0;

	virtual int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal)
	{
		return 0;
	}
	void setListener(Module* module) {
		body->listenerptr = module;
	}
	PhysBody* body;
protected:

};

class Circle : public PhysicEntity
{
public:
	Circle(ModulePhysics* physics, int _x, int _y, Texture2D _texture)
		: PhysicEntity(physics->CreateCircle(_x, _y, 12))
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		Vector2 position{ (float)x, (float)y };
		float scale = 1.6f;
		Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
		Rectangle dest = { position.x , position.y , (float)texture.width * scale , (float)texture.height * scale };
		Rectangle sourceEnd = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
		Rectangle destEnd = { position.x , position.y , (float)texture.width * scale , (float)texture.height * scale };
		Vector2 origin = { ((float)texture.width / (2.0f)) * scale, ((float)texture.height / (2.0f)) * scale };
		float rotation = body->GetRotation() * RAD2DEG;
		DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
	}

private:
	Texture2D texture;


};

class CollisionCircle : public PhysicEntity
{
public:
	CollisionCircle(ModulePhysics* physics, int _x, int _y, Texture2D _texture)
		: PhysicEntity(physics->CreateCollisionCircle(_x, _y, 12))
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		Vector2 position{ (float)x, (float)y };
		float scale = 1.6f;
		Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
		Rectangle dest = { position.x , position.y , (float)texture.width * scale , (float)texture.height * scale };
		Vector2 origin = { ((float)texture.width / (2.0f)) * scale, ((float)texture.height / (2.0f)) * scale };
		float rotation = body->GetRotation() * RAD2DEG;
		DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
	}

private:
	Texture2D texture;

};

class Box : public PhysicEntity
{
public:
	Box(ModulePhysics* physics, int _x, int _y, Texture2D _texture)
		: PhysicEntity(physics->CreateRectangle(_x, _y, 100, 50))
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTexturePro(texture, Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
			Rectangle{ (float)x, (float)y, (float)texture.width, (float)texture.height },
			Vector2{ (float)texture.width / 2.0f, (float)texture.height / 2.0f }, body->GetRotation() * RAD2DEG, WHITE);
	}

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override
	{
		return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);;
	}

private:
	Texture2D texture;

};

class Spring : public PhysicEntity
{
public:

	Spring(ModulePhysics* physics, Texture2D _texture)
		: PhysicEntity(physics->CreateKicker())
		, texture(_texture)
	{
		body->ctype = ColliderType::SPRING;
	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTextureEx(texture, Vector2{ (float)x - (texture.width / 2), (float)y - (texture.height / 2) }, body->GetRotation() * RAD2DEG, 1.0f, WHITE);
	}

private:
	Texture2D texture;
};

class Flipper : public PhysicEntity
{
public:

	Flipper(ModulePhysics* physics, Texture2D _texture, bool _right)
		: PhysicEntity(physics->createFlipper(_right))
		, texture(_texture)
	{
		right = _right;
	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		Vector2 position{ (float)x, (float)y };
		float scale = 1.6f;
		Rectangle source;
		if (right) {
			source = { 0.0f, 0.0f, -(float)texture.width, (float)texture.height };
		}
		else {
			source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };

		}
		Rectangle dest = { position.x , position.y , (float)texture.width * scale , (float)texture.height * scale };
		Vector2 origin = { ((float)texture.width / (2.0f)) * scale, ((float)texture.height / (2.0f)) * scale };
		float rotation = body->GetRotation() * RAD2DEG;
		DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
	}

private:
	Texture2D texture;
	bool right;
};

class CollisionChain : public PhysicEntity
{
public:

	CollisionChain(ModulePhysics* physics, b2Vec2* array, int _num, ColliderType _ct)
		: PhysicEntity(physics->CreatePinball(array, _num, _ct))
	{
	}

	void Update() override
	{
	}
};

ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ray_on = false;
	sensed = false;
	endRun = false;

	lifes = 3;
	score = 0;

}
void ModuleGame::DrawScore() {
	Vector2 position = { 100.0f, 10.0f };
	float spacing = 1.0f;
	Color color = BLACK;

	char scoreText[20];
	char highscoreText[20];
	char previousText[20];
	char lifesText[20];
	snprintf(scoreText, sizeof(scoreText), "SCORE: %d", score);
	snprintf(highscoreText, sizeof(highscoreText), "HIGHSCORE: %d", highscore);
	snprintf(previousText, sizeof(previousText), "PREVIOUS: %d", previousscore);
	snprintf(lifesText, sizeof(lifesText), "LIFES: %d", lifes);

	App->renderer->DrawText(scoreText, (int)position.x + 20, (int)position.y + 2, GetFontDefault(), (int)spacing, color);
	App->renderer->DrawText(highscoreText, (int)position.x + 20, (int)position.y + 12, GetFontDefault(), (int)spacing, color);
	App->renderer->DrawText(previousText, (int)position.x + 20, (int)position.y + 22, GetFontDefault(), (int)spacing, color);
	App->renderer->DrawText(lifesText, (int)position.x + 250, (int)position.y + 2, GetFontDefault(), (int)spacing, color);

	if (endRun) {
		DrawText(TextFormat(scoreText), (int)(SCREEN_WIDTH / 2) * SCALE - 90, (int)(SCREEN_HEIGHT / 2) * SCALE - 160, 20, color);
	}
}
ModuleGame::~ModuleGame()
{}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = LoadTexture("Assets/ball.png");
	bouncetx = LoadTexture("Assets/bounce.png");


	background = LoadTexture("Assets/Ruby Table base.png");
	background_layer = LoadTexture("Assets/Ruby Table base2.png");
	end = LoadTexture("Assets/End.png");


	music = LoadMusicStream("Audio/Music/Red Table.mp3");
	flipper_fx = App->audio->LoadFx("Audio/Effects/flipper.wav");
	spring_fx = App->audio->LoadFx("Audio/Effects/spring.wav");
	bonus_fx = App->audio->LoadFx("Audio/Effects/bonus.wav");

	if (music.stream.buffer == NULL) {
		LOG("Error loading music stream");
		ret = false;
	}
	else PlayMusicStream(music);

	bonus_fx = App->audio->LoadFx("Assets/bonus.wav");


	flipper = LoadTexture("Assets/flipper.png");

	//COLLISIONS BOUNCE
	entities.emplace_back(new CollisionCircle(App->physics, 258, 266, bouncetx));
	entities[(entities.size() - 1)]->setListener(this);
	entities.emplace_back(new CollisionCircle(App->physics, 324, 282, bouncetx));
	entities[(entities.size() - 1)]->setListener(this);
	entities.emplace_back(new CollisionCircle(App->physics, 283, 338, bouncetx));
	entities[(entities.size() - 1)]->setListener(this);

	//SPRING
	spring = LoadTexture("Assets/spring.png");
	entities.emplace_back(new Spring(App->physics, spring));
	entities[(entities.size() - 1)]->setListener(this);

	//FLIPPER
	Flipper* leftFlipper = new Flipper(App->physics, flipper, true);
	leftFlipper->body->ctype = ColliderType::FLIPPER;
	entities.emplace_back(leftFlipper);
	entities[(entities.size() - 1)]->setListener(this);

	Flipper* rightFlipper = new Flipper(App->physics, flipper, false);
	rightFlipper->body->ctype = ColliderType::FLIPPER;
	entities.emplace_back(rightFlipper);
	entities[(entities.size() - 1)]->setListener(this);

	//CIRCLE
	Circle* ball = new Circle(App->physics, 242.0f * SCALE, 320.0f * SCALE, circle);
	entities.emplace_back(ball);
	entities[(entities.size() - 1)]->setListener(this);

	b2Vec2 pinball[] = {
	b2Vec2(0, 0),
	b2Vec2(512, 0),
	b2Vec2(512, 864),
	b2Vec2(281.818, 863.455),
	b2Vec2(281.375, 838.75),
	b2Vec2(284.5, 835.25),
	b2Vec2(300.75, 827.25),
	b2Vec2(320.75, 817.375),
	b2Vec2(334.5, 809.625),
	b2Vec2(352.727, 799.273),
	b2Vec2(385.273, 778.909),
	b2Vec2(398.909, 768.909),
	b2Vec2(400.727, 768.909),
	b2Vec2(401.091, 794.545),
	b2Vec2(432.909, 794.727),
	b2Vec2(431.273, 646.727),
	b2Vec2(425.455, 634.727),
	b2Vec2(412.545, 628.909),
	b2Vec2(398.727, 629.091),
	b2Vec2(390.727, 627.091),
	b2Vec2(386.909, 623.273),
	b2Vec2(384.909, 618.909),
	b2Vec2(385.091, 571.273),
	b2Vec2(399.091, 557.091),
	b2Vec2(411.636, 539.455),
	b2Vec2(422.909, 519.636),
	b2Vec2(436.727, 491.818),
	b2Vec2(444.75, 467.625),
	b2Vec2(451.25, 441.125),
	b2Vec2(455, 415.625),
	b2Vec2(454.875, 363),
	b2Vec2(452.875, 334.625),
	b2Vec2(446.875, 300.625),
	b2Vec2(439.125, 277.375),
	b2Vec2(439, 273.25),
	b2Vec2(444.875, 279.25),
	b2Vec2(455, 303.5),
	b2Vec2(461.75, 324.75),
	b2Vec2(463.75, 338.875),
	b2Vec2(465.75, 354.625),
	b2Vec2(468, 371.125),
	b2Vec2(469.875, 417.375),
	b2Vec2(470.125, 818.25),
	b2Vec2(502.75, 818.5),
	b2Vec2(502.875, 388),
	b2Vec2(501.625, 373.875),
	b2Vec2(496.375, 326.5),
	b2Vec2(489.625, 297.75),
	b2Vec2(476.875, 265.375),
	b2Vec2(459.75, 237),
	b2Vec2(436.25, 212),
	b2Vec2(410.25, 188.5),
	b2Vec2(384.75, 171.25),
	b2Vec2(350.75, 152.25),
	b2Vec2(310, 136.25),
	b2Vec2(276, 129.5),
	b2Vec2(208, 129.75),
	b2Vec2(183.5, 134),
	b2Vec2(159.25, 141.75),
	b2Vec2(137.75, 150.25),
	b2Vec2(119.5, 159.5),
	b2Vec2(106.25, 169),
	b2Vec2(91.25, 181.25),
	b2Vec2(75.75, 194.75),
	b2Vec2(61.75, 211.5),
	b2Vec2(50.25, 232.5),
	b2Vec2(38.75, 255),
	b2Vec2(32, 278),
	b2Vec2(26.25, 304.25),
	b2Vec2(22.75, 330.25),
	b2Vec2(20.25, 352),
	b2Vec2(20.25, 409.25),
	b2Vec2(25.375, 445.375),
	b2Vec2(32.125, 469),
	b2Vec2(39, 488.75),
	b2Vec2(56.875, 522.375),
	b2Vec2(67, 539.625),
	b2Vec2(82.5, 560.75),
	b2Vec2(93.25, 570.25),
	b2Vec2(93.375, 616.75),
	b2Vec2(91.75, 621.125),
	b2Vec2(87.625, 625.375),
	b2Vec2(71.25, 627.125),
	b2Vec2(58.875, 629.125),
	b2Vec2(52.25, 634.5),
	b2Vec2(45.875, 645.75),
	b2Vec2(47, 792.545),
	b2Vec2(78.8182, 792.727),
	b2Vec2(78.8182, 768.727),
	b2Vec2(199.5, 838.75),
	b2Vec2(199.5, 864),
	b2Vec2(0, 864)
	};
	obstacles.emplace_back(new CollisionChain(App->physics, pinball, 91, ColliderType::WALL));

	b2Vec2 pinball2[] = {
	b2Vec2(130.5, 664.625),
	b2Vec2(1.875, -2.125),
	b2Vec2(6.375, -2.125),
	b2Vec2(44.5, 52.125),
	b2Vec2(44.875, 59.0),
	b2Vec2(40.125, 62.5),
	b2Vec2(33.0, 62.75),
	b2Vec2(0.25, 40.5)
	};
	obstacles.emplace_back(new CollisionChain(App->physics, pinball2, 7, ColliderType::BOUNCE));

	b2Vec2 pinball3[] = {
	b2Vec2(349.125f, 664.0f),
	b2Vec2(-0.25f, 40.5f),
	b2Vec2(-33.0f, 62.75f),
	b2Vec2(-40.125f, 62.5f),
	b2Vec2(-44.875f, 59.0f),
	b2Vec2(-44.5f, 52.125f),
	b2Vec2(-6.375f, -2.125f),
	b2Vec2(-1.875f, -2.125f)
	};
	obstacles.emplace_back(new CollisionChain(App->physics, pinball3, 7, ColliderType::BOUNCE));

	b2Vec2 pinball4[] = {
	b2Vec2(296.25f, 778.563f),
	b2Vec2(78.375f, -51.4375f),
	b2Vec2(84.75f, -61.9375f),
	b2Vec2(84.375f, -111.938f),
	b2Vec2(88.375f, -117.563f),
	b2Vec2(92.75f, -117.688f),
	b2Vec2(96.875f, -113.563f),
	b2Vec2(96.75f, -43.6875f),
	b2Vec2(95.625f, -40.4375f),
	b2Vec2(11.875f, 14.8125f)
	};
	obstacles.emplace_back(new CollisionChain(App->physics, pinball4, 9, ColliderType::NORMAL));

	b2Vec2 pinball5[] = {
	b2Vec2(332.364f, 194.182f),
	b2Vec2(34.0f, 26.5455f),
	b2Vec2(60.7273f, 58.3636f),
	b2Vec2(67.2727f, 81.2727f),
	b2Vec2(84.1818f, 133.091f),
	b2Vec2(88.3636f, 167.636f),
	b2Vec2(86.9091f, 216.182f),
	b2Vec2(74.5455f, 268.182f),
	b2Vec2(52.9091f, 316.909f),
	b2Vec2(50.1818f, 317.091f),
	b2Vec2(50.7273f, 292.727f),
	b2Vec2(62.9091f, 256.364f),
	b2Vec2(69.2727f, 232.545f),
	b2Vec2(74.5455f, 210.909f),
	b2Vec2(66.5455f, 200.364f),
	b2Vec2(66.0f, 198.364f),
	b2Vec2(55.8182f, 198.364f),
	b2Vec2(48.1818f, 204.545f),
	b2Vec2(38.1818f, 224.727f),
	b2Vec2(14.3636f, 261.091f),
	b2Vec2(14.9091f, 255.636f),
	b2Vec2(53.0909f, 186.0f),
	b2Vec2(17.1364f, 168.818f),
	b2Vec2(-10.6136f, 222.818f),
	b2Vec2(-10.8636f, 238.568f),
	b2Vec2(-13.6136f, 238.568f),
	b2Vec2(-21.8636f, 236.568f),
	b2Vec2(-23.3636f, 234.818f),
	b2Vec2(-23.3636f, 212.818f),
	b2Vec2(-7.61364f, 187.318f),
	b2Vec2(12.6364f, 146.818f),
	b2Vec2(34.3864f, 90.8182f),
	b2Vec2(36.8864f, 87.5682f),
	b2Vec2(36.3864f, 44.3182f),
	b2Vec2(17.8864f, 31.0682f),
	b2Vec2(0.636364f, 21.0682f)
	};
	obstacles.emplace_back(new CollisionChain(App->physics, pinball5, 35, ColliderType::NORMAL));

	b2Vec2 pinball6[] = {
	b2Vec2(289.909f, 182.182f),
	b2Vec2(4.27273f, -3.27273f),
	b2Vec2(9.90909f, -3.09091f),
	b2Vec2(13.9091f, 0.0909091f),
	b2Vec2(13.8182f, 29.7273f),
	b2Vec2(11.2727f, 31.5455f),
	b2Vec2(2.72727f, 31.4545f),
	b2Vec2(0.363636f, 29.8182f)
	};
	obstacles.emplace_back(new CollisionChain(App->physics, pinball6, 7, ColliderType::NORMAL));

	b2Vec2 pinball7[] = {
	b2Vec2(248.182f, 176.909f),
	b2Vec2(4.18182f, -4.54545f),
	b2Vec2(9.27273f, -4.54545f),
	b2Vec2(13.0909f, -0.0909091f),
	b2Vec2(13.1818f, 32.3636f),
	b2Vec2(11.2727f, 33.8182f),
	b2Vec2(0.272727f, 34.0341f)
	};
	obstacles.emplace_back(new CollisionChain(App->physics, pinball7, 6, ColliderType::NORMAL));

	b2Vec2 pinball8[] = {
	b2Vec2(151.091f, 374.364f),
	b2Vec2(-6.36364f, -24.1818f),
	b2Vec2(-10.1818f, -45.6364f),
	b2Vec2(-10.1818f, -116.727f),
	b2Vec2(3.78409f, -149.114f),
	b2Vec2(15.7841f, -167.364f),
	b2Vec2(33.9091f, -185.989f),
	b2Vec2(49.7841f, -193.739f),
	b2Vec2(67.6591f, -199.364f),
	b2Vec2(67.6591f, -163.864f),
	b2Vec2(62.1591f, -156.989f),
	b2Vec2(62.1591f, -101.989f),
	b2Vec2(65.9091f, -101.114f),
	b2Vec2(66.0341f, -87.1136f),
	b2Vec2(70.0341f, -60.8636f),
	b2Vec2(88.0341f, -21.8636f),
	b2Vec2(87.9091f, -5.23864f),
	b2Vec2(59.9091f, -5.48864f),
	b2Vec2(51.7841f, -11.6136f),
	b2Vec2(51.7841f, -43.3636f),
	b2Vec2(55.9091f, -85.2386f),
	b2Vec2(57.4091f, -87.6136f),
	b2Vec2(57.6591f, -103.239f),
	b2Vec2(47.4091f, -111.864f),
	b2Vec2(41.4091f, -111.489f),
	b2Vec2(41.6591f, -120.114f),
	b2Vec2(7.65909f, -119.614f),
	b2Vec2(7.90909f, -109.182f),
	b2Vec2(-7.81818f, -103.0f),
	b2Vec2(-8.0f, -87.3636f),
	b2Vec2(-2.09091f, -87.4545f),
	b2Vec2(0.0909091f, -85.3636f),
	b2Vec2(-0.0909091f, -63.9091f),
	b2Vec2(5.90909f, -29.6364f),
	b2Vec2(9.81818f, -13.0f),
	b2Vec2(12.0909f, -5.27273f),
	b2Vec2(10.1818f, -1.27273f)
	};
	obstacles.emplace_back(new CollisionChain(App->physics, pinball8, 36, ColliderType::NORMAL));

	b2Vec2 pinball9[] = {
	b2Vec2(114.75f, 216.875f),
	b2Vec2(0.0f, 3.75f),
	b2Vec2(-7.375f, 21.375f),
	b2Vec2(-17.25f, 60.875f),
	b2Vec2(-17.625f, 110.125f),
	b2Vec2(-10.375f, 156.0f),
	b2Vec2(-9.25f, 163.25f),
	b2Vec2(7.875f, 232.125f),
	b2Vec2(20.125f, 260.75f),
	b2Vec2(42.25f, 300.5f),
	b2Vec2(42.375f, 323.75f),
	b2Vec2(20.125f, 340.125f),
	b2Vec2(-1.75f, 314.25f),
	b2Vec2(-17.875f, 290.0f),
	b2Vec2(-30.375f, 266.375f),
	b2Vec2(-37.625f, 245.75f),
	b2Vec2(-45.75f, 215.5f),
	b2Vec2(-49.875f, 192.0f),
	b2Vec2(-49.25f, 104.25f),
	b2Vec2(-43.75f, 75.875f),
	b2Vec2(-37.5f, 51.875f),
	b2Vec2(-30.0f, 34.25f),
	b2Vec2(-20.0f, 18.125f),
	b2Vec2(-11.75f, 8.0f),
	b2Vec2(-3.625f, -0.125f)
	};
	obstacles.emplace_back(new CollisionChain(App->physics, pinball9, 24, ColliderType::NORMAL));

	b2Vec2 pinball10[] = {
	b2Vec2(86.9091f, 665.0f),
	b2Vec2(3.81818f, -4.0f),
	b2Vec2(8.09091f, -4.0f),
	b2Vec2(12.1818f, 2.0f),
	b2Vec2(12.0f, 54.2727f),
	b2Vec2(15.6364f, 59.9091f),
	b2Vec2(95.6364f, 113.364f),
	b2Vec2(95.7273f, 115.909f),
	b2Vec2(86.6364f, 126.273f),
	b2Vec2(82.2727f, 126.273f),
	b2Vec2(-0.272727f, 70.0909f)
	};
	obstacles.emplace_back(new CollisionChain(App->physics, pinball10, 10, ColliderType::NORMAL));

	for (auto i : obstacles) i->setListener(this);

	return ret;
}

// Load assets
bool ModuleGame::CleanUp()
{
	LOG("Unloading Intro scene");
	UnloadTexture(circle);
	UnloadTexture(spring);
	UnloadTexture(bouncetx);
	UnloadTexture(background);
	UnloadTexture(background_layer);
	UnloadTexture(flipper);
	StopMusicStream(music);
	UnloadMusicStream(music);

	return true;
}

// Update: draw background
update_status ModuleGame::Update()
{

	UpdateMusicStream(music);
	timePlayed = GetMusicTimePlayed(music) / GetMusicTimeLength(music);
	if (timePlayed > 1.0f) timePlayed = 1.0f;

	if (IsKeyPressed(KEY_LEFT)) App->audio->PlayFx(flipper_fx);
	if (IsKeyPressed(KEY_RIGHT)) App->audio->PlayFx(flipper_fx);
	if (IsKeyPressed(KEY_DOWN)) App->audio->PlayFx(spring_fx);


	if (IsKeyPressed(KEY_SPACE))
	{
		ray_on = !ray_on;
		ray.x = GetMouseX();
		ray.y = GetMouseY();
	}

	if (IsKeyPressed(KEY_ONE) && App->physics->getDebug())
	{
		entities.emplace_back(new Circle(App->physics, GetMouseX(), GetMouseY(), circle));
		entities[(entities.size() - 1)]->setListener(this);
	}

	if (IsKeyPressed(KEY_R))
	{
		for (int i = 0; i < entities.size(); i++) {
			if (entities[i]->body->ctype == ColliderType::BALL) {
				App->physics->DeleteBody(entities[i]->body->body);
				entities.erase(entities.begin() + i);
				i--;
				TraceLog(LOG_INFO, "BOLA ELIMINADA");
			}
		}

		Circle* ball = new Circle(App->physics, 242.0f * SCALE, 320.0f * SCALE, circle);
		entities.emplace_back(ball);
		entities[(entities.size() - 1)]->setListener(this);

		previousscore = score;
		if (score >= highscore)
			highscore = score;
		extraBall = true;

		score = 0;
		lifes = 3;
		endRun = false;
	}

	int x, y;
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->body->ctype == ColliderType::BALL) {
			entities[i]->body->GetPhysicPosition(x, y);
			if (y >= SCREEN_HEIGHT * SCALE) {
				App->physics->DeleteBody(entities[i]->body->body);
				entities.erase(entities.begin() + i);
				TraceLog(LOG_INFO, "ADIOS BOLA ;)");
				Circle* ball = new Circle(App->physics, 242.0f * SCALE, 320.0f * SCALE, circle);
				entities.emplace_back(ball);
				entities[(entities.size() - 1)]->setListener(this);
				lifes--;
			}
			if (x >= 162 && x <= 192 && y <= 300 && y >= 280 && extraBall) {
				Circle* ball = new Circle(App->physics, 242.0f * SCALE, 320.0f * SCALE, circle);
				entities.emplace_back(ball);
				entities[(entities.size() - 1)]->setListener(this);
				extraBall = false;
				lifes++;
			}

		}
	}

	if (lifes <= 0) {
		endRun = true;
		for (int i = 0; i < entities.size(); i++) {
			if (entities[i]->body->ctype == ColliderType::BALL) {
				App->physics->DeleteBody(entities[i]->body->body);
				entities.erase(entities.begin() + i);
				i--;
				TraceLog(LOG_INFO, "BOLA ELIMINADA");
			}
		}

		if (score >= highscore)
			highscore = score;

	}

	// Prepare for raycast ------------------------------------------------------

	vec2i mouse;
	mouse.x = GetMouseX();
	mouse.y = GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	vec2f normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------

	Rectangle rect;
	rect.x = 0;
	rect.y = 208;
	rect.width = SCREEN_WIDTH;
	rect.height = SCREEN_HEIGHT;
	App->renderer->Draw(background, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, &rect);


	for (PhysicEntity* entity : entities)
	{
		entity->Update();
		if (ray_on)
		{
			int hit = entity->RayHit(ray, mouse, normal);
			if (hit >= 0)
			{
				ray_hit = hit;
			}
		}
	}

	App->renderer->Draw(background_layer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, &rect);

	if (endRun)
		App->renderer->Draw(end, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, &rect);

	// ray -----------------
	if (ray_on == true)
	{
		vec2f destination((float)(mouse.x - ray.x), (float)(mouse.y - ray.y));
		destination.Normalize();
		destination *= (float)ray_hit;

		DrawLine(ray.x, ray.y, (int)(ray.x + destination.x), (int)(ray.y + destination.y), RED);

		if (normal.x != 0.0f)
		{
			DrawLine((int)(ray.x + destination.x), (int)(ray.y + destination.y), (int)(ray.x + destination.x + normal.x * 25.0f), (int)(ray.y + destination.y + normal.y * 25.0f), Color{ 100, 255, 100, 255 });
		}
	}

	if(App->physics->getDebug()) DrawFPS(10, 10);
	DrawScore();

	return UPDATE_CONTINUE;
}

void ModuleGame::OnCollision(PhysBody* A, PhysBody* B) {
	int randomNum;
	switch (A->ctype)
	{
	case ColliderType::FLIPPER:
		TraceLog(LOG_INFO, "FLIPPER");
		break;
	case ColliderType::BOUNCE:
		TraceLog(LOG_INFO, "BOUNCE");
		if (!endRun) {
			score += (rand() % 9000 + 1000);
			App->audio->PlayFx(bonus_fx);
		}
		break;
	case ColliderType::NORMAL:
		TraceLog(LOG_INFO, "NORMAL");
		if (!endRun && B->ctype == ColliderType::BALL) {
			score += (rand() % 4000 + 1000);
			App->audio->PlayFx(bonus_fx);
		}
		break;
	case ColliderType::WALL:
		TraceLog(LOG_INFO, "WALL");
		break;
	case ColliderType::SPRING:
		TraceLog(LOG_INFO, "SPRING");
		break;
	default:
		break;
	}
}

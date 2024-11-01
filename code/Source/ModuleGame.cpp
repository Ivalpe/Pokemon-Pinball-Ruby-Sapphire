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
		Vector2 origin = { ((float)texture.width / (2.0f))*scale, ((float)texture.height / (2.0f))*scale };
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
		if(right){ 
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

ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ray_on = false;
	sensed = false;
	
	score = -1000; // Inicializar el puntaje
	
}
void ModuleGame::DrawScore() {
	Vector2 position = { 100.0f, 10.0f };
	float fontSize = 100.0f;
	float spacing = 1.0f;
	Color color = BLACK;

	// Crear una cadena que combine "SCORE" y el valor del puntaje
	char scoreText[20];
	snprintf(scoreText, sizeof(scoreText), "SCORE: %d", score);

	// Dibujar la cadena en la pantalla
	App->renderer->DrawText(scoreText, (int)position.x, (int)position.y, GetFontDefault(), (int)spacing, color);
}
ModuleGame::~ModuleGame()
{}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	

	//InitAudioDevice();

	music = LoadMusicStream("OST y efectos/Musica/Red Table.mp3");

	if (music.stream.buffer == NULL) // Verifica que se haya cargado correctamente
	{
		LOG("Error loading music stream");
		ret = false;
	}
	else
	{
		PlayMusicStream(music);
	}

	flipper_fx = App->audio->LoadFx("OST y efectos/Sound Effects/flipper.wav");   
	spring_fx = App->audio->LoadFx("OST y efectos/Sound Effects/spring.wav");
	bonus_fx = App->audio->LoadFx("OST y efectos/Sound Effects/bonus.wav");
	circle = LoadTexture("Assets/ball.png"); 

	box = LoadTexture("Assets/crate.png");
	rick = LoadTexture("Assets/rick_head.png");

	background = LoadTexture("Assets/Ruby Table base.png");
	background_layer = LoadTexture("Assets/Ruby Table base2.png");
	
	

	spring = LoadTexture("Assets/spring.png");
	entities.emplace_back(new Spring(App->physics, spring));

	flipper = LoadTexture("Assets/flipper.png");
	
	Flipper* leftFlipper = new Flipper(App->physics, flipper, true);
	leftFlipper->body->ctype = ColliderType::FLIPPER;
	entities.emplace_back(leftFlipper);
	flipper = LoadTexture("Assets/flipper.png");
	Flipper* rightFlipper = new Flipper(App->physics, flipper, false);
	rightFlipper->body->ctype = ColliderType::FLIPPER;
	entities.emplace_back(rightFlipper);

	entities.emplace_back(new Circle(App->physics, 242.0f * SCALE, 320.0f * SCALE, circle));
	entities[(entities.size() - 1)]->setListener(this);

	
	return ret;
}

// Load assets
bool ModuleGame::CleanUp()
{
	LOG("Unloading Intro scene");

	StopMusicStream(music);
	UnloadMusicStream(music);

	//CloseAudioDevice();

	return true;
}

// Update: draw background
update_status ModuleGame::Update()
{
	UpdateMusicStream(music);

	timePlayed = GetMusicTimePlayed(music) / GetMusicTimeLength(music);
	if (timePlayed > 1.0f) timePlayed = 1.0f;

	if(IsKeyPressed(KEY_SPACE))
	{
		ray_on = !ray_on;
		ray.x = GetMouseX();
		ray.y = GetMouseY();
	}

	if (IsKeyPressed(KEY_ONE) && App->physics->getDebug())
	{
		entities.emplace_back(new Circle(App->physics, GetMouseX(), GetMouseY(), circle));
		entities[(entities.size()-1)]->setListener(this);
	}

	if (IsKeyPressed(KEY_UP))
	{
		circle = LoadTexture("Assets/ball.png");
		entities.emplace_back(new Circle(App->physics, 242.0f * SCALE, 320.0f * SCALE, circle));
		entities[(entities.size() - 1)]->setListener(this);
		
	}
	if (IsKeyPressed(KEY_LEFT)) {
		App->audio->PlayFx(flipper_fx);
	}

	if (IsKeyPressed(KEY_RIGHT)) {
		App->audio->PlayFx(flipper_fx);
	}
	if (IsKeyPressed(KEY_DOWN))
	{
		App->audio->PlayFx(spring_fx);

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
	App->renderer->Draw(background, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, &rect);

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

	// ray -----------------
	if(ray_on == true)
	{
		vec2f destination((float)(mouse.x-ray.x), (float)(mouse.y-ray.y));
		destination.Normalize();
		destination *= (float)ray_hit;

		DrawLine(ray.x, ray.y, (int)(ray.x + destination.x), (int)(ray.y + destination.y), RED);

		if (normal.x != 0.0f)
		{
			DrawLine((int)(ray.x + destination.x), (int)(ray.y + destination.y), (int)(ray.x + destination.x + normal.x * 25.0f), (int)(ray.y + destination.y + normal.y * 25.0f), Color{ 100, 255, 100, 255 });
		}
	}
	

	// Llama a la función para dibujar "SCORE" y el puntaje
	DrawScore();
	//TraceLog(LOG_INFO, "%d", entities.size());

	return UPDATE_CONTINUE;
}

// TODO 8: Now just define collision callback for the circle and play bonus_fx audio
void ModuleGame::OnCollision(PhysBody* A, PhysBody* B) {
	if (A->ctype == ColliderType::FLIPPER) {
		int randomNum = rand() % 9000 + 1000;
		score += randomNum;
		App->audio->PlayFx(bonus_fx); 
	}
}

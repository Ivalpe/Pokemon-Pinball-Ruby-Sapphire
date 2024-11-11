#pragma once

#include "Module.h"
#include "Globals.h"

#include "box2d\box2d.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)


enum ColliderType {
	FLIPPER,
	SPRING,
	BALL,
	WALL,
	NORMAL,
	BOUNCE, 
	PIKACHU
};
class PhysBody
{
public:
	PhysBody() : body(NULL)
	{}

	void GetPhysicPosition(int& x, int& y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;
public:
	int width, height;
	b2Body* body;
	Module* listenerptr = nullptr;
	ColliderType ctype;
};

// Module --------------------------------------
class ModulePhysics : public Module, b2ContactListener
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();
	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();
	PhysBody* createFlipper(bool right);
	PhysBody* CreateCircle(int x, int y, int radius);
	PhysBody* CreateCollisionCircle(int x, int y, int radius);
	PhysBody* CreateRectangle(int x, int y, int width, int height);
	PhysBody* CreateStaticRectangle(int x, int y, int width, int height);
	PhysBody* CreateCollisionRectangle(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, const int* points, int size);
	void BeginContact(b2Contact* contact) override;
	PhysBody* CreatePinball(b2Vec2* coords, int size, ColliderType ct);
	PhysBody* CreateKicker();
	PhysBody* CreatePikaSpring(bool right);

	bool getDebug() {
		return debug;
	}

private:

	bool debug;
	b2World* world;

	//Kicker
	b2Body* kickerBody;
	b2Body* pikaBody;
	b2Body* pikaBodytwo;
	b2RevoluteJoint* leftFlipper;
	b2RevoluteJoint* rightFlipper;
	bool kickerActivated = false;
	bool pikaActivated = false;
	bool pikatwoActivated = false;
	const float kickerForce = -500.0f;
	const float pikaForce = -150.0f;
	
};

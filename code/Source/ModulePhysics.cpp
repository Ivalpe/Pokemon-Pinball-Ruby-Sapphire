#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"

#include "p2Point.h"

#include <math.h>

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	// TODO 3: You need to make ModulePhysics class a contact listener
	world->SetContactListener(this);

	// big static circle as "ground" in the middle of the screen
	int x = (int)(SCREEN_WIDTH / 2);
	int y = (int)(SCREEN_HEIGHT / 1.5f);
	int diameter = SCREEN_WIDTH / 2;

	//b2BodyDef body;
	//body.type = b2_staticBody;
	//body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	//b2Body* big_ball = world->CreateBody(&body);

	//b2CircleShape shape;
	//shape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	//b2FixtureDef fixture;
	//fixture.shape = &shape;
	//big_ball->CreateFixture(&fixture);
	/*
	b2Vec2 pinball[] = {
b2Vec2(PIXEL_TO_METERS(140), PIXEL_TO_METERS(423.957)),
b2Vec2(PIXEL_TO_METERS(0.0869565), PIXEL_TO_METERS(-4.82609)),
b2Vec2(PIXEL_TO_METERS(60), PIXEL_TO_METERS(-39.9565)),
b2Vec2(PIXEL_TO_METERS(61), PIXEL_TO_METERS(-39.9565)),
b2Vec2(PIXEL_TO_METERS(61.0222), PIXEL_TO_METERS(-28.001)),
b2Vec2(PIXEL_TO_METERS(75.9844), PIXEL_TO_METERS(-27.9565)),
b2Vec2(PIXEL_TO_METERS(75.9556), PIXEL_TO_METERS(-91.8676)),
b2Vec2(PIXEL_TO_METERS(72.9556), PIXEL_TO_METERS(-97.9121)),
b2Vec2(PIXEL_TO_METERS(69), PIXEL_TO_METERS(-100.957)),
b2Vec2(PIXEL_TO_METERS(56.0667), PIXEL_TO_METERS(-102.001)),
b2Vec2(PIXEL_TO_METERS(52.0222), PIXEL_TO_METERS(-106.001)),
b2Vec2(PIXEL_TO_METERS(52.0222), PIXEL_TO_METERS(-138.334)),
b2Vec2(PIXEL_TO_METERS(62.7188), PIXEL_TO_METERS(-150.3)),
b2Vec2(PIXEL_TO_METERS(72.875), PIXEL_TO_METERS(-166.863)),
b2Vec2(PIXEL_TO_METERS(83.087), PIXEL_TO_METERS(-191.87)),
b2Vec2(PIXEL_TO_METERS(87.6364), PIXEL_TO_METERS(-216.957)),
b2Vec2(PIXEL_TO_METERS(87.8182), PIXEL_TO_METERS(-232.32)),
b2Vec2(PIXEL_TO_METERS(85.9091), PIXEL_TO_METERS(-251.502)),
b2Vec2(PIXEL_TO_METERS(80.4545), PIXEL_TO_METERS(-274.866)),
b2Vec2(PIXEL_TO_METERS(79.3612), PIXEL_TO_METERS(-284.272)),
b2Vec2(PIXEL_TO_METERS(81.6919), PIXEL_TO_METERS(-284.889)),
b2Vec2(PIXEL_TO_METERS(89.49), PIXEL_TO_METERS(-268.351)),
b2Vec2(PIXEL_TO_METERS(92.6646), PIXEL_TO_METERS(-253.809)),
b2Vec2(PIXEL_TO_METERS(94.6366), PIXEL_TO_METERS(-234.155)),
b2Vec2(PIXEL_TO_METERS(95.6507), PIXEL_TO_METERS(-215.301)),
b2Vec2(PIXEL_TO_METERS(95.8782), PIXEL_TO_METERS(-15.8121)),
b2Vec2(PIXEL_TO_METERS(110.063), PIXEL_TO_METERS(-15.9655)),
b2Vec2(PIXEL_TO_METERS(109.917), PIXEL_TO_METERS(-234.707)),
b2Vec2(PIXEL_TO_METERS(107.219), PIXEL_TO_METERS(-255.205)),
b2Vec2(PIXEL_TO_METERS(99.1211), PIXEL_TO_METERS(-278.717)),
b2Vec2(PIXEL_TO_METERS(85.2273), PIXEL_TO_METERS(-301.354)),
b2Vec2(PIXEL_TO_METERS(59.2841), PIXEL_TO_METERS(-324.559)),
b2Vec2(PIXEL_TO_METERS(38), PIXEL_TO_METERS(-337.411)),
b2Vec2(PIXEL_TO_METERS(14.5455), PIXEL_TO_METERS(-346.32)),
b2Vec2(PIXEL_TO_METERS(-6.36364), PIXEL_TO_METERS(-350.684)),
b2Vec2(PIXEL_TO_METERS(-30.7273), PIXEL_TO_METERS(-350.502)),
b2Vec2(PIXEL_TO_METERS(-55.4545), PIXEL_TO_METERS(-346.047)),
b2Vec2(PIXEL_TO_METERS(-76.0909), PIXEL_TO_METERS(-337.047)),
b2Vec2(PIXEL_TO_METERS(-91.8182), PIXEL_TO_METERS(-326.502)),
b2Vec2(PIXEL_TO_METERS(-108.545), PIXEL_TO_METERS(-308.411)),
b2Vec2(PIXEL_TO_METERS(-119.727), PIXEL_TO_METERS(-287.593)),
b2Vec2(PIXEL_TO_METERS(-125.545), PIXEL_TO_METERS(-263.593)),
b2Vec2(PIXEL_TO_METERS(-127.182), PIXEL_TO_METERS(-240.411)),
b2Vec2(PIXEL_TO_METERS(-128.545), PIXEL_TO_METERS(-240.502)),
b2Vec2(PIXEL_TO_METERS(-128.455), PIXEL_TO_METERS(-215.684)),
b2Vec2(PIXEL_TO_METERS(-123.273), PIXEL_TO_METERS(-193.502)),
b2Vec2(PIXEL_TO_METERS(-116.818), PIXEL_TO_METERS(-174.411)),
b2Vec2(PIXEL_TO_METERS(-103.545), PIXEL_TO_METERS(-151.32)),
b2Vec2(PIXEL_TO_METERS(-92.3636), PIXEL_TO_METERS(-138.32)),
b2Vec2(PIXEL_TO_METERS(-92.6364), PIXEL_TO_METERS(-114.411)),
b2Vec2(PIXEL_TO_METERS(-95.5455), PIXEL_TO_METERS(-110.502)),
b2Vec2(PIXEL_TO_METERS(-98.7273), PIXEL_TO_METERS(-109.593)),
b2Vec2(PIXEL_TO_METERS(-106.636), PIXEL_TO_METERS(-109.411)),
b2Vec2(PIXEL_TO_METERS(-111.455), PIXEL_TO_METERS(-107.502)),
b2Vec2(PIXEL_TO_METERS(-114.455), PIXEL_TO_METERS(-103.684)),
b2Vec2(PIXEL_TO_METERS(-116.364), PIXEL_TO_METERS(-99.5929)),
b2Vec2(PIXEL_TO_METERS(-116.455), PIXEL_TO_METERS(-27.4111)),
b2Vec2(PIXEL_TO_METERS(-100.727), PIXEL_TO_METERS(-27.5929)),
b2Vec2(PIXEL_TO_METERS(-100.636), PIXEL_TO_METERS(-39.5929)),
b2Vec2(PIXEL_TO_METERS(-99.4545), PIXEL_TO_METERS(-39.3202)),
b2Vec2(PIXEL_TO_METERS(-40.5455), PIXEL_TO_METERS(-4.50198)),
b2Vec2(PIXEL_TO_METERS(-41.303), PIXEL_TO_METERS(4.10408)),
b2Vec2(PIXEL_TO_METERS(-151.117), PIXEL_TO_METERS(3.07037)),
b2Vec2(PIXEL_TO_METERS(-146.792), PIXEL_TO_METERS(-437.79)),
b2Vec2(PIXEL_TO_METERS(128.347), PIXEL_TO_METERS(-441.299)),
b2Vec2(PIXEL_TO_METERS(126.466), PIXEL_TO_METERS(4.58928)),
	};
	*/


	b2Vec2 pinball[] = {
	b2Vec2(140, 423.957),
	b2Vec2(0.0869565, -4.82609),
	b2Vec2(60, -39.9565),
	b2Vec2(61, -39.9565),
	b2Vec2(61.0222, -28.001),
	b2Vec2(75.9844, -27.9565),
	b2Vec2(75.9556, -91.8676),
	b2Vec2(72.9556, -97.9121),
	b2Vec2(69, -100.957),
	b2Vec2(56.0667, -102.001),
	b2Vec2(52.0222, -106.001),
	b2Vec2(52.0222, -138.334),
	b2Vec2(62.7188, -150.3),
	b2Vec2(72.875, -166.863),
	b2Vec2(83.087, -191.87),
	b2Vec2(87.6364, -216.957),
	b2Vec2(87.8182, -232.32),
	b2Vec2(85.9091, -251.502),
	b2Vec2(80.4545, -274.866),
	b2Vec2(79.3612, -284.272),
	b2Vec2(81.6919, -284.889),
	b2Vec2(89.49, -268.351),
	b2Vec2(92.6646, -253.809),
	b2Vec2(94.6366, -234.155),
	b2Vec2(95.6507, -215.301),
	b2Vec2(95.8782, -15.8121),
	b2Vec2(110.063, -15.9655),
	b2Vec2(109.917, -234.707),
	b2Vec2(107.219, -255.205),
	b2Vec2(99.1211, -278.717),
	b2Vec2(85.2273, -301.354),
	b2Vec2(59.2841, -324.559),
	b2Vec2(38, -337.411),
	b2Vec2(14.5455, -346.32),
	b2Vec2(-6.36364, -350.684),
	b2Vec2(-30.7273, -350.502),
	b2Vec2(-55.4545, -346.047),
	b2Vec2(-76.0909, -337.047),
	b2Vec2(-91.8182, -326.502),
	b2Vec2(-108.545, -308.411),
	b2Vec2(-119.727, -287.593),
	b2Vec2(-125.545, -263.593),
	b2Vec2(-127.182, -240.411),
	b2Vec2(-128.545, -240.502),
	b2Vec2(-128.455, -215.684),
	b2Vec2(-123.273, -193.502),
	b2Vec2(-116.818, -174.411),
	b2Vec2(-103.545, -151.32),
	b2Vec2(-92.3636, -138.32),
	b2Vec2(-92.6364, -114.411),
	b2Vec2(-95.5455, -110.502),
	b2Vec2(-98.7273, -109.593),
	b2Vec2(-106.636, -109.411),
	b2Vec2(-111.455, -107.502),
	b2Vec2(-114.455, -103.684),
	b2Vec2(-116.364, -99.5929),
	b2Vec2(-116.455, -27.4111),
	b2Vec2(-100.727, -27.5929),
	b2Vec2(-100.636, -39.5929),
	b2Vec2(-99.4545, -39.3202),
	b2Vec2(-40.5455, -4.50198),
	b2Vec2(-41.303, 4.10408),
	b2Vec2(-151.117, 3.07037),
	b2Vec2(-146.792, -437.79),
	b2Vec2(128.347, -441.299),
	b2Vec2(126.466, 4.58928),
	b2Vec2(-0.0909091, 3.31621),
	};

	CreatePinball(pinball, 66);
	/*
	b2Vec2 pinball2[] = {
	b2Vec2(PIXEL_TO_METERS(153.813), PIXEL_TO_METERS(396)),
	b2Vec2(PIXEL_TO_METERS(42.9375), PIXEL_TO_METERS(-29.125)),
	b2Vec2(PIXEL_TO_METERS(43), PIXEL_TO_METERS(-63.875)),
	b2Vec2(PIXEL_TO_METERS(39.6875), PIXEL_TO_METERS(-66)),
	b2Vec2(PIXEL_TO_METERS(36.25), PIXEL_TO_METERS(-62.75)),
	b2Vec2(PIXEL_TO_METERS(36.1875), PIXEL_TO_METERS(-37.1875)),
	b2Vec2(PIXEL_TO_METERS(31.25), PIXEL_TO_METERS(-30.125)),
	b2Vec2(PIXEL_TO_METERS(5.6875), PIXEL_TO_METERS(-6.1875))
	};
	CreatePinball(pinball2, 7);
	*/
	CreateKicker();

	return true;
}

update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	// TODO: HomeWork
	/*
	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
	}
	*/

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	// TODO 4: add a pointer to PhysBody as UserData to the body
	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	pbody->width = pbody->height = radius;

	b->GetUserData().pointer = (uintptr_t)pbody;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);
	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	pbody->width = (int)(width * 0.5f);
	pbody->height = (int)(height * 0.5f);

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, const int* points, int size)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (int i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	pbody->width = pbody->height = 0;

	return pbody;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if (IsKeyPressed(KEY_F1))
		debug = !debug;

	//Kicker movement
	if (IsKeyDown(KEY_DOWN)) {
		kickerBody->ApplyForceToCenter(b2Vec2(0, kickerForce), true);
		kickerActivated = true;
	}

	if (IsKeyReleased(KEY_DOWN)) {
		kickerActivated = false;
	}

	if (!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
				// Draw circles ------------------------------------------------
			case b2Shape::e_circle:
			{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				b2Vec2 pos = f->GetBody()->GetPosition();

				DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), (float)METERS_TO_PIXELS(shape->m_radius), Color{ 0, 0, 0, 128 });
			}
			break;

			// Draw polygons ------------------------------------------------
			case b2Shape::e_polygon:
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
				int32 count = polygonShape->m_count;
				b2Vec2 prev, v;

				for (int32 i = 0; i < count; ++i)
				{
					v = b->GetWorldPoint(polygonShape->m_vertices[i]);
					if (i > 0)
						DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), RED);

					prev = v;
				}

				v = b->GetWorldPoint(polygonShape->m_vertices[0]);
				DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), RED);
			}
			break;

			// Draw chains contour -------------------------------------------
			case b2Shape::e_chain:
			{
				b2ChainShape* shape = (b2ChainShape*)f->GetShape();
				b2Vec2 prev, v;

				for (int32 i = 0; i < shape->m_count; ++i)
				{
					v = b->GetWorldPoint(shape->m_vertices[i]);
					if (i > 0)
						DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), GREEN);
					prev = v;
				}

				v = b->GetWorldPoint(shape->m_vertices[0]);
				DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), GREEN);
			}
			break;

			// Draw a single segment(edge) ----------------------------------
			case b2Shape::e_edge:
			{
				b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
				b2Vec2 v1, v2;

				v1 = b->GetWorldPoint(shape->m_vertex0);
				v1 = b->GetWorldPoint(shape->m_vertex1);
				DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), BLUE);
			}
			break;
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

//void PhysBody::GetPosition(int& x, int &y) const
//{
//	b2Vec2 pos = body->GetPosition();
//	x = METERS_TO_PIXELS(pos.x) - (width);
//	y = METERS_TO_PIXELS(pos.y) - (height);
//}

void PhysBody::GetPhysicPosition(int& x, int& y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x);
	y = METERS_TO_PIXELS(pos.y);
}

float PhysBody::GetRotation() const
{
	return body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// TODO 1: Write the code to return true in case the point
	// is inside ANY of the shapes contained by this body
	for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
		if (f->GetShape()->TestPoint(body->GetTransform(), p)) return true;
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	// TODO 2: Write code to test a ray cast between both points provided. If not hit return -1
	// if hit, fill normal_x and normal_y and return the distance between x1,y1 and it's colliding point

	int ret = -1;

	b2Transform transform;
	transform.SetIdentity();

	b2RayCastInput input;
	input.p1 = { PIXEL_TO_METERS(x1),PIXEL_TO_METERS(y1) };
	input.p2 = { PIXEL_TO_METERS(x2),PIXEL_TO_METERS(y2) };
	input.maxFraction = 1.0f;
	int32 childIndex = 0;

	b2RayCastOutput output;

	for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
		if (f->GetShape()->RayCast(&output, input, body->GetTransform(), childIndex)) {
			normal_x = output.normal.x;
			normal_y = output.normal.y;

			float dx = (x2 - x1);
			float dy = (y2 - y1);
			float dist = sqrt((dx * dx) - (dy * dy));
			return (dist * output.fraction);
		}
	}

	return ret;
}

// TODO 3
void ModulePhysics::BeginContact(b2Contact* contact) {
	TraceLog(LOG_INFO, ":D");
	PhysBody* A = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	PhysBody* B = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

	// TODO 7: Call the listeners that are not NULL
	if (A != nullptr && A->listenerptr != nullptr) {
		A->listenerptr->OnCollision(A, B);
	}

	if (B != nullptr && B->listenerptr != nullptr) {
		B->listenerptr->OnCollision(B, A);
	}
}

void ModulePhysics::CreatePinball(b2Vec2* coords, int size) {

	for (int i = 1; i <= size; i++) {
		coords[i].Set(PIXEL_TO_METERS((coords[i].x + coords[0].x) * 2.0f),
					PIXEL_TO_METERS((coords[i].y + coords[0].y) * 2.0f));
	}

	coords[0] = { PIXEL_TO_METERS(coords[0].x), PIXEL_TO_METERS(coords[0].y) };

	b2BodyDef body;
	body.type = b2_dynamicBody;
	//body.position.Set(PIXEL_TO_METERS((GetScreenWidth() / 2) + 0.5), PIXEL_TO_METERS(GetScreenHeight()) + 0.2);
	TraceLog(LOG_INFO, "COORD X: %f, COORD Y: %f", coords[0].x, coords[0].y);
	body.position.Set(coords[0].x, coords[0].y);

	b2Body* b = world->CreateBody(&body);

	b2ChainShape chain;
	TraceLog(LOG_INFO, "LENGTH %f", coords->Length());
	chain.CreateLoop(coords, size + 1);

	b2FixtureDef fd;
	fd.shape = &chain;
	fd.density = 1;

	b->CreateFixture(&fd);
}

void ModulePhysics::CreateKicker() {
	float scalekicker = 50.0f;
	float initialX = 242.0f * SCALE;
	float initialY = 380.0f * SCALE;
	float kickerWidth = PIXEL_TO_METERS(10.0f);
	float kickerHeight = PIXEL_TO_METERS(40.0f);


	// Body
	b2BodyDef springBodyDef;
	springBodyDef.type = b2_dynamicBody;
	springBodyDef.position.Set(initialX, initialY);
	kickerBody = world->CreateBody(&springBodyDef);

	// Shape
	b2PolygonShape springShape;
	springShape.SetAsBox(kickerWidth, kickerHeight);

	// Fixture
	b2FixtureDef springFixtureDef;
	springFixtureDef.shape = &springShape;
	springFixtureDef.density = 1.0f;
	springFixtureDef.friction = 0.5f;
	kickerBody->CreateFixture(&springFixtureDef);

	// Joint
	b2BodyDef anchorBodyDef;
	anchorBodyDef.type = b2_staticBody;
	anchorBodyDef.position.Set(initialX / scalekicker, (initialY - kickerHeight) / scalekicker);
	b2Body* anchorBody = world->CreateBody(&anchorBodyDef);

	b2PrismaticJointDef jointDef;
	jointDef.bodyA = anchorBody;
	jointDef.bodyB = kickerBody;
	jointDef.localAnchorA.Set(0, 0);
	jointDef.localAnchorB.Set(0, 0);
	jointDef.enableLimit = true;
	jointDef.lowerTranslation = -50.0f / scalekicker;
	jointDef.upperTranslation = 0.0f;
	jointDef.localAxisA.Set(0, 1);
	world->CreateJoint(&jointDef);
}
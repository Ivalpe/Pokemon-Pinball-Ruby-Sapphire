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

	for(int i = 0; i < size / 2; ++i)
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
	if(IsKeyPressed(KEY_F1))
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					
					DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), (float)METERS_TO_PIXELS(shape->m_radius), Color{0, 0, 0, 128});
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->m_count;
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->m_vertices[i]);
						if(i > 0)
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

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
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
		if(f->GetShape()->TestPoint(body->GetTransform(), p)) return true;
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
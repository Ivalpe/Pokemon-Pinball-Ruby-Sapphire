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
	CreatePinball(pinball, 91);

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
	CreatePinball(pinball2, 7);

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
	CreatePinball(pinball3, 7);

	b2Vec2 pinball4[] = {
	b2Vec2(297.5f, 779.5f),
	b2Vec2(68.0f, -43.25f),
	b2Vec2(76.25f, -50.5f),
	b2Vec2(83.0f, -61.5f),
	b2Vec2(83.5f, -112.25f),
	b2Vec2(88.0f, -118.75f),
	b2Vec2(91.5f, -119.0f),
	b2Vec2(95.25f, -114.0f),
	b2Vec2(95.75f, -44.0f),
	b2Vec2(83.0f, -35.25f),
	b2Vec2(10.0f, 11.25f)
	};
	CreatePinball(pinball4, 10);

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
	CreatePinball(pinball5, 35);

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

	CreatePinball(pinball6, 7);

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
						DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), PURPLE);
					prev = v;
				}

				v = b->GetWorldPoint(shape->m_vertices[0]);
				DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), PURPLE);
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

	coords[0].Set(PIXEL_TO_METERS(coords[0].x / 2.0f), PIXEL_TO_METERS(coords[0].y / 2.0f));
	for (int i = 1; i <= size; i++) {
		coords[i].Set(PIXEL_TO_METERS((coords[i].x) + coords[0].x),
			PIXEL_TO_METERS((coords[i].y) + coords[0].y));
	}



	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(coords[0].x, coords[0].y);

	b2Body* b = world->CreateBody(&body);

	b2ChainShape chain;
	chain.CreateLoop(coords, size + 1);

	b2FixtureDef fd;
	fd.shape = &chain;
	fd.density = 1;

	b->CreateFixture(&fd);
}

PhysBody* ModulePhysics::CreateKicker() {
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

	PhysBody* pbody = new PhysBody();
	pbody->body = kickerBody;

	return pbody;
}
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"

#include "p2Point.h"

#include <math.h>

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	debug = false;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	b2BodyDef bd;
	ground = world->CreateBody(&bd);


	int x = (int)(SCREEN_WIDTH / 2);
	int y = (int)(SCREEN_HEIGHT / 1.5f);
	int diameter = SCREEN_WIDTH / 2;

	return true;
}

PhysBody* ModulePhysics::createFlipper(bool right) {
	if (right) {
		PhysBody* rightBody1 = CreateRectangle(284, 780, 68, 14);
		PhysBody* rightBody2 = CreateStaticRectangle(311, 779, 4, 4);

		b2RevoluteJointDef jointDef;

		jointDef.Initialize(rightBody1->body, rightBody2->body, rightBody2->body->GetWorldCenter());
		jointDef.collideConnected = false;
		jointDef.enableLimit = true;
		jointDef.motorSpeed = 15.0f;
		jointDef.maxMotorTorque = 1000.0f;
		jointDef.lowerAngle = -0.25f * b2_pi;  //  45 degrees
		jointDef.upperAngle = 0.15f * b2_pi; // -90 degrees
		jointDef.enableMotor = true;

		rightFlipper = (b2RevoluteJoint*)world->CreateJoint(&jointDef);

		PhysBody* pbody = new PhysBody();
		pbody->body = rightBody1->body;
		rightBody1->body->GetUserData().pointer = (uintptr_t)pbody;

		return pbody;
	}
	else {
		PhysBody* leftBody1 = CreateRectangle(190, 778, 68, 14);
		PhysBody* leftBody2 = CreateStaticRectangle(167, 777, 4, 4);

		b2RevoluteJointDef jointDef;

		jointDef.Initialize(leftBody1->body, leftBody2->body, leftBody2->body->GetWorldCenter());
		jointDef.collideConnected = false;
		jointDef.enableLimit = true;
		jointDef.motorSpeed = -15.0f;
		jointDef.maxMotorTorque = 1000.0f;
		jointDef.lowerAngle = -0.15f * b2_pi; // -90 degrees
		jointDef.upperAngle = 0.25f * b2_pi;  //  45 degrees
		jointDef.enableMotor = true;

		leftFlipper = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
		PhysBody* pbody = new PhysBody();
		pbody->body = leftBody1->body;
		leftBody1->body->GetUserData().pointer = (uintptr_t)pbody;

		//PhysBody* pbody = new PhysBody();

		return pbody;
	}
}

update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);
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

	PhysBody* pbody = new PhysBody();
	pbody->ctype = ColliderType::BALL;
	pbody->body = b;
	pbody->width = pbody->height = radius;

	b->GetUserData().pointer = (uintptr_t)pbody;

	return pbody;
}

PhysBody* ModulePhysics::CreateCollisionCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.restitution = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->ctype = ColliderType::BOUNCE;
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

PhysBody* ModulePhysics::CreateStaticRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
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

PhysBody* ModulePhysics::CreateCollisionRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;             
	fixture.restitution = 1.0f;         

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->ctype = ColliderType::BOUNCE;
	pbody->body = b;
	pbody->width = (int)(width * 0.5f);
	pbody->height = (int)(height * 0.5f);

	b->GetUserData().pointer = (uintptr_t)pbody;

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

update_status ModulePhysics::PostUpdate()
{
	if (IsKeyPressed(KEY_F1))
		debug = !debug;

	b2Body* mouseSelect = nullptr;
	Vector2 mousePosition = GetMousePosition();
	b2Vec2 pMousePosition = b2Vec2(PIXEL_TO_METERS(mousePosition.x), PIXEL_TO_METERS(mousePosition.y));

	//Kicker movement
	if (IsKeyDown(KEY_DOWN)) {
		kickerBody->ApplyForceToCenter(b2Vec2(0, kickerForce), true);
		kickerActivated = true;
	}

	if (IsKeyReleased(KEY_DOWN)) {
		kickerActivated = false;
	}

	if (IsKeyDown(KEY_LEFT)) {
		leftFlipper->SetMotorSpeed(15);
		pikaBodytwo->ApplyForceToCenter(b2Vec2(0, pikaForce), true);
		pikatwoActivated = true;
	}

	if (IsKeyReleased(KEY_LEFT)) {
		leftFlipper->SetMotorSpeed(-15);
		pikatwoActivated = false;
	}

	if (IsKeyDown(KEY_RIGHT)) {
		rightFlipper->SetMotorSpeed(-15);
		pikaBody->ApplyForceToCenter(b2Vec2(0, pikaForce), true);
		pikaActivated = true;
	}

	if (IsKeyReleased(KEY_RIGHT)) {
		rightFlipper->SetMotorSpeed(15);
		pikaActivated = false;
	}


	if (!debug)
		return UPDATE_CONTINUE;

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

			if (mouse_joint == nullptr && mouseSelect == nullptr && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

				if (f->TestPoint(pMousePosition)) {
					mouseSelect = b;
				}
			}
		}
	}

	// If a body was selected we will attach a mouse joint to it
	// so we can pull it around
	// TODO 2: If a body was selected, create a mouse joint
	// using mouse_joint class property
	if (mouseSelect) {
		b2MouseJointDef def;

		def.bodyA = ground;
		def.bodyB = mouseSelect;
		def.target = pMousePosition;
		def.damping = 0.5f;
		def.stiffness = 20.f;
		def.maxForce = 100.f * mouseSelect->GetMass();

		mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
	}

	// TODO 3: If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points
	else if (mouse_joint && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		mouse_joint->SetTarget(pMousePosition);
		b2Vec2 anchorPosition = mouse_joint->GetBodyB()->GetPosition();
		anchorPosition.x = METERS_TO_PIXELS(anchorPosition.x);
		anchorPosition.y = METERS_TO_PIXELS(anchorPosition.y);

		DrawLine(anchorPosition.x, anchorPosition.y, mousePosition.x, mousePosition.y, RED);
	}

	// TODO 4: If the player releases the mouse button, destroy the joint
	else if (mouse_joint && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
		world->DestroyJoint(mouse_joint);
		mouse_joint = nullptr;
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

	for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
		if (f->GetShape()->TestPoint(body->GetTransform(), p)) return true;
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
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

void ModulePhysics::BeginContact(b2Contact* contact) {
	PhysBody* A = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	PhysBody* B = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

	if (A != nullptr && A->listenerptr != nullptr) {
		A->listenerptr->OnCollision(A, B);
	}

	if (B != nullptr && B->listenerptr != nullptr) {
		B->listenerptr->OnCollision(B, A);
	}
}

PhysBody* ModulePhysics::CreatePinball(b2Vec2* coords, int size, ColliderType ct) {

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
	if (ct == ColliderType::BOUNCE) fd.restitution = 1.0f;

	b->CreateFixture(&fd);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->GetUserData().pointer = (uintptr_t)pbody;
	pbody->ctype = ct;
	return pbody;
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

	kickerBody->GetUserData().pointer = (uintptr_t)pbody;

	return pbody;
}
PhysBody* ModulePhysics::CreatePikaSpring(bool right) {
	float scalekicker = 50.0f;
	float initialX, initialY;
	if (right) {
		initialX = 419.0f;
		initialY = 770.0f;
	}
	else {
		initialX = 61.0f;
		initialY = 770.0f;
	}
	
	float pikaWidth = PIXEL_TO_METERS(27.0f);
	float pikaHeight = PIXEL_TO_METERS(28.0f);



	// Body
	b2BodyDef springBodyDef;
	springBodyDef.type = b2_dynamicBody;
	springBodyDef.position.Set(initialX, initialY);
	if(right) pikaBody = world->CreateBody(&springBodyDef);
	else pikaBodytwo = world->CreateBody(&springBodyDef);

	// Shape
	b2PolygonShape springShape;
	springShape.SetAsBox(pikaWidth/2.5, pikaHeight/2.5);

	// Fixture
	b2FixtureDef springFixtureDef;
	springFixtureDef.shape = &springShape;
	springFixtureDef.density = 1.0f;
	springFixtureDef.friction = 0.5f;
	if(right) pikaBody->CreateFixture(&springFixtureDef);
	else pikaBodytwo->CreateFixture(&springFixtureDef);

	// Joint
	b2BodyDef anchorBodyDef;
	anchorBodyDef.type = b2_staticBody;
	anchorBodyDef.position.Set((initialX) / scalekicker, (initialY - pikaHeight) / scalekicker);
	b2Body* anchorBody = world->CreateBody(&anchorBodyDef);

	b2PrismaticJointDef jointDef;
	jointDef.bodyA = anchorBody;
	if(right) jointDef.bodyB = pikaBody;
	else jointDef.bodyB = pikaBodytwo;
	jointDef.localAnchorA.Set(0, 0);
	jointDef.localAnchorB.Set(0, 0);
	jointDef.enableLimit = true;
	jointDef.lowerTranslation = -10.0f / scalekicker;
	jointDef.upperTranslation = 0.0f;
	jointDef.localAxisA.Set(0, 1);
	world->CreateJoint(&jointDef);


	PhysBody* pbody = new PhysBody();
	pbody->ctype = ColliderType::PIKACHU;
	if (right) {
		pbody->body = pikaBody;
		pikaBody->GetUserData().pointer = (uintptr_t)pbody;
	}
	else {
		pbody->body = pikaBodytwo;
		pikaBodytwo->GetUserData().pointer = (uintptr_t)pbody;
	}

	return pbody;
}

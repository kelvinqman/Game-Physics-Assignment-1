#include "Target.h"
#include "TextureManager.h"


Target::Target()
{
	TextureManager::Instance()->load("../Assets/textures/Circle.png","circle");

	const auto size = TextureManager::Instance()->getTextureSize("circle");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(100.0f, 100.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(TARGET);
}

Target::~Target()
= default;

void Target::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("circle", x, y, 0, 255, true);
}

void Target::update()
{
	// Target.cpp -> update()
	if (!pause) m_move();
	m_checkBounds();
}

void Target::clean()
{
}

void Target::m_move()
{
	// Target.cpp -> m_move()
	if (getTransform()->position.y > floor)
	{
		getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	}
	float deltaTime = 1.0f / 20.0f;
	getRigidBody()->velocity += (getRigidBody()->acceleration) * deltaTime;
	getTransform()->position += getRigidBody()->velocity * deltaTime;
}

void Target::m_checkBounds()
{
}

void Target::m_reset()
{
}

// Target.cpp -> member function
void Target::beThrowedToReachST() {
	getRigidBody()->velocity.x = 95 * cos(74.1 / 180 * 3.14);
	getRigidBody()->velocity.y = -95 * sin(74.1 / 180 * 3.14);
	getRigidBody()->acceleration.x = 0.0f;
	getRigidBody()->acceleration.y = gravity;
}
void Target::beThrowedInMaxDistance() {
	getRigidBody()->velocity.x = velocity * cos(angle);
	getRigidBody()->velocity.y = -velocity * sin(angle);
	getRigidBody()->acceleration.x = 0.0f;
	getRigidBody()->acceleration.y = gravity;
}
void Target::reSet() {
	getTransform()->position = glm::vec2(50.0f, floor);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	pause = false;
}
// Target.cpp -> member function
void Target::throwWithSpeed(float speed, float angle)
{
	getRigidBody()->velocity.x = speed * cos(angle);
	getRigidBody()->velocity.y = -speed * sin(angle);
	getRigidBody()->acceleration.x = 0;
	getRigidBody()->acceleration.y = isGravityEnabled ? gravity : 0;
}
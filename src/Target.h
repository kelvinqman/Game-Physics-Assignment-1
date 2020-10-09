#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "DisplayObject.h"

class Target final : public DisplayObject {
public:
	Target();
	~Target();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	// Target.h -> public:
	void beThrowedToReachST();
	void beThrowedInMaxDistance();
	void reSet();
private:
	void m_move();
	void m_checkBounds();
	void m_reset();

	// Target.h -> private:
	float scale = 1.0f;
	float velocity = 95.0f * scale;
	float angle = 45.0f / 180 * 3.14;
	float gravity = 9.8f * scale;
	float floor = 500.0f;
};


#endif /* defined (__TARGET__) */
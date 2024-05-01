#pragma once
#include <raymath.h>

typedef enum
{
	STATIC,
	KINEMATIC,
	DYNAMIC
} ekBodyType;

typedef struct ekBody
{
	ekBodyType body;
	// force -> acceleration -> velocity -> position
	Vector2 position;
	Vector2 velocity;
	Vector2 force;

	float mass;
	float inverseMass; // 1 / mass (static = 0)

	struct ekBody* next;
	struct ekBody* prev;
} ekBody;

inline void ApplyForce(ekBody* body, Vector2 force)
{
	body->force = Vector2Add(body->force, force);
}

inline void ClearForce(ekBody* body)
{
	body->force = Vector2Zero();
}
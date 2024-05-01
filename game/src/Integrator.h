#pragma once
#include "body.h"

void ExplicitEuler(ekBody* body, float timeStep)
{
	body->position = Vector2Add(body->position, Vector2Scale(body->velocity, timeStep));
	body->velocity = Vector2Add(body->velocity, Vector2Scale(Vector2Scale(body->force, 1 / body->mass), timeStep));
}
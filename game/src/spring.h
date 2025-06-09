#pragma once
#include "raylib.h"

typedef struct ekSpring
{
	struct ekBody* body1;
	struct ekBody* body2;
	float restLength;
	float k; // stiffness

	struct ekSpring* next;
	struct ekSpring* prev;
} ekSpring_t;

extern ekSpring_t* ekSprings;

ekSpring_t* CreateSpring(struct ekBody* body1, struct ekBody* body2, float restLength, float k);
void AddSpring(ekSpring_t* spring);
void DestroySpring(ekSpring_t* spring);
void DestroyAllSprings();

void ApplySpringForce(ekSpring_t* springs);
void ApplySpringForcePosition(Vector2 position, struct ekBody* body, float restLength, float k, float damping);
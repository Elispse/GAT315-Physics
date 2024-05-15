#pragma once
#include "raylib.h"

typedef struct ekContact
{
	struct ekBody* body1;
	struct ekBody* body2;

	float restitution;
	float depth;
	Vector2 normal;

	struct ekContact* next;
} ekContact_t;

void AddContact(ekContact_t* contact, ekContact_t** contacts);
void DestroyAllContacts(ekContact_t** contacts);

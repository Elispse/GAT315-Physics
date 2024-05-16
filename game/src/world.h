#pragma once
#include "raylib.h"
#include "body.h"

typedef struct ekBody ekBody;

extern ekBody* ekBodies;
extern int ekBodyCount;
extern Vector2 ekGravity;

ekBody* CreateBody(Vector2 position, float mass, ekBodyType bodyType);
void AddBody(ekBody* body);
void DestroyBody(ekBody* body);
void DestroyAllBodies();
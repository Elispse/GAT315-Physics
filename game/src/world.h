#pragma once
#include <body.h>
extern ekBody* ekBodies;
extern int ekBodyCount;
extern Vector2 ekGravity;

ekBody* CreateBody();
void DestroyBody(ekBody* body);
void DestroyAllBodies();
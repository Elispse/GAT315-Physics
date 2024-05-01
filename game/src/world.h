#pragma once
#include <body.h>
extern ekBody* ekBodies;
extern int ekBodyCount;

ekBody* CreateBody();
void DestroyBody(ekBody* body);
void DestroyAllBodies();
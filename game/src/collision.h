#pragma once
#include "raylib.h"

typedef struct ekContact ekContact_t;
typedef struct ekBody ekBody;

void CreateContacts(ekBody* bodies, ekContact_t** contacts);
ekContact_t* GenerateContact(ekBody* body1, ekBody* body2);

void SeparateContacts(ekContact_t* contacts);
void ResolveContacts(ekContact_t* contacts);
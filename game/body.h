#pragma once
#include "raylib.h"
#include "raymath.h"

typedef enum
{
    BT_DYNAMIC,
    BT_STATIC,
    BT_KINEMATIC
}ekBodyType;

typedef enum
{
    FM_FORCE,
    FM_IMPULSE,
    FM_VELOCITY
}ekForceMode;

typedef struct ekBody
{
    enum ekBodyType type;

    //acceleration -> velocity -> position
    Vector2 position;
    Vector2 prevPosition;
    Vector2 prevPositions[100];
    
    Vector2 velocity;
    Vector2 acceleration;
    Vector2 force;

    float mass;
    float inverseMass; // 1/mass (static = 0)
    float gravityScale;
    float damping;

    float restitution;

    Color color;
    int shape;

    struct ekBody* next;
    struct ekBody* prev;
} ekBody;


inline void ApplyForce(ekBody* body, Vector2 force, ekForceMode fmode)
{
    if (body->type != BT_DYNAMIC) return;

    switch (fmode)
    {
    case FM_FORCE:
        body->force = Vector2Add(body->force, force);
        break;
    case FM_IMPULSE:
        //applies a sudden change in momentum(velocity)
        body->velocity = Vector2Scale(force, body->inverseMass);
        break;
    case FM_VELOCITY:
        body->velocity = force;
        break;
    }
}

inline void ClearForce(ekBody* body)
{
    body->force = Vector2Zero();
}

void Step(ekBody* body, float timestep);
#include "raylib.h"
#include "raymath.h"
#include <world.h>
#include <stdlib.h>
#include <assert.h>
#include "Integrator.h"
#include "body.h"
#include "mathf.h"

int main(void)
{
	InitWindow(1280, 720, "Pysics Engine");
	while (!WindowShouldClose())
	{
		// update 
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();

		if (IsMouseButtonDown(0))
		{
			ekBody* body = CreateBody();
			body->position = position;
			body->mass = GetRandomFloatValue(1, 10);
			ApplyForce(body, CreateVector2(GetRandomFloatValue(-50, 50), GetRandomFloatValue(-50, 50)));
		}

		//aply force
		ekBody* body = ekBodies;
		while (body)
		{
			ApplyForce(body, CreateVector2(0, -50));
			body = body->next;
		}

		//update bodies
		body = ekBodies;
		while (body) // do while we have a valid pointer, will be NULL at the end of the list
		{
			// update body position
			/*body->position.x = GetRandomValue(1, 800);
			body->position.y = GetRandomValue(1, 450);*/
			//body->position = Vector2Add(body - position, body - force);
			
			ExplicitEuler(body, dt);
			ClearForce(body);
			// draw body
			body = body->next; // get next body
		}

		//render
		BeginDrawing();
		ClearBackground(RAYWHITE);

		// update
		body = ekBodies;
		while (body) // do while we have a valid pointer, will be NULL at the end of the list
		{
			// draw body
			DrawCircle((int)body->position.x, (int)body->position.y, body->mass, RED);
			body = body->next; // get next body
		}

		EndDrawing();
	}

	while (ekBodies)
	{
		DestroyBody(ekBodies);
	}


	CloseWindow();
	return 0;
};
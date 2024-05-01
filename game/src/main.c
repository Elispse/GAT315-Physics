#include "raylib.h"
#include <world.h>

int main(void)
{
	InitWindow(800, 450, "raylib [core] example - basic window");
	while (!WindowShouldClose())
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			CreateBody();
		}

		BeginDrawing();
		ClearBackground(RAYWHITE);
		// update / draw bodies
		Body* body = bodies;
		while (body) // do while we have a valid pointer, will be NULL at the end of the list
		{
			// update body position
			body->position.x = GetRandomValue(1, 800);
			body->position.y = GetRandomValue(1, 450);
			// draw body
			DrawCircle(body->position.x, body->position.y, 20, RED);
			body = body->next; // get next body
		}

		EndDrawing();
	}

	while (bodies)
	{
		DestroyBody(bodies);
	}


	CloseWindow();
	return 0;
};
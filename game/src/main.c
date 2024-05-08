#include "raylib.h"
#include "raymath.h"
#include <world.h>

#include "Integrator.h"
#include "body.h"
#include "mathf.h"
#include "Force.h"
#include "render.h"
#include "editor.h"

#include <stdlib.h>
#include <assert.h>

int main(void)
{
	InitWindow(1280, 720, "Pysics Engine");
	InitEditor();

	//initialize world
	ekGravity = (Vector2){ 0, 0 };
	Image ray = LoadImage("C:\\Users\\elija\\Documents\\Neumont School Work\\GAT315\\GAT315-Phyics\\game\\raymond.png");
	Texture rayTex;
	if (IsImageReady(ray) == true)
	{
		rayTex = LoadTextureFromImage(ray);
		rayTex.height = 100;
		rayTex.width = 100;
		UnloadImage(ray);
	}
	while (!WindowShouldClose())
	{
		// update 
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		ekScreenZoom += GetMouseWheelMove() * 0.2f;
		ekScreenZoom = Clamp(ekScreenZoom, 0.1f, 10);

		UpdateEditor(position);

		if (IsMouseButtonPressed(0))
		{
			float angle = GetRandomFloatValue(0, 360);
			for (int i = 0; i < 1; i++)
			{
				ekBody* body = CreateBody();
				body->position = ConvertScreenToWorld(position);
				body->mass = GetRandomFloatValue(ekEditorData.MassMinValue, ekEditorData.MassMaxValue);
				body->inverseMass = 1 / body->mass;
				body->type = BT_DYNAMIC;
				body->damping = 0; // 2.5f
				body->gravityScale = 5;
				body->color = ColorFromHSV(GetRandomFloatValue(0, 360), 1, 1);
				body->shape = 0; //GetRandomValue(0, 2);
				//Vector2 force = Vector2Scale(GetVector2FromAngle((angle * GetRandomFloatValue(-30, 30)) * DEG2RAD), GetRandomFloatValue(1000, 2000));
				//ApplyForce(body, force, FM_IMPULSE);
			}
		}


		//ApplyForce
		ApplyGravitation(ekBodies, ekEditorData.GravitationValue);

		for (ekBody* body = ekBodies; body != NULL; body = body->next)
		{
			Step(body, dt);
		}

		BeginDrawing();
		ClearBackground(BLACK);

		// stats
		DrawText(TextFormat("FPS: %.2f (%.2fms", fps, 1000 / fps), 10, 10, 20, LIME);


		DrawCircle((int)position.x, (int)position.y, 20, RED);
		// draw lines
		for (ekBody* body = ekBodies; body != NULL; body = body->next)
		{
			for (int i = 0; i < 20; i++)
			{
				if (body->prevPositions[i].x != 0 && body->prevPositions[i].y != 0 && body->prevPositions[i - 1].x != 0 && body->prevPositions[i - 1].y != 0)
				{
					if (body->shape == 1)
					{
						DrawLine(body->prevPositions[i].x + 7, body->prevPositions[i].y, body->prevPositions[i - 1].x + 7, body->prevPositions[i - 1].y, body->color);
					}
					else
					{
						DrawLine(body->prevPositions[i].x, body->prevPositions[i].y, body->prevPositions[i - 1].x, body->prevPositions[i - 1].y, body->color);
					}
				}
			}
		}

		// draw bodies
		for (ekBody* body = ekBodies; body != NULL; body = body->next)
		{
			Vector2 screen;
			switch (body->shape)
			{
			case 0:
				screen = ConvertWorldToScreen(body->position);
				DrawCircle(screen.x, screen.y, ConvertWorldToPixel(body->mass), body->color);
				break;
			case 1:
				DrawRectangle(body->position.x, body->position.y, GetRandomValue(10, 20), GetRandomValue(10, 20), body->color);

				break;
			case 2:
				DrawCircleLines(body->position.x, body->position.y, GetRandomValue(2, 10), body->color);
				break;
			case 3:
				if (IsImageReady(ray) == true)
				{
					DrawTexture(rayTex, body->position.x, body->position.y, WHITE);
				}
				break;
			}
			PushBackIndex(body->prevPositions, body->position, 100);
		}
		DrawEditor();
		EndDrawing();
	}

	while (ekBodies)
	{
		DestroyBody(ekBodies);
	}


	CloseWindow();
	return 0;
};
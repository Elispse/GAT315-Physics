#include "raylib.h"
#include "raymath.h"
#include <world.h>

#include "Integrator.h"
#include "body.h"
#include "mathf.h"
#include "Force.h"
#include "render.h"
#include "editor.h"
#include "collision.h"
#include "contact.h"

#include <stdlib.h>
#include <assert.h>
#include <spring.h>

int main(void)
{
	ekBody* selectedBody = NULL;
	ekBody* connectBody = NULL;

	InitWindow(1280, 720, "Pysics Engine");
	InitEditor();
	SetTargetFPS(60);

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

	ekEditorData.Timestep_sliderValue = 1.0f;

	
	double timeAccumulator = 0.0;

	ekEditorData.Simulate_toggleActive = true;
	// game loop
	while (!WindowShouldClose())
	{
		double fixedTimestep = 1 / ekEditorData.Timestep_sliderValue;
		// update 
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		ekGravity = (Vector2){ 0, ekEditorData.GravityScaleValue };

		Vector2 position = GetMousePosition();
		ekScreenZoom += GetMouseWheelMove() * 0.2f;
		ekScreenZoom = Clamp(ekScreenZoom, 0.1f, 10);

		UpdateEditor(position);

		selectedBody = GetBodyIntersect(ekBodies, position);
		if (selectedBody)
		{
			Vector2 screen = ConvertWorldToScreen(selectedBody->position);
			DrawCircleLines(screen.x, screen.y, ConvertWorldToPixel(selectedBody->mass) + 5, YELLOW);
		}

		if (!ekEditorIntersect)
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_CONTROL))
			{
				float angle = GetRandomFloatValue(0, 360);
				for (int i = 0; i < 1; i++)
				{
					ekBody* body = CreateBody(ConvertScreenToWorld(position), ekEditorData.MassValue, ekEditorData.BodyTypeDropActive);
					body->damping = ekEditorData.DampingValue; // 2.5f
					body->gravityScale = 1;// ekEditorData.WorldGravitationSliderValue;
					body->color = WHITE; //ColorFromHSV(GetRandomFloatValue(0, 360), 1, 1);
					body->shape = 0; //GetRandomValue(0, 2);
					body->restitution = ekEditorData.Restitution_sliderValue;

					AddBody(body);
				}
			}

			if (IsKeyDown(KEY_LEFT_ALT))
			{
				if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody) connectBody = selectedBody;
				if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody) DrawLineBodyToPosition(connectBody, position);
				if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
				{
					selectedBody = NULL;
					connectBody = NULL;
				}
				if (connectBody)
				{
					Vector2 world = ConvertScreenToWorld(position);
					if (connectBody->type == BT_STATIC || connectBody->type == BT_KINEMATIC)
					{
						connectBody->position = world;
					}
					else
					{
						ApplySpringForcePosition(world, connectBody, 0, 20, 5);
					}
				}
			}
			else
			{
				// connect springs
				if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody) connectBody = selectedBody;
				if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody) DrawLineBodyToPosition(connectBody, position);
				if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && connectBody)
				{
					if (selectedBody && selectedBody != connectBody)
					{
						ekSpring_t* spring = CreateSpring(connectBody, selectedBody, Vector2Distance(connectBody->position, selectedBody->position), ekEditorData.StiffnessValue);
						AddSpring(spring);
					}
				}
			}
		}
		if (ekEditorData.Simulate_toggleActive)
		{
			timeAccumulator += dt;
			while (timeAccumulator >= fixedTimestep)
			{
				timeAccumulator -= fixedTimestep;

				//ApplyForce
				ApplyGravitation(ekBodies, ekEditorData.GravitySliderValue);
				ApplySpringForce(ekSprings);

				for (ekBody* body = ekBodies; body != NULL; body = body->next)
				{
					Step(body, dt);
				}

				// collisison
				ekContact_t* contacts = NULL;
				CreateContacts(ekBodies, &contacts);
				SeparateContacts(contacts);
				ResolveContacts(contacts);
			}


			//ApplyForce
			ApplyGravitation(ekBodies, ekEditorData.GravitySliderValue);
			ApplySpringForce(ekSprings);

			for (ekBody* body = ekBodies; body != NULL; body = body->next)
			{
				Step(body, dt);
			}
		}

		// collisison
		ekContact_t* contacts = NULL;
		CreateContacts(ekBodies, &contacts);
		SeparateContacts(contacts);
		ResolveContacts(contacts);

		// render
		BeginDrawing();
		ClearBackground(BLACK);

		// stats
		DrawText(TextFormat("FPS: %.2f (%.2fms", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME);


		if (ekEditorIntersect) DrawCircle((int)position.x, (int)position.y, 20, RED);
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
				DrawCircle(screen.x, screen.y, ConvertWorldToPixel(body->mass * 0.5f), body->color);
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
					screen = ConvertWorldToScreen(body->position);
					DrawTexture(rayTex, screen.x, screen.y, WHITE);
				}
				break;
			}
			PushBackIndex(body->prevPositions, body->position, 100);
		}

		// draw Springs
		for (ekSpring_t* spring = ekSprings; spring; spring = spring->next)
		{
			Vector2 screen1 = ConvertWorldToScreen(spring->body1->position);
			Vector2 screen2 = ConvertWorldToScreen(spring->body2->position);
			DrawLine(screen1.x, screen1.y, screen2.x, screen2.y, YELLOW);
		}

		// draw contacts 
		for (ekContact_t* contact = contacts; contact; contact = contact->next)
		{
			Vector2 screen = ConvertWorldToScreen(contact->body1->position);
			DrawCircle(screen.x, screen.y, ConvertWorldToPixel(contact->body1->mass * 0.5f), RED);
		}

		DrawEditor(position);
		if (ekEditorData.Reset_buttonPressed)
		{
			DestroyAllBodies();
			DestroyAllSprings();
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
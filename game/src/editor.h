#pragma once
#include "raylib.h"

typedef struct ekEditorData
{
	float MassMinValue;
	float MassMaxValue;
	float GravitationValue;
} ekEditorData_t;
extern ekEditorData_t ekEditorData;

extern bool ekEditorActive;
extern bool ekEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 mousePosition);
void DrawEditor();
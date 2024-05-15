#pragma once
#include "raylib.h"

typedef struct ekEditorData
{
    bool EditorBoxActive;
    bool WorldBoxActive;
    float WorldGravitationSliderValue;
    float MassMinSliderValue;
    float MassMaxSliderValue;
    bool BodyTypeDropEditMode;
    int BodyTypeDropActive;
    float GravitySliderValue;
    float DampingSliderValue;
} ekEditorData_t;
extern ekEditorData_t ekEditorData;

extern bool ekEditorActive;
extern bool ekEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 position);
void DrawEditor(Vector2 positon);
struct ekBody* GetBodyIntersect(struct ekBody* bodies, Vector2 position); 
void DrawLineBodyToPosition(struct ekBody* body, Vector2 position);
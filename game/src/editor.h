#pragma once
#include "raylib.h"

typedef struct ekEditorData
{
    bool EditorBoxActive;
    bool BodyTypeDropEditMode;
    int BodyTypeDropActive;
    float GravitySliderValue;
    float DampingValue;
    float MassValue;
    float StiffnessValue;
    float GravityScaleValue;
    float WorldGravitationSliderValue;
    bool Simulate_toggleActive;
    bool Reset_buttonPressed;
    float Timestep_sliderValue;
    float Restitution_sliderValue;

} ekEditorData_t;
extern ekEditorData_t ekEditorData;


extern bool ekEditorActive;
extern bool ekEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 position);
void DrawEditor(Vector2 positon);

struct ekBody* GetBodyIntersect(struct ekBody* bodies, Vector2 position); 
void DrawLineBodyToPosition(struct ekBody* body, Vector2 position);
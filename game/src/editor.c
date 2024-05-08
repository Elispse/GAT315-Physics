#include "editor.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

bool ekEditorActive = true;
bool ekEditorIntersect = false;
ekEditorData_t ekEditorData;

Rectangle editorRect;

Vector2 anchor01 = { 950, 50 };

bool EditorBoxActive = true;

void InitEditor()
{
    GuiLoadStyle("raygui/styles/terminal/style_terminal.rgs");

    ekEditorData.GravitationValue = 2;
    ekEditorData.MassMinValue = 0.1f;
    ekEditorData.MassMaxValue = 1;
}

void UpdateEditor(Vector2 position)
{
    //
}

void DrawEditor()
{
    if (EditorBoxActive)
    {
        EditorBoxActive = !GuiWindowBox((Rectangle) { anchor01.x + 0, anchor01.y + 0, 224, 416 }, "Editor");
        GuiSlider((Rectangle) { anchor01.x + 72, anchor01.y + 64, 120, 16 }, "Mass Min", NULL, & ekEditorData.MassMinValue, 0, 10);
        GuiSlider((Rectangle) { anchor01.x + 72, anchor01.y + 88, 120, 16 }, "Mass Max", NULL, & ekEditorData.MassMaxValue, 0, 10);
        GuiSliderBar((Rectangle) { anchor01.x + 72, anchor01.y + 120, 120, 16 }, "Gravitation", NULL, & ekEditorData.GravitationValue, 0, 100);
    }
}
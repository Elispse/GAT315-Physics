#include "editor.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"
#include "render.h"
#include "body.h"

bool ekEditorActive = true;
bool ekEditorIntersect = false;
ekEditorData_t ekEditorData;

Rectangle editorRect;

//Vector2 anchor01 = { 950, 50 };
Vector2 anchor01 = { 920, 50 };
Vector2 anchor02 = { 952, 98 };
Vector2 anchor03 = { 952, 285 };
Texture2D cursorTexture;

bool EditorBoxActive = true;

void InitEditor()
{
    GuiLoadStyle("raygui/styles/terminal/style_terminal.rgs");

    Image image = LoadImage("resources/r.png");
    cursorTexture = LoadTextureFromImage(image);
    cursorTexture.height = 50;
    cursorTexture.width = 50;
    UnloadImage(image);

    ekEditorData.EditorBoxActive = true;
    ekEditorData.BodyTypeDropEditMode = false;
    ekEditorData.BodyTypeDropActive = 0;
    ekEditorData.GravitySliderValue = 2;
    ekEditorData.DampingValue = 0.0f;
    ekEditorData.MassValue = 4.0f;
    ekEditorData.StiffnessValue = 20;
    ekEditorData.GravityScaleValue = 0;
    ekEditorData.WorldGravitationSliderValue = 0.0f;

    editorRect = (Rectangle){ anchor01.x + 0, anchor01.y + 0, 304, 616 };
}

void UpdateEditor(Vector2 position)
{
    if (IsKeyPressed(KEY_TAB)) EditorBoxActive = !EditorBoxActive;

    ekEditorIntersect = EditorBoxActive && CheckCollisionPointRec(position, editorRect);
}

void DrawEditor(Vector2 position)
{
    // toggle show / hide editor box with key press

    if (ekEditorData.BodyTypeDropEditMode) GuiLock();

    if (EditorBoxActive)
    {
        EditorBoxActive = !GuiWindowBox((Rectangle) { anchor01.x + 0, anchor01.y + 0, 344, 368 }, "Editor");
    }
    GuiGroupBox((Rectangle) { anchor02.x + 0, anchor02.y + 0, 280, 168 }, "Elements");
    GuiSliderBar((Rectangle) { anchor02.x + 88, anchor02.y + 88, 120, 16 }, "Gravity Scale", NULL, & ekEditorData.GravitySliderValue, 0, 100);
    GuiSliderBar((Rectangle) { anchor02.x + 88, anchor02.y + 112, 120, 16 }, "Damping", NULL, & ekEditorData.DampingValue, 0, 100);
    GuiGroupBox((Rectangle) { anchor03.x + 0, anchor03.y + 0, 280, 80 }, "World");
    GuiSliderBar((Rectangle) { anchor02.x + 88, anchor02.y + 64, 120, 16 }, "Mass", NULL, & ekEditorData.MassValue, 0, 10);
    GuiSliderBar((Rectangle) { anchor02.x + 88, anchor02.y + 136, 120, 16 }, "Stiffness (k)", NULL, & ekEditorData.StiffnessValue, 0, 100);
    GuiSlider((Rectangle) { anchor03.x + 112, anchor03.y + 16, 120, 16 }, "Gravity", NULL, & ekEditorData.GravityScaleValue, -15, 15);
    GuiSlider((Rectangle) { anchor03.x + 112, anchor03.y + 40, 120, 16 }, "World Gravitation", NULL, & ekEditorData.WorldGravitationSliderValue, 0, 100);
    if (GuiDropdownBox((Rectangle) { anchor02.x + 88, anchor02.y + 24, 120, 24 }, "Dynamic; Static; Kinematic", & ekEditorData.BodyTypeDropActive, ekEditorData.BodyTypeDropEditMode)) ekEditorData.BodyTypeDropEditMode = !ekEditorData.BodyTypeDropEditMode;

    HideCursor();
    DrawTexture(cursorTexture, (int)position.x - (cursorTexture.width / 2 - 3), (int)position.y - (cursorTexture.height / 2 - 20), WHITE);

    GuiUnlock();
}

ekBody* GetBodyIntersect(ekBody* bodies, Vector2 position)
{
    for (ekBody* body = bodies; body; body = body->next)
    {
        Vector2 screen = ConvertWorldToScreen(body->position);
        if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass * 0.5f)))
        {
            return body;
        }
    }

    return NULL;
}

void DrawLineBodyToPosition(ekBody* body, Vector2 position)
{
    Vector2 screen = ConvertWorldToScreen(body->position);
    DrawLine((int)screen.x, (int)screen.y, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, YELLOW);
}
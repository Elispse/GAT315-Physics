#include "editor.h"
#define RAYGUI_IMPLEMENTATION
#define EDITOR_DATA(data) TextFormat("%0.2f", data), &data
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

    bool EditorBoxActive = true;
    bool BodyTypeDropEditMode = false;
    int BodyTypeDropActive = 0;
    float GravitySliderValue = 1.0f;
    float DampingValue = 0.0f;
    float MassValue = 0.5f;
    float StiffnessValue = 0.5f;
    float GravityScaleValue = 0.0f;
    float WorldGravitationSliderValue = 0.0f;
    bool Simulate_toggleActive = true;
    bool Reset_buttonPressed = false;
    float Timestep_sliderValue = 1.0f;
    float Restitution_sliderValue = 0.0f;

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
        EditorBoxActive = !GuiWindowBox((Rectangle) { anchor01.x + 0, anchor01.y + 0, 344, 424 }, "Editor");
        GuiToggle((Rectangle) { anchor01.x + 200, anchor01.y + 376, 88, 24 }, "Simulate", & ekEditorData.Simulate_toggleActive);
        ekEditorData.Reset_buttonPressed = GuiButton((Rectangle) { anchor01.x + 48, anchor01.y + 376, 120, 24 }, "Reset");
    }
    GuiGroupBox((Rectangle) { anchor02.x + 0, anchor02.y + 0, 280, 192 }, "Elements");
    GuiSliderBar((Rectangle) { anchor02.x + 88, anchor02.y + 88, 120, 16 }, "Gravity Scale", NULL, & ekEditorData.GravitySliderValue, 0, 100);
    GuiSliderBar((Rectangle) { anchor02.x + 88, anchor02.y + 112, 120, 16 }, "Damping", NULL, & ekEditorData.DampingValue, 0, 100);
    GuiGroupBox((Rectangle) { 952, 300, 280, 104 }, "World");
    GuiSlider((Rectangle) { anchor02.x + 88, anchor02.y + 64, 120, 16 }, "Mass", NULL, & ekEditorData.MassValue, 0.1, 10);
    GuiSlider((Rectangle) { anchor02.x + 88, anchor02.y + 136, 120, 16 }, "Stiffness (k)", NULL, & ekEditorData.StiffnessValue, 0.1, 100);
    GuiSlider((Rectangle) { anchor03.x + 112, anchor03.y + 24, 120, 16 }, "Gravity", NULL, & ekEditorData.GravityScaleValue, -50, 50);
    GuiSlider((Rectangle) { anchor03.x + 112, anchor03.y + 48, 120, 16 }, "World Gravitation", NULL, & ekEditorData.WorldGravitationSliderValue, 0, 100);
    GuiSliderBar((Rectangle) { anchor03.x + 112, anchor03.y + 72, 120, 16 }, "Timestep", NULL, & ekEditorData.Timestep_sliderValue, 1, 120);
    GuiSliderBar((Rectangle) { anchor02.x + 88, anchor02.y + 160, 120, 16 }, "Restitution", NULL, & ekEditorData.Restitution_sliderValue, 0, 100);
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
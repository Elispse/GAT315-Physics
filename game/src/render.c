#include "render.h"
#include "raymath.h"

// Define screen size and zoom level
Vector2 ekScreenSize = { 1280, 720 };
float ekScreenZoom = 1.0f;
float ekViewSize = 25.0f;

// Function to convert screen coordinates to world coordinates
Vector2 ConvertScreenToWorld(Vector2 screen)
{
    // Calculate the aspect ratio of the screen
    float ratio = ekScreenSize.x / ekScreenSize.y;

    // Calculate the extents of the view based on the aspect ratio and view size
    Vector2 extents = (Vector2){ ekViewSize * ratio, ekViewSize };
    // Scale the extents based on the screen zoom level
    extents = Vector2Scale(extents, ekScreenZoom);

    // Calculate the lower and upper bounds of the view
    Vector2 lower = Vector2Negate(extents);
    Vector2 upper = extents;

    // Normalize screen coordinates to range [0,1]
    float nx = screen.x / ekScreenSize.x;
    float ny = (ekScreenSize.y - screen.y) / ekScreenSize.y;

    // Convert normalized screen coordinates to world coordinates
    Vector2 world;
    world.x = ((1 - nx) * lower.x) + (nx * upper.x);
    world.y = ((1 - ny) * lower.y) + (ny * upper.y);

    return world;
}

// Function to convert world coordinates to screen coordinates
Vector2 ConvertWorldToScreen(Vector2 world)
{
    // Calculate the aspect ratio of the screen
    float ratio = ekScreenSize.x / ekScreenSize.y;

    // Calculate the extents of the view based on the aspect ratio and view size
    Vector2 extents = (Vector2){ ekViewSize * ratio, ekViewSize };
    // Scale the extents based on the screen zoom level
    extents = Vector2Scale(extents, ekScreenZoom);

    // Calculate the lower and upper bounds of the view
    Vector2 lower = Vector2Negate(extents);
    Vector2 upper = extents;

    // Normalize world coordinates to range [0,1] based on view bounds
    float nx = (world.x - lower.x) / (upper.x - lower.x);
    float ny = (world.y - lower.y) / (upper.y - lower.y);

    // Convert normalized world coordinates to screen coordinates
    Vector2 screen;
    screen.x = nx * ekScreenSize.x;
    screen.y = (1.0f - ny) * ekScreenSize.y;

    return screen;
}

// Function to convert world units to pixels
float ConvertWorldToPixel(float world)
{
    // Calculate the aspect ratio of the screen
    float ratio = ekScreenSize.x / ekScreenSize.y;
    // Calculate the extents of the view based on the aspect ratio and view size
    Vector2 extents = (Vector2){ ekViewSize * ratio, ekViewSize };
    // Scale the extents based on the screen zoom level
    extents = Vector2Scale(extents, ekScreenZoom);

    // Normalize world unit to range [0,1] based on view bounds
    float nx = world / extents.x;
    // Convert normalized world unit to pixel
    float pixel = nx * ekScreenSize.x;

    return pixel;
}
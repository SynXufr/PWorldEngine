#include "raylib.h"
#include "raymath.h"
#include "world/NoiseManager.h"

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1600;
    const int screenHeight = 800;
    const int mapSize = 256;

    float frequency = 0.02f;
    int seed = 1337;

    InitWindow(screenWidth, screenHeight, "Generative World Engine");

    NoiseManager noiseManager(mapSize, frequency, seed);

    // Render texture for 3D view
    RenderTexture2D renderTexture3D = LoadRenderTexture(screenWidth / 2, screenHeight);

    // Set up 3D Camera
    Camera3D camera = {0};
    camera.position = (Vector3){40.0f, 30.0f, 40.0f};
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // 3D Model
    Vector3 meshSize = {50.0f, 15, 50.0f};
    noiseManager.UpdateNoise();
    Mesh mesh = GenMeshHeightmap(noiseManager.heightmap, meshSize);
    Model model = LoadModelFromMesh(mesh);

    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(noiseManager.colormap);

    // Main game loop
    while (!WindowShouldClose())
    {
        // --- INPUT ---
        if (IsKeyDown(KEY_UP)) {
            frequency += 0.0005f;
            noiseManager.SetFrequency(frequency);
        }
        if (IsKeyDown(KEY_DOWN)) {
            frequency -= 0.0005f;
            noiseManager.SetFrequency(frequency);
        }
        if (IsKeyDown(KEY_RIGHT)) {
            noiseManager.SetZOffset(noiseManager.GetZOffset() + 1.0f);
        }
        if (IsKeyDown(KEY_LEFT)) {
            noiseManager.SetZOffset(noiseManager.GetZOffset() - 1.0f);
        }
        if (IsKeyDown(KEY_SPACE)) {
            noiseManager.SetSeed(++seed);
        }
        if (IsKeyDown(KEY_W)) camera.position.y += 0.5f;
        if (IsKeyDown(KEY_S)) camera.position.y -= 0.5f;

        if (model.meshCount > 0) {
            UnloadTexture(model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture);
            UnloadModel(model);
        }

        // --- GENERATE NOISE MAP ---
        noiseManager.UpdateNoise();

        // Regenerate model and texture
        Mesh newMesh = GenMeshHeightmap(noiseManager.heightmap, meshSize);
        model = LoadModelFromMesh(newMesh);

        Texture2D currentTex = LoadTextureFromImage(noiseManager.colormap);
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = currentTex;

        UpdateCamera(&camera, CAMERA_ORBITAL);

        // Render 3D view to texture
        BeginTextureMode(renderTexture3D);
            ClearBackground(DARKGRAY);
            BeginMode3D(camera);
                if (model.meshCount > 0) DrawModel(model, (Vector3){-25, 0, -25}, 1.0f, WHITE);
                DrawPlane((Vector3){0, 1.2f, 0}, (Vector2){60, 60}, Fade(BLUE, 0.4f));
                DrawGrid(20, 1.0f);
            EndMode3D();
        EndTextureMode();

        BeginDrawing();
            ClearBackground(DARKGRAY);

            DrawTexturePro(currentTex,
                (Rectangle){0, 0, (float)mapSize, (float)mapSize},
                (Rectangle){0, 0, (float)screenWidth / 2, (float)screenHeight},
                (Vector2){ 0, 0 }, 0.0f, WHITE);

            DrawText("2D Heightmap", 20, screenHeight - 40, 20, YELLOW);
            DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, RAYWHITE);

            DrawTexturePro(renderTexture3D.texture,
                (Rectangle){0, 0, (float)screenWidth / 2, -(float)screenHeight},
                (Rectangle){(float)screenWidth / 2, 0, (float)screenWidth / 2, (float)screenHeight},
                (Vector2){ 0, 0 }, 0.0f, WHITE);

            DrawText("3D World View", screenWidth / 2 + 20, screenHeight - 40, 20, YELLOW);

            DrawRectangle(10, 10, 250, 80, Fade(BLACK, 0.5f));
            DrawText(TextFormat("Frequency: %.4f", noiseManager.GetFrequency()), 20, 20, 20, WHITE);
            DrawText(TextFormat("Z Offset: %.1f", noiseManager.GetZOffset()), 20, 40, 20, WHITE);
        EndDrawing();

        UnloadTexture(currentTex);
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(renderTexture3D);
    UnloadImage(noiseManager.heightmap);
    UnloadImage(noiseManager.colormap);
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}

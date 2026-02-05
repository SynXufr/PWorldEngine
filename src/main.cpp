#include "raylib.h"
#include "FastNoiseLite.h"

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1000;
    const int screenHeight = 1000;

    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;
    float maxDistance = screenWidth / 2.0f;

    InitWindow(screenWidth, screenHeight, "Generative World Engine");

    // Initialize FastNoiseLite
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    float frequency = 0.01f;
    int seed = 1337;

    // Image and texture to store noise
    Image noiseImg = GenImageColor(screenWidth, screenHeight, BLACK);
    Texture2D noiseTex = LoadTextureFromImage(noiseImg);

    // 3D Noise
    float zOffset = 0.0f;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // --- INPUT ---
        if (IsKeyDown(KEY_UP)) frequency += 0.0005f;
        if (IsKeyDown(KEY_DOWN)) frequency -= 0.0005f;
        if (IsKeyDown(KEY_RIGHT)) zOffset += 5.0f;
        if (IsKeyDown(KEY_LEFT)) zOffset -= 5.0f;
        if (IsKeyDown(KEY_SPACE)) noise.SetSeed(++seed);

        // --- GENERATE NOISE MAP ---
        for (int y = 0; y < screenHeight; y++) {
            for (int x = 0; x < screenWidth; x++) {
                noise.SetFrequency(frequency);

                float noiseValue = noise.GetNoise((float)x, (float)y, zOffset);

                float dx = x - centerX;
                float dy = y - centerY;
                float distance = sqrtf(dx*dx + dy*dy) / maxDistance;

                float mask = 1.0f - distance;
                if (mask < 0) mask = 0;

                float islandHeight = noiseValue + (mask * 2.0f - 1.0f);




                unsigned char colorVal = (unsigned char)((islandHeight + 1.0f) * 0.5f * 255);

                // GRAYSCALE
                // Color pixelColor = { colorVal, colorVal, colorVal, 255 };

                // COLOR
                Color pixelColor;
                if (islandHeight < -0.2f) pixelColor = BLUE;
                else if (islandHeight < 0.1f) pixelColor = GOLD;
                else if (islandHeight < 0.5f) pixelColor = LIME;
                else pixelColor = GRAY;

                ImageDrawPixel(&noiseImg, x, y, pixelColor);
            }
        }

        UpdateTexture(noiseTex, noiseImg.data);

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(noiseTex, 0, 0, WHITE);

            DrawRectangle(10, 10, 250, 70, Fade(BLACK, 0.5f));
            DrawText(TextFormat("Frequency: %.4f", frequency), 20, 20, 20, WHITE);
            DrawText(TextFormat("Z Offset: %.1f", zOffset), 20, 40, 20, WHITE);
        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(noiseTex);
    UnloadImage(noiseImg);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
//
// Created by synx on 2/4/26.
//

#include "world/NoiseManager.h"
#include "FastNoiseLite.h"

void NoiseManager::Initialize() {
    heightmap = GenImageColor(mapSize, mapSize, BLACK);
    colormap = GenImageColor(mapSize, mapSize, WHITE);

    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.SetFractalOctaves(FractalOctaves);
    noise.SetFractalLacunarity(FractalLacunarity);
    noise.SetFractalGain(FractalGain);
}

NoiseManager::NoiseManager(const int mapSize) {
    this->mapSize = mapSize;
    Initialize();
}

NoiseManager::NoiseManager(const int mapSize, float freq, int seed) {
    this->mapSize = mapSize;
    this->seed = seed;
    this->frequency = freq;
    Initialize();
}

float NoiseManager::circleGradient(float dist) {
    float tmp = 1.0f - powf(dist, 3.0f);
    return tmp < 0 ? 0 : tmp;
}

Color NoiseManager::LerpColor(Color a, Color b, float factor) {
    return (Color){
        (unsigned char) (a.r + (b.r - a.r) * factor),
        (unsigned char) (a.g + (b.g - a.g) * factor),
        (unsigned char) (a.b + (b.b - a.b) * factor),
        255
    };
}

Color NoiseManager::computeColor(float height) {
    Color pixelColor;

    if (height < 0.1f) {
        pixelColor = water;
    } else if (height < 0.15f) {
        // Smooth blend from Water to Sand over the 0.1 -> 0.15 range
        float t = (height - 0.1f) / 0.05f;
        pixelColor = LerpColor(water, sand, t);
    } else if (height < 0.25f) {
        pixelColor = sand;
    } else if (height < 0.35f) {
        // Smooth blend from Sand to Grass
        float t = (height - 0.25f) / 0.1f;
        pixelColor = LerpColor(sand, grass, t);
    } else if (height < 0.6f) {
        pixelColor = grass;
    } else if (height < 0.8f) {
        // Smooth blend from Grass to Rock
        float t = (height - 0.6f) / 0.2f;
        pixelColor = LerpColor(grass, rock, t);
    } else {
        // Smooth blend from Rock to Snow
        float t = fminf((height - 0.8f) / 0.2f, 1.0f);
        pixelColor = LerpColor(rock, snow, t);
    }

    return pixelColor;
}

void NoiseManager::UpdateNoise() {
    for (int y = 0; y < mapSize; y++) {
        for (int x = 0; x < mapSize; x++) {
            noise.SetFrequency(frequency);

            float noiseValue = (noise.GetNoise((float) x, (float) y, zOffset) + 1.0f) * 0.5f;

            float dx = (float) x - centerX;
            float dy = (float) y - centerY;
            float dist = sqrtf(dx * dx + dy * dy) / maxRadius;

            float heightMask = circleGradient(dist);

            float finalHeight = (noiseValue * heightMask) * heightMult;
            if (finalHeight > 1.0f) finalHeight = 1.0f;

            Color pixelColor = computeColor(finalHeight);

            unsigned char gray = (unsigned char)(finalHeight * 255);

            ImageDrawPixel(&heightmap, x, y, (Color){gray, gray, gray, 255});

            ImageDrawPixel(&colormap, x, y, pixelColor);
        }
    }
}

void NoiseManager::SetFrequency(float freq) {
    frequency = freq;
}

void NoiseManager::SetSeed(int seed) {
    this->seed = seed;
    noise.SetSeed(seed);
}

void NoiseManager::SetZOffset(float offset) {
    zOffset = offset;
}

float NoiseManager::GetFrequency() const {
    return frequency;
}

float NoiseManager::GetZOffset() const {
    return zOffset;
}

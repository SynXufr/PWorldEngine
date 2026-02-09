//
// Created by synx on 2/4/26.
//

#ifndef PWORLDENGINE_NOISEMANAGER_H
#define PWORLDENGINE_NOISEMANAGER_H
#include "FastNoiseLite.h"
#include "raylib.h"

class NoiseManager {
    const int FractalOctaves = 5;
    const float FractalLacunarity = 2.0f;
    const float FractalGain = 0.5f;
    const float heightMult = 1.2f;

    const Color water = { 34, 126, 196, 255 };
    const Color sand  = { 240, 225, 150, 255 };
    const Color grass = { 70, 170, 70, 255 };
    const Color rock  = { 120, 110, 100, 255 };
    const Color snow  = WHITE;

    FastNoiseLite noise;
    float frequency = 0.02f;
    int seed = 1337;
    float zOffset = 0.0f;
    float circleGradient(float dist);
    int mapSize = 256;

    float centerX = (float)mapSize / 2.0f;
    float centerY = (float)mapSize / 2.0f;
    float maxRadius = (float)mapSize / 2.0f;

    Color computeColor(float height);
    Color LerpColor(Color a, Color b, float factor);
    void Initialize();

public:
    Image heightmap{};
    Image colormap{};

    void UpdateNoise();
    void SetFrequency(float freq);
    void SetSeed(int seed);
    void SetZOffset(float offset);
    float GetFrequency() const;
    float GetZOffset() const;
    NoiseManager(int mapSize);
    NoiseManager(int mapSize, float freq, int seed);
};

#endif //PWORLDENGINE_NOISEMANAGER_H
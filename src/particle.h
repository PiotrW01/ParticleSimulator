#pragma once
#include "vec2.hpp"
#include "vec4.hpp"
#include "colors.h"

enum class ParticleType : uint8_t
{
    Water,
    Sand,
    Smoke
};

class Particle
{
public:
    Particle(ParticleType type);
    ParticleType type;
    float lifeTime = -1.0f;
    glm::vec2 velocity = {0, 0};
    glm::vec2 position = {0, 0};
    bool wasUpdated = false;
};

class ParticleInfo
{
public:
    float density;
    float viscosity;
    float lifeTime;
    Color color;
    static ParticleInfo get(ParticleType type);
};
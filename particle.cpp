#include "particle.h"

Particle::Particle(ParticleType type)
{
    this->type = type;
}

ParticleInfo ParticleInfo::get(ParticleType type)
{
    switch (type)
    {
        // Density | viscosity | lifeTime | Color
    case ParticleType::Water:
        return {1.0f, 1.0f, -1.0f, Colors::BLUE};
        break;
    case ParticleType::Sand:
        return {2.0f, 1.0f, -1.0f, Colors::SAND};
        break;
    default:
        return {1.0f, 1.0f, -1.0f, Colors::BLACK};
        break;
    }
    return {1.0f, 1.0f, -1.0f, Colors::BLACK};
}
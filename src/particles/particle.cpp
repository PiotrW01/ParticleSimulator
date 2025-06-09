#include "particle.h"

Particle::Particle(ParticleType type)
{
    this->type = type;
}

ParticleInfo ParticleInfo::get(ParticleType type)
{
    static const ParticleInfo water   = { 1.0f, 1.0f, -1.0f, false, Colors::BLUE  };
    static const ParticleInfo sand    = { 2.0f, 1.0f, -1.0f, false, Colors::SAND  };
    static const ParticleInfo smoke   = { 0.1f, 1.0f, -1.0f, false, Colors::GRAY  };
    static const ParticleInfo unknown = { 1.0f, 1.0f, -1.0f, false, Colors::BLACK };
    static const ParticleInfo stone   = { 1.0f, 1.0f, -1.0f, true,  Colors::RED   };

    switch (type)
    {
    case ParticleType::Water: return water;
    case ParticleType::Sand:  return sand;
    case ParticleType::Smoke: return smoke;
	case ParticleType::Stone: return stone;
    default: return unknown;
    }
}
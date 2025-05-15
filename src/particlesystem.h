#pragma once
#include <vector>
#include <memory>
#include "particle.h"
#include "shaders.h"
class Camera2D;

enum class CellStatus
{
    INVALID,
    EMPTY,
    OCCUPIED
};

class ParticleSystem
{
public:
    static void init();
    static void spawnParticle(const glm::vec2 &spawnPos);
    static void spawnParticle(int x, int y, ParticleType type = ParticleType::Sand);
    static void spawnParticles(const glm::vec2 &spawnPos, ParticleType type = ParticleType::Sand, int radius = 1);
    static void update();
    static void render(Camera2D &camera);
    static void resize(int size);
    static unsigned int particleCount;

private:
    static std::vector<std::vector<std::shared_ptr<Particle>>> grid;
    static std::vector<std::shared_ptr<Particle>> particles;
    static std::vector<VertexPosColor> vertices;
    static unsigned int VAO, VBO;
    static unsigned int shaderProgram;
    static bool isCellEmpty(int x, int y);
    static CellStatus getCellStatus(int x, int y);
};

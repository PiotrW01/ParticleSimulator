#pragma once
#include <vector>
#include <memory>
#include "vec2.hpp"
#include "particle.h"
#include "graphics/shaders.h"
#include "graphics/texture.h"
class Camera2D;

enum class CellStatus
{
    INVALID,
    EMPTY,
    OCCUPIED,
};

class ParticleSystem
{
public:
    void init();
    void spawnParticle(int x, int y, ParticleType type = ParticleType::Sand);
    void spawnParticles(const glm::vec2 &spawnPos, ParticleType type = ParticleType::Sand, int radius = 1);
    void update();
    void render(Camera2D &camera);
    void resize(int size);
public:
    unsigned int particleCount = 0;
private:
    std::vector<glm::ivec2> getNextCells(int j, int i, ParticleType type);
    CellStatus getCellStatus(int x, int y);
private:
    int width = 600, height = 400;
    Texture texture = { 0.0f, 0.0f };
    std::vector<std::vector<std::shared_ptr<Particle>>> grid;
    std::vector<std::shared_ptr<Particle>> particles;
    std::vector<VertexPosColor> vertices;
    unsigned int VAO = 0, VBO = 0;
    unsigned int shaderProgram = 0;
};

#include "glad.h"
#include "particlesystem.h"
#include "utils/globals.h"
#include "graphics/camera2d.h"
#include <cstdlib>
#include <ctime>

void ParticleSystem::init()
{
    texture = {0.0f, 0.0f};
    texture.load("assets/white.png");
    texture.setPosition(0, 0);
    texture.setSize(width, height);
    // WINDOW_HEIGHT
    grid.resize(height);
    for (int i = 0; i < grid.size(); i++)
    {
        grid[i].resize(width);
    }
    particles.reserve(height * width);
    std::srand(std::time(0));
	shaderProgram = shaders::compileShaderProgram(shaders::solidColorVertexShader, shaders::solidColorFragmentShader);

    // Generate the VAO (Vertex Array Object), VBO (Vertex Buffer Object)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the VAO and VBO
    glBindVertexArray(VAO);

    // Bind the VBO and load data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPosColor) * width * height, nullptr, GL_DYNAMIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPosColor) * vertices.size(), nullptr, GL_DYNAMIC_DRAW);

    // Set the vertex attribute pointers
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPosColor), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPosColor), (void *)offsetof(VertexPosColor, color));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ParticleSystem::update()
{
    vertices.clear();

    for (auto &particle : particles)
    {
        particle->wasUpdated = false;
    }

    for (int i = grid.size() - 1; i >= 0; i--)
    {
        for (int j = grid[0].size() - 1; j >= 0; j--)
        {
            if (grid[i][j])
            {
                int row = i, col = j;
                ParticleInfo currentParticle = ParticleInfo::get(grid[i][j]->type);

                ParticleInfo neighborParticle = currentParticle;
                if (grid[i][j]->wasUpdated || currentParticle.isSolid)
                {
                    vertices.push_back(VertexPosColor({glm::vec2(col, row), currentParticle.color}));
                    continue;
                }
                bool swapped = false;

                auto offsets = getNextCells(i, j, grid[i][j]->type);
                for (auto &offset : offsets)
                {
                    glm::ivec2 cell = glm::ivec2(i, j) + offset;
                    CellStatus status = getCellStatus(cell.x, cell.y);
                    if (status == CellStatus::INVALID)
                        continue;
                    if (status == CellStatus::OCCUPIED)
                    {
                        neighborParticle = ParticleInfo::get(grid[cell.x][cell.y]->type);
						if (neighborParticle.isSolid)
							continue;
                        if (currentParticle.density > neighborParticle.density)
                        {
                            if (!grid[cell.x][cell.y]->wasUpdated)
                            {
                                swapped = true;
                                row = cell.x;
                                col = cell.y;
                                break;
                            }
                        }
                    }
                    else
                    {
                        row = cell.x;
                        col = cell.y;
                        break;
                    }
                }

                if (swapped)
                {
                    std::swap(grid[i][j], grid[row][col]);
                    grid[i][j]->wasUpdated = true;
                    grid[row][col]->wasUpdated = true;
                    vertices.push_back(VertexPosColor({glm::vec2(col, row) + 0.5f, currentParticle.color}));
                    vertices.push_back(VertexPosColor({glm::vec2(j, i) + 0.5f, neighborParticle.color}));
                    continue;
                }

                if (row != i || col != j)
                {
                    grid[row][col] = std::move(grid[i][j]);
                    grid[row][col]->wasUpdated = true;
                }
                vertices.push_back(VertexPosColor({glm::vec2(col, row) + 0.5f, currentParticle.color}));
            }
        }
    }
}

void ParticleSystem::render(Camera2D &camera)
{
    auto cMatrix = camera.getCombinedMatrix();
    texture.draw(camera);

    unsigned int cLoc = glGetUniformLocation(shaderProgram, "cMatrix");
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(cLoc, 1, GL_FALSE, &cMatrix[0][0]);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(VertexPosColor), vertices.data());
    glPointSize(camera.zoom);

    glDrawArrays(GL_POINTS, 0, vertices.size());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ParticleSystem::spawnParticle(int x, int y, ParticleType type)
{

    if (y < grid.size() && x < grid[0].size())
    {
        if (!grid[y][x])
        {
            auto particle = std::make_shared<Particle>(type);
            particle->position = glm::vec2(y, x);
            grid[y][x] = particle;
            particles.push_back(particle);
            particleCount++;
        }
    }
}

void ParticleSystem::spawnParticles(const glm::vec2 &spawnPos, ParticleType type, int radius)
{
    int rSquared = radius * radius;

    for (int y = spawnPos.y - radius; y <= spawnPos.y + radius; ++y)
    {
        for (int x = spawnPos.x - radius; x <= spawnPos.x + radius; ++x)
        {
            int dx = x - spawnPos.x;
            int dy = y - spawnPos.y;
            if (dx * dx + dy * dy <= rSquared)
            {
                spawnParticle(x, y, type);
            }
        }
    }
}

CellStatus ParticleSystem::getCellStatus(int row, int col)
{
    if (row >= height || col >= width || row < 0 || col < 0)
        return CellStatus::INVALID;
    if (!grid[row][col])
        return CellStatus::EMPTY;
    return CellStatus::OCCUPIED;
}

std::vector<glm::ivec2> ParticleSystem::getNextCells(int i, int j, ParticleType type)
{

    std::vector<glm::ivec2> offsets;
    switch (type)
    {
    case ParticleType::Sand:
        offsets.push_back({1, 0});
        for (int i = 1; i < rand() % 2 + 1; i++)
        {
            offsets.push_back({1, i});
            offsets.push_back({1, -i});
        }
        break;
    case ParticleType::Water:
        offsets.push_back({1, 0});
        for (int i = 1; i < rand() % 6 + 2; i++)
        {
            offsets.push_back({1, i});
            offsets.push_back({1, -i});
        }
        break;
    case ParticleType::Smoke:
        offsets.push_back({-1, 0});
        for (int i = 1; i < rand() % 8 + 2; i++)
        {
            offsets.push_back({-1, i});
            offsets.push_back({-1, -i});
        }
        break;
    default:
        break;
    }
    return offsets;
}
#include "glad.h"
#include <GLFW/glfw3.h>
#include "particlesystem.h"
#include "logger.h"
#include "globals.h"
#include <cstdlib>
#include <ctime>
#include "camera2d.h"

std::vector<std::vector<std::shared_ptr<Particle>>> ParticleSystem::grid;
std::vector<std::shared_ptr<Particle>> ParticleSystem::particles;
std::vector<VertexPosColor> ParticleSystem::vertices;
unsigned int ParticleSystem::VAO, ParticleSystem::VBO;
unsigned int ParticleSystem::shaderProgram;
unsigned int ParticleSystem::particleCount = 0;
int ParticleSystem::width = 300;
int ParticleSystem::height = 100;
Texture ParticleSystem::texture = { 0.0f, 0.0f };

void ParticleSystem::init()
{
    texture.load("assets/white.png");
    texture.setPosition(0, 0);
    texture.setSize(width, height);
    //WINDOW_HEIGHT
    grid.resize(height);
    for (int i = 0; i < grid.size(); i++)
    {
        grid[i].resize(width);
    }
    particles.reserve(height * width);
    std::srand(std::time(0));

    // Create and compile the vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shaders::solidColorVertexShader, nullptr);
    glCompileShader(vertexShader);
    // Check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        LOG_ERROR << infoLog;
    }

    // Create and compile the fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shaders::solidColorFragmentShader, nullptr);
    glCompileShader(fragmentShader);

    // Check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        LOG_ERROR << infoLog;
    }

    // Link the shaders into a shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for program linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        LOG_ERROR << infoLog;
    }

    // Delete the shaders as they're linked into the program now
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Generate the VAO (Vertex Array Object), VBO (Vertex Buffer Object)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the VAO and VBO
    glBindVertexArray(VAO);

    // Bind the VBO and load data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPosColor) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPosColor), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPosColor), (void *)offsetof(VertexPosColor, color));
    glEnableVertexAttribArray(1);

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

std::vector<glm::ivec2> ParticleSystem::getNextCells(int i, int j, ParticleType type)
{

    std::vector<glm::ivec2> offsets;
    switch (type)
    {
    case ParticleType::Sand:
        offsets.push_back({ -1,0 });
        for (int i = 1; i < rand() % 2 + 1; i++)
        {
            offsets.push_back({ -1,i });
            offsets.push_back({ -1,-i });
        }
        break;
    case ParticleType::Water:
        offsets.push_back({ -1,0 });
        for (int i = 1; i < rand() % 6 + 2; i++)
        {
            offsets.push_back({ -1,i });
            offsets.push_back({ -1,-i });
        }
        break;
    case ParticleType::Smoke:
        offsets.push_back({ 1,0 });
        for (int i = 1; i < rand() % 8 + 2; i++)
        {
            offsets.push_back({ 1,i });
            offsets.push_back({ 1,-i });
        }
        break;
    default:
        break;
    }
    return offsets;
}

void ParticleSystem::update()
{
    vertices.clear();

    for (auto &particle : particles)
    {
        particle->wasUpdated = false;
    }

    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = grid[0].size() - 1; j >= 0; j--)
        {
            if (grid[i][j])
            {
                int row = i, col = j;
                ParticleInfo currentParticle = ParticleInfo::get(grid[i][j]->type);
                ParticleInfo neighborParticle = currentParticle;
                if (grid[i][j]->wasUpdated)
                {
                    vertices.push_back(VertexPosColor({glm::vec2(col, row), currentParticle.color}));
                    continue;
                }
                bool swapped = false;


                auto offsets = getNextCells(i, j, grid[i][j]->type);
                for (auto& offset : offsets)
                {
                    glm::ivec2 cell = glm::ivec2(i, j) + offset;
                    CellStatus status = getCellStatus(cell.x, cell.y);
                    if (status == CellStatus::INVALID) continue;
                    if (status == CellStatus::OCCUPIED)
                    {
                        neighborParticle = ParticleInfo::get(grid[cell.x][cell.y]->type);
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

CellStatus ParticleSystem::getCellStatus(int row, int col)
{
    if (row >= height || col >= width || row < 0 || col < 0)
        return CellStatus::INVALID;
    if (!grid[row][col])
        return CellStatus::EMPTY;
    return CellStatus::OCCUPIED;
}

void ParticleSystem::render(Camera2D &camera)
{
    auto cMatrix = camera.getCombinedMatrix();
    texture.draw(camera);

    unsigned int cLoc = glGetUniformLocation(shaderProgram, "cMatrix");

    if (cLoc == -1)
    {
        LOG_ERROR << "Could not get uniform";
        return;
    }
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(cLoc, 1, GL_FALSE, &cMatrix[0][0]);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexPosColor), vertices.data(), GL_STATIC_DRAW);
    glPointSize(camera.zoom);

    glDrawArrays(GL_POINTS, 0, vertices.size());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ParticleSystem::resize(int size)
{
    grid.resize(size);
}

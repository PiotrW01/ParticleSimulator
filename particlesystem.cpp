#include "particlesystem.h"
#include "glad.h"
#include <GLFW/glfw3.h>
#include "logger.h"
#include "globals.h"
#include <cstdlib>
#include <ctime>
#include "camera2d.h"

std::vector<std::vector<std::shared_ptr<Particle>>> ParticleSystem::grid;
std::vector<std::shared_ptr<Particle>> ParticleSystem::particles;
std::vector<Vertex> ParticleSystem::vertices;
unsigned int ParticleSystem::VAO, ParticleSystem::VBO;
unsigned int ParticleSystem::shaderProgram;
unsigned int ParticleSystem::particleCount = 0;

void ParticleSystem::init()
{
    grid.resize(WINDOW_HEIGHT);
    for (int i = 0; i < grid.size(); i++)
    {
        grid[i].resize(WINDOW_WIDTH);
    }
    particles.reserve(WINDOW_HEIGHT * WINDOW_WIDTH);
    std::srand(std::time(0));

    const char *vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec4 aColor;

        uniform mat4 cMatrix;

        out vec4 vColor;

        void main()
        {
            //vec2 flippedPos = vec2(aPos.x, windowSize.y - aPos.y);
            //vec2 ndcPos = (flippedPos / windowSize) * 2.0 - 1.0;
            //gl_Position = vec4(ndcPos, 0.0, 1.0);
            gl_Position = cMatrix * vec4(aPos, 0.0, 1.0);
            vColor = aColor;
        }
    )";

    const char *fragmentShaderSource = R"(
        #version 330 core
        in vec4 vColor;
        out vec4 FragColor;
        void main()
        {
            FragColor = vColor;
        }
    )";

    // Create and compile the vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    // Check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        Logger::error(infoLog);
    }

    // Create and compile the fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        Logger::error(infoLog);
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
        Logger::error(infoLog);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));
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
            grid[y][x] = std::make_shared<Particle>(type);
            particles.push_back(std::shared_ptr(grid[y][x]));
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
                int col = j, row = i;
                ParticleInfo currentParticle = ParticleInfo::get(grid[i][j]->type);
                ParticleInfo neighborParticle = ParticleInfo::get(grid[i][j]->type);
                if (grid[i][j]->wasUpdated)
                {
                    vertices.push_back(Vertex({glm::vec2(j, i), currentParticle.color}));
                    continue;
                }

                bool swapped = false;
                bool moved = false;

                CellStatus fbelow = getCellStatus(j, i - 1);
                CellStatus fbelowleft = getCellStatus(j - 1, i - 1);
                CellStatus fbelowright = getCellStatus(j + 1, i - 1);
                CellStatus fabove = getCellStatus(j, i + 1);

                // BELOW
                if (fbelow != CellStatus::INVALID)
                {
                    if (fbelow == CellStatus::OCCUPIED)
                    {
                        neighborParticle = ParticleInfo::get(grid[i - 1][j]->type);
                        if (currentParticle.density > neighborParticle.density)
                        {
                            if (!grid[i - 1][j]->wasUpdated)
                            {
                                row--;
                                swapped = true;
                            }
                        }
                        else
                        {
                            if (fbelowleft != CellStatus::INVALID)
                            {
                                if (fbelowleft == CellStatus::OCCUPIED)
                                {
                                    neighborParticle = ParticleInfo::get(grid[i - 1][j - 1]->type);
                                    if (currentParticle.density > neighborParticle.density)
                                    {
                                        if (!grid[i - 1][j - 1]->wasUpdated)
                                        {
                                            row--;
                                            col--;
                                            swapped = true;
                                        }
                                    }
                                    else
                                    {
                                        if (fbelowright != CellStatus::INVALID)
                                        {
                                            if (fbelowright == CellStatus::OCCUPIED)
                                            {
                                                neighborParticle = ParticleInfo::get(grid[i - 1][j + 1]->type);
                                                if (currentParticle.density > neighborParticle.density)
                                                {
                                                    if (!grid[i - 1][j + 1]->wasUpdated)
                                                    {
                                                        row--;
                                                        col++;
                                                        swapped = true;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                row--;
                                                col++;
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    row--;
                                    col--;
                                }
                            }
                        }
                    }
                    else
                    {
                        row--;
                    }
                }

                if (swapped)
                {
                    std::swap(grid[i][j], grid[row][col]);
                    grid[i][j]->wasUpdated = true;
                    grid[row][col]->wasUpdated = true;
                    vertices.push_back(Vertex({glm::vec2(col, row), currentParticle.color}));
                    vertices.push_back(Vertex({glm::vec2(j, i), neighborParticle.color}));
                    continue;
                }

                if (row != i || col != j)
                {
                    grid[row][col] = std::move(grid[i][j]);
                    grid[row][col]->wasUpdated = true;
                }
                vertices.push_back(Vertex({glm::vec2(col, row), currentParticle.color}));
            }
        }
    }
}

CellStatus ParticleSystem::getCellStatus(int x, int y)
{
    if (y >= grid.size() || x >= grid[0].size())
        return CellStatus::INVALID;
    if (!grid[y][x])
        return CellStatus::EMPTY;
    return CellStatus::OCCUPIED;
}

void ParticleSystem::render(Camera2D &camera)
{
    auto cMatrix = camera.getCombinedMatrix();
    unsigned int cLoc = glGetUniformLocation(shaderProgram, "cMatrix");

    if (cLoc == -1)
    {
        Logger::error("Could not get uniform");
        return;
    }
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(cLoc, 1, GL_FALSE, &cMatrix[0][0]);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glPointSize(camera.zoom);

    glDrawArrays(GL_POINTS, 0, vertices.size());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ParticleSystem::resize(int size)
{
    grid.resize(size);
}

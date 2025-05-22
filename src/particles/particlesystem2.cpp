#include "particlesystem2.h"
#include "utils/logger.h"
#include "graphics/camera2d.h"
#include "glad.h"

void ParticleSystem2::step() {
    applyExternalForces();
    predictPositions();
    findNeighbors();

    for (int i = 0; i < solverIterations; ++i) {
        computeLambdas();
        computePositionDeltas();
        applyPositionCorrections();
    }

    updateVelocities();
    updatePositions();

    vertices.clear();
    for (auto& particle : particles)
    {
        VertexPosColor v = { particle.position, Colors::GREEN };
        vertices.push_back(v);
    }
}

void ParticleSystem2::applyExternalForces() {
    glm::vec2 gravity(0, -9.81f);
    for (auto& p : particles) {
        p.velocity += gravity * timeStep;
    }
}

void ParticleSystem2::predictPositions() {
    for (auto& p : particles) {
        p.predictedPosition = p.position + p.velocity * timeStep;
    }
}

void ParticleSystem2::findNeighbors() {
    for (size_t i = 0; i < particles.size(); ++i) {
        particles[i].neighbors.clear();
        for (size_t j = 0; j < particles.size(); ++j) {
            if (i == j) continue;
            if (glm::length2(particles[i].predictedPosition - particles[j].predictedPosition) < h2) {
                particles[i].neighbors.push_back(j);
            }
        }
    }
}

float ParticleSystem2::computeDensity(int i) {
    float density = 0.0f;
    for (int j : particles[i].neighbors) {
        float r2 = glm::length2(particles[i].predictedPosition - particles[j].predictedPosition);
        density += mass * poly6Kernel(r2);
    }
    return density;
}

void ParticleSystem2::computeLambdas() {
    for (size_t i = 0; i < particles.size(); ++i) {
        float density = computeDensity(i);
        float constraint = density / restDensity - 1.0f;

        float sumGradC2 = 0.0f;
        for (int j : particles[i].neighbors) {
            glm::vec2 grad = spikyGrad(particles[i].predictedPosition - particles[j].predictedPosition);
            sumGradC2 += glm::dot(grad, grad);
        }
        sumGradC2 += eps;

        particles[i].lambda = -constraint / sumGradC2;
    }
}

void ParticleSystem2::computePositionDeltas() {
    for (size_t i = 0; i < particles.size(); ++i) {
        glm::vec2 delta(0);
        for (int j : particles[i].neighbors) {
            glm::vec2 grad = spikyGrad(particles[i].predictedPosition - particles[j].predictedPosition);
            delta += (particles[i].lambda + particles[j].lambda) * grad;
        }
        particles[i].deltaP = delta * (mass / restDensity);
    }
}

void ParticleSystem2::applyPositionCorrections() {
    for (auto& p : particles) {
        p.predictedPosition += p.deltaP;
    }
}

void ParticleSystem2::updateVelocities() {
    for (auto& p : particles) {
        p.velocity = (p.predictedPosition - p.position) / timeStep;
    }
}

void ParticleSystem2::updatePositions() {
    for (auto& p : particles) {
        p.position = p.predictedPosition;
        LOG_INFO << p.position.x << " " << p.position.y;
    }
}

float ParticleSystem2::poly6Kernel(float r2) {
    if (r2 >= h2) return 0.0f;
    float term = h2 - r2;
    return 315.0f / (64.0f * PI * pow(h, 9)) * term * term * term;
}

glm::vec2 ParticleSystem2::spikyGrad(const glm::vec2& rij) {
    float r = glm::length(rij);
    if (r < eps || r >= h) return glm::vec2(0);
    float coeff = static_cast<float>((-45.0f / (PI * pow(h, 6.0f))) * (h - r) * (h - r));
    return coeff * (rij / r);
}


void ParticleSystem2::render(Camera2D& camera)
{
    auto cMatrix = camera.getCombinedMatrix();
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

void ParticleSystem2::spawnParticles(glm::vec2& position)
{
    for (int x = -10; x < 10; x+=2)
    {
        for (int y = -10; y < 10; y+=2)
        {
            glm::vec2 v = { position.x + x, position.y + y };
            spawnParticle(v);
        }
    }
}

void ParticleSystem2::spawnParticle(glm::vec2& position)
{
    Particle p;
    p.position = position;
    particles.push_back(p);
}

void ParticleSystem2::init()
{
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPosColor), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPosColor), (void*)offsetof(VertexPosColor, color));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
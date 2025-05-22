#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include "graphics/shaders.h"
#include <glm.hpp>
#include <gtx/norm.hpp>
#include <cmath>
#include <unordered_map>
class Camera2D;

constexpr float restDensity = 1000.0f;
constexpr float h = 16.0f; // Kernel radius
constexpr float h2 = h * h;
constexpr float eps = 1e-5f;
constexpr float mass = 1.0f;
constexpr float timeStep = 0.016f;
constexpr int solverIterations = 4;

#define PI 3.14159265358979323846

struct Particle {
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 predictedPosition;
    glm::vec2 deltaP;
    float lambda = 0.0f;
    std::vector<int> neighbors;
};

class ParticleSystem2 {
public:
    std::vector<Particle> particles;
    std::vector<VertexPosColor> vertices;
    void step();
    void init();
    void spawnParticle(glm::vec2& position);
    void spawnParticles(glm::vec2& position);
    void render(Camera2D& camera);

private:
    unsigned int VAO, VBO;
    unsigned int shaderProgram;
    void applyExternalForces();
    void predictPositions();
    void findNeighbors();
    void computeLambdas();
    void computePositionDeltas();
    void applyPositionCorrections();
    void updateVelocities();
    void updatePositions();

    float poly6Kernel(float r2);
    glm::vec2 spikyGrad(const glm::vec2& rij);
    float computeDensity(int i);
};

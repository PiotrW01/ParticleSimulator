#include "glad.h"
#include "utils/globals.h"
#include "utils/logger.h"
#include "gtc/matrix_transform.hpp"
#include "glm.hpp"
#include "texture.h"
#include "textureloader.h"
#include "shaders.h"
#include "camera2d.h"

Texture::~Texture()
{
}

Texture::Texture(float posX, float posY)
{
    this->posX = posX;
    this->posY = posY;
}

void Texture::move(float x, float y)
{
    this->posX += x;
    this->posY += y;
}

void Texture::setSize(float width, float height)
{
    this->width = width;
    this->height = height;
}

void Texture::setPosition(float x, float y)
{
    this->posX = x;
    this->posY = y;
}

void Texture::load(const char *filename)
{
    int x, y, n;
    auto *data = TextureLoader::loadTexture(filename, &x, &y, &n);

    if (data)
    {
        glGenTextures(1, &texPtr);
        glBindTexture(GL_TEXTURE_2D, texPtr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format = n == 4 ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, x, y, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        width = x;
        height = y;

        prepare();
    }
    else
    {
        LOG_WARN << "Failed to load texture";
    }
    TextureLoader::free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::draw(const Camera2D &camera)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, 0.0f));
    model = glm::scale(model, glm::vec3(width, height, 1.0f));
    auto cMatrix = camera.getCombinedMatrix() * model;
    unsigned int cLoc = glGetUniformLocation(shaderProgram, "cMatrix");

    if (cLoc == -1)
    {
        LOG_ERROR << "Could not get matrix";
        return;
    }

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(cLoc, 1, GL_FALSE, &cMatrix[0][0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texPtr);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

    glBindVertexArray(VAO);           
    glDrawArrays(GL_TRIANGLES, 0, 6); 
    glBindVertexArray(0);             
}

void Texture::prepare()
{


    // Create and compile the vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shaders::textureVertexShader, nullptr);
    glCompileShader(vertexShader);
    // Check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        LOG_ERROR << "Texture preparation" << infoLog;
    }

    // Create and compile the fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shaders::textureFragmentShader, nullptr);
    glCompileShader(fragmentShader);

    // Check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        LOG_ERROR << "Texture preparation" << infoLog;
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
        LOG_ERROR << "Texture preparation" << infoLog;
    }

    // Delete the shaders as they're linked into the program now
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Generate the VAO (Vertex Array Object), VBO (Vertex Buffer Object)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // top left
    VertexPosTexture v0 = { {0.0f, 0.0f}, {0.0f, 1.0f} };

    // bottom left
    VertexPosTexture v1 = { {0.0f, 1.0f}, {0.0f, 0.0f} };

    // bottom right
    VertexPosTexture v2 = { {1.0f, 1.0f}, {1.0f, 0.0f} };

    // top right
    VertexPosTexture v3 = { {1.0f, 0.0f}, {1.0f, 1.0f} };

    //top left, bottom left, bottom right
    //top right, top left, bottom right
    vertices = {
        v0, v1, v2, v3, v0, v2
    };

    // Bind the VAO and VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPosTexture) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPosTexture), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPosTexture), (void*)offsetof(VertexPosTexture, texCoord));
    glEnableVertexAttribArray(1);

    // Unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
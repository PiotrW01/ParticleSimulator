#include "glad.h"
#include <GLFW/glfw3.h>
#include "texture.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "const.h"
#include "globals.h"
#include "textureloader.h"
#include "logger.h"

Texture::Texture()
{
    prepare();
}

Texture::~Texture()
{
}

Texture::Texture(int posX, int posY, int width, int height)
{
    this->posX = posX;
    this->posY = posY;
    this->width = width;
    this->height = height;
    prepare();
}

void Texture::move(float x, float y)
{
    this->posX += x;
    this->posY += y;
    updatePosition();
}

void Texture::setPosition(float x, float y)
{
    this->posX = x;
    this->posY = y;
    updatePosition();
}

bool Texture::contains(float x, float y)
{
    return (x >= this->posX && x <= this->posX + this->width) &&
           (y >= this->posY && y <= this->posY + this->height);
}

void Texture::loadTexture(const char *filename)
{
    glGenTextures(1, &texPtr);
    glBindTexture(GL_TEXTURE_2D, texPtr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int x, y, n;
    auto *data = TextureLoader::loadTexture(filename, &x, &y, &n);

    if (data)
    {
        GLenum format = n == 4 ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        Logger::warn("Failed to load texture");
    }
    TextureLoader::free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::draw()
{
    // Draw the triangle
    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texPtr);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

    glBindVertexArray(VAO);           // Bind the VAO
    glDrawArrays(GL_TRIANGLES, 0, 6); // Draw the triangle
    glBindVertexArray(0);             // Unbind the VAO
}

void Texture::updatePosition()
{
    calcVertices();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Texture::calcVertices()
{
    // top left
    float x0 = (2.0f * this->posX) / WINDOW_WIDTH - 1.0f;
    float y0 = 1.0f - (2.0f * this->posY) / WINDOW_HEIGHT;

    // bottom right
    float x1 = (2.0f * (this->posX + this->width)) / WINDOW_WIDTH - 1.0f;
    float y1 = 1.0f - (2.0f * (this->posY + this->height)) / WINDOW_HEIGHT;

    // bottom left
    float x2 = x0;
    float y2 = y1;

    // top right
    float x3 = x1;
    float y3 = y0;

    vertices = {
        x0, y0, 0.0f, 0.0f, 0.0f, // top left
        x2, y2, 0.0f, 0.0f, 1.0f, // bottom left
        x1, y1, 0.0f, 1.0f, 1.0f, // bottom right

        x3, y3, 0.0f, 1.0f, 0.0f, // top right
        x0, y0, 0.0f, 0.0f, 0.0f, // top left
        x1, y1, 0.0f, 1.0f, 1.0f  // bottom right
    };
}

void Texture::prepare()
{
    /*    const char* vertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            void main()
            {
                gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
            }
        )";

        const char* fragmentShaderSource = R"(
            #version 330 core
            out vec4 FragColor;
            void main()
            {
                FragColor = vec4(0.0, 1.0, 0.0, 1.0);  // Green color
            }
        )"; */

    const char *vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoord;
    
        out vec2 TexCoord;
    
        void main()
        {
            gl_Position = vec4(aPos, 1.0);
            TexCoord = aTexCoord;
        }
    )";

    const char *fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
    
        in vec2 TexCoord;
    
        uniform sampler2D texture1;
    
        void main()
        {
            FragColor = texture(texture1, TexCoord);
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

    calcVertices();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
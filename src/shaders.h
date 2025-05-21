#pragma once
#include "vec2.hpp"
#include "colors.h"
#include "logger.h"
struct VertexPosColor
{
    glm::vec2 position;
    Color color;
};

struct VertexPosTexture
{
    glm::vec2 position;
    glm::vec2 texCoord;
};


namespace shaders
{
    unsigned int compileShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);

    constexpr const char* solidColorVertexShader = R"(
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

    constexpr const char* solidColorFragmentShader = R"(
            #version 330 core
            in vec4 vColor;
            out vec4 FragColor;
            void main()
            {
                FragColor = vColor;
            }
        )";

    constexpr const char* textureVertexShader = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aTexCoord;
    
        uniform mat4 cMatrix;
        out vec2 texCoord;
    
        void main()
        {
            gl_Position = cMatrix * vec4(aPos, 0.0, 1.0);
            texCoord = aTexCoord;
        }
    )";

    constexpr const char* textureFragmentShader = R"(
        #version 330 core
        out vec4 FragColor;
    
        in vec2 texCoord;
    
        uniform sampler2D texture1;
    
        void main()
        {
            FragColor = texture(texture1, texCoord);
        }
    )";

	constexpr const char* textVertexShader = R"(
        #version 330 core
        layout (location = 0) in vec4 vertex;
    
        uniform mat4 pMatrix;
        out vec2 texCoord;
    
        void main()
        {
            gl_Position = pMatrix * vec4(vertex.xy, 0.0, 1.0);
            texCoord = vertex.zw;
        }
    )";

	constexpr const char* textFragmentShader = R"(
        #version 330 core
        in vec2 texCoord;
        uniform sampler2D text;
        uniform vec3 textColor;
        out vec4 FragColor;
    
        void main()
        {
            vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texCoord).r);
            FragColor = vec4(textColor, 1.0) * sampled;
        }
    )";
}
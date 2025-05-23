#include "utils/logger.h"
#include "textrenderer.h"
#include "glad.h"
#include "shaders.h"
#include "camera2d.h"
#include <algorithm>

void TextRenderer::init()
{
	if (!atlas.load("assets/fonts/PressStart2P-Regular.ttf", fontSize))
	{
		LOG_ERROR << "ERROR::FREETYPE: Failed to load font atlas";
		return;
	}
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, bufferCapacity * sizeof(float) * 4, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	shaderProgram = shaders::compileShaderProgram(shaders::textVertexShader, shaders::textFragmentShader);
}

void TextRenderer::RenderText(const Camera2D& cam, const std::string& text, float x, float y, float scale, glm::vec3& color, bool fixed)
{
    size_t maxVerts = text.size() * 6;
    y += fontSize * scale;

    if (bufferCapacity < maxVerts) {
        bufferCapacity = maxVerts + 128; // add some padding
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, bufferCapacity * 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    std::vector<float> vertices;
    vertices.reserve(maxVerts * 4);
    float originX = x;

    for (char c : text) 
    {
        const Glyph& g = atlas.getGlyph(c);

        float xpos = x + g.bearing.x * scale;
        //float ypos = y - (g.size.y - g.bearing.y) * scale;
        float ypos = y - g.bearing.y * scale;
        float w = g.size.x * scale;
        float h = g.size.y * scale;

        glm::vec2 uv0 = g.uv0;
        glm::vec2 uv1 = g.uv1;

        // 2 triangles (6 vertices)
        float quad[6][4] = {
        { xpos,     ypos,     uv0.x, uv0.y },
        { xpos,     ypos + h, uv0.x, uv1.y },
        { xpos + w, ypos + h, uv1.x, uv1.y },

        { xpos,     ypos,     uv0.x, uv0.y },
        { xpos + w, ypos + h, uv1.x, uv1.y },
        { xpos + w, ypos,     uv1.x, uv0.y }
        };

        //float quad[6][4] = {
        //    { xpos,     ypos + h,   uv0.x, uv0.y },
        //    { xpos,     ypos,       uv0.x, uv1.y },
        //    { xpos + w, ypos,       uv1.x, uv1.y },

        //    { xpos,     ypos + h,   uv0.x, uv0.y },
        //    { xpos + w, ypos,       uv1.x, uv1.y },
        //    { xpos + w, ypos + h,   uv1.x, uv0.y }
        //};

        for (auto& v : quad)
            vertices.insert(vertices.end(), v, v + 4);

        x += (g.advance >> 6) * scale;
    }

	glUseProgram(shaderProgram);
    glm::mat4 pMatrix;
    if (fixed)
    {
        pMatrix = cam.getProjectionMatrix();
    }
    else
    {
        pMatrix = cam.getCombinedMatrix();
    }
    unsigned int mLoc = glGetUniformLocation(shaderProgram, "pMatrix");
    glUniformMatrix4fv(mLoc, 1, GL_FALSE, &pMatrix[0][0]);
    glUniform3f(glGetUniformLocation(shaderProgram, "textColor"), color.x, color.y, color.z);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlas.getTextureID());

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());

    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size() / 4));
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

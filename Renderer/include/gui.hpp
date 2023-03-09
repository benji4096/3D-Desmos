#pragma once

#include <map>

#include "glm/glm.hpp"
#include "freetype/freetype.h"

#include "Renderer.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"

struct Character
{
    unsigned int textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class FontSDF
{
private:
    GLuint mTextures[128];
public:
    uint32_t fontSize;
    float yMax = -1;
    std::map<char, Character> characters;

    FontSDF(const char* path, FT_UInt letterHeightResolution);
    FontSDF() = delete;
    ~FontSDF();
};

class TextSDF
{
    struct TextVert
    {
        glm::vec2 xy;
        glm::vec2 uv;
    };

    static const std::vector<TextVert> s_vertices;
    static const std::vector<uint8_t> s_indices;

    static std::optional < IMeshUniversal<TextVert, uint8_t>> s_mesh;
    static std::optional<Shader> s_shader;
    static std::optional<FontSDF> s_font;

    Shader* p_shader;
    FontSDF* p_font;

    glm::vec4 m_color = { 0.0f, 0.0f, 0.0f, 1.0f };
    uint32_t m_fontSize = 16;
    GLuint m_transformUniformLocation;
    GLuint m_colorUniformLocation;
    GLuint m_AADistUniformLocation;

    std::string m_text;

    float m_AA = 1.0f;
    glm::ivec2 m_pos = { 0, 0 };

    void initMesh();
    void initDefaultShader();
    void initDefaultFont();
    void init();

public:
    TextSDF();
    TextSDF(FontSDF* font);
    TextSDF(Shader* shader);
    TextSDF(Shader* shader, FontSDF* font);
    TextSDF(const TextSDF& other) = delete;
    const TextSDF& operator=(const TextSDF& other) = delete;

    void updateFont(FontSDF* font);
    void setText(const std::string& text);
    void setColor(const glm::vec4& color);
    void setSize(uint32_t size);
    void setAntiAliasAmmount(float ammount);
    void setPos(glm::ivec2 pos);
    void draw(float width, float height);
};




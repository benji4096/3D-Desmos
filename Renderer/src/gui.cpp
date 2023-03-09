#include "gui.hpp"

#include "glm/gtc/matrix_transform.hpp"


FontSDF::FontSDF(const char* path, FT_UInt letterHeightResolution)
    : fontSize(letterHeightResolution)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        throw std::runtime_error("Freetype failed to initalize");
    FT_Face face;
    if (FT_New_Face(ft, path, 0, &face))
        throw std::runtime_error("failed to load font");

    FT_Set_Pixel_Sizes(face, 0, letterHeightResolution);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(128, mTextures);

    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            throw std::runtime_error("failed to load Glyph");

        FT_GlyphSlot slot = face->glyph;
        FT_Render_Glyph(slot, FT_RENDER_MODE_SDF);


        //uint16_t newWidth = face->glyph->bitmap.width + 2;
        //uint16_t newHeight = face->glyph->bitmap.rows + 2;
        //std::vector<uint8_t> imageData(newWidth * newHeight);
        //
        //for (uint16_t x = 0; x < newWidth; x++)
        //    for (uint16_t y = 0; y < newHeight; y++)
        //        if (x == 0 || x == newWidth - 1 || y == 0 || y == newHeight - 1)
        //            imageData[x + newWidth * y] = 0;
        //        else
        //            imageData[x + newWidth * y] = face->glyph->bitmap.buffer[x - 1 + face->glyph->bitmap.width * (y - 1)];

        glBindTexture(GL_TEXTURE_2D, mTextures[c]);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        yMax = float(face->glyph->face->bbox.yMax >> 6);

        Character character = {
            mTextures[c],
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };

        characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

FontSDF::~FontSDF()
{
    glDeleteTextures(128, mTextures);
}


const std::vector<TextSDF::TextVert> TextSDF::s_vertices = {
        { { 1.0f, 1.0f }, { 1.0f, 0.0f} },
        { { 1.0f, 0.0f }, { 1.0f, 1.0f} },
        { { 0.0f, 0.0f }, { 0.0f, 1.0f} },
        { { 0.0f, 1.0f }, { 0.0f, 0.0f} }
};

const std::vector<uint8_t> TextSDF::s_indices = {
        0, 1, 3,
        1, 2, 3
};

std::optional<IMeshUniversal<TextSDF::TextVert, uint8_t>> TextSDF::s_mesh = {};
std::optional<Shader> TextSDF::s_shader = {};
std::optional<FontSDF> TextSDF::s_font = {};


void TextSDF::initMesh()
{
    if (!s_mesh.has_value())
    {
        std::vector<IMeshUniversalAttribute> attribs = {
            {0, 2, GL_FLOAT, GL_FALSE, offsetof(TextSDF::TextVert, xy) },
            {1, 2, GL_FLOAT, GL_FALSE, offsetof(TextSDF::TextVert, uv) }
        };

        s_mesh.emplace(&TextSDF::s_vertices, &TextSDF::s_indices, attribs, GL_UNSIGNED_BYTE);

        s_mesh.value().upload();
    }
}

void TextSDF::initDefaultShader()
{
    if (!s_shader.has_value())
    {
        s_shader.emplace();

        s_shader.value().addSources({"res/shaders/textSDF/simple.vert", "res/shaders/textSDF/simple.frag"});
        s_shader.value().compile();
        s_shader.value().link();
    }
    p_shader = &s_shader.value();
}

void TextSDF::initDefaultFont()
{
    if (!s_font.has_value())
    {
        s_font.emplace("res/fonts/Anonymous_Pro/AnonymousPro-Regular.ttf", 32);
    }
    p_font = &s_font.value();
}

void TextSDF::init()
{
    initMesh();

    m_transformUniformLocation = p_shader->getUniformLocation("transform");
    m_colorUniformLocation = p_shader->getUniformLocation("textColor");
    m_AADistUniformLocation = p_shader->getUniformLocation("aaDist");
}

TextSDF::TextSDF()
{
    initDefaultShader();
    initDefaultFont();

    init();
}
TextSDF::TextSDF(FontSDF* font)
{
    initDefaultShader();
    p_font = font;

    init();
}
TextSDF::TextSDF(Shader* shader)
{
    initDefaultFont();
    p_shader = shader;

    init();
}

TextSDF::TextSDF(Shader* shader, FontSDF* font)
{
    p_font = font;
    p_shader = shader;

    init();
}

void TextSDF::updateFont(FontSDF* pFont)
{ p_font = pFont; }
void TextSDF::setText(const std::string& pText)
{ m_text = pText; }
void TextSDF::setColor(const glm::vec4& pColor)
{ m_color = pColor; }
void TextSDF::setSize(uint32_t size)
{ m_fontSize = size; }
void TextSDF::setAntiAliasAmmount(float ammount)
{ m_AA = ammount; }
void TextSDF::setPos(glm::ivec2 pos)
{ m_pos = pos; }

void TextSDF::draw(glm::ivec2 windowDims)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);

    glActiveTexture(GL_TEXTURE0);
    p_shader->use();

    p_shader->uniform(m_colorUniformLocation, m_color);
    //glUniform4f(mColorUniformLocation, mColor.r, mColor.g, mColor.b, mColor.a);

    float currentX = 0.0f;
    float currentY = 0.0f;
    uint32_t line = 0;

    glm::mat4 projection = glm::ortho(0.0f, float(windowDims.x), 0.0f, float(windowDims.y));
    glm::mat4 scale;
    glm::mat4 translation;
    glm::mat4 transformation;

    for (int i = 0; i < m_text.size(); i++)
    {
        Character* c = &p_font->characters[m_text[i]];

        if (m_text[i] == '\n')
        {
            line++;
            currentX = 0.0f;
        }
        else
        {
            float h = float(m_fontSize) * float(c->size.y) / float(p_font->fontSize);
            float w = float(m_fontSize) * float(c->size.x) / float(p_font->fontSize);

            float tY = float(m_fontSize) / float(p_font->fontSize);
            float tX = float(m_fontSize) / float(p_font->fontSize);
            //float tY = float(mFontSize) / float(c->size.y);
            //float tX = float(mFontSize) / float(c->size.y);
            glm::vec2 bearing = { tX * float(c->bearing.x), tY * float(c->bearing.y) };

            translation = glm::translate(glm::mat4(1.0f), glm::vec3(glm::vec2(m_pos.x, -m_pos.y) + glm::vec2(currentX + tX * float(c->bearing.x), windowDims.y - (int(line) + 1) * int(m_fontSize) + tY * float(c->bearing.y - c->size.y)), 0.0f));
            scale = glm::scale(glm::mat4(1.0f), glm::vec3(glm::vec2(tY * float(c->size.x), tX * float(c->size.y)), 1.0f));

            currentX += tX * float(c->advance >> 6);

            //std::cout << mText[i] << " " << tY * float(c->bearing.y) << " " << mFontSize << std::endl;

            float aaDist = m_AA * 0.035f / tX;

            transformation = projection * translation * scale;

            p_shader->uniform(m_transformUniformLocation, transformation);
            p_shader->uniform(m_AADistUniformLocation, aaDist);
            //glUniformMatrix4fv(mTransformUniformLocation, 1, GL_FALSE, glm::value_ptr(transformation));
            //glUniform1f(mAADistUniformLocation, aaDist);
            glBindTexture(GL_TEXTURE_2D, p_font->characters[m_text[i]].textureID);
            s_mesh.value().draw();
        }
    }
}

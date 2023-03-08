#include "gui.hpp"


#include <map>
#include <string>

#include "glm/glm.hpp"
#include "freetype/freetype.h"

#include "Renderer.hpp"
#include "Shader.hpp"

struct Character
{
	unsigned int textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;

	~Character();
};

struct FontSDF
{
private:
	GLuint mTextures[128];
public:
	uint32_t fontSize;
	float yMax = -1;
	std::map<char, Character> characters;

	FontSDF(const std::string& path, FT_UInt letterHeightResolution);
	~FontSDF();
};

class TextSDF
{
	struct TextVert
	{
		glm::vec2 xy;
		glm::vec2 uv;
	};

	Shader* m_shaderProgram;
	FontSDF* m_font;

	IMeshUniversal<TextVert, uint8_t> mMesh =
	{
		{ {0, 2, GL_FLOAT, GL_FALSE, offsetof(TextVert, xy) },
		{1, 2, GL_FLOAT, GL_FALSE, offsetof(TextVert, uv) }
		}, GL_UNSIGNED_BYTE
	};

	glm::vec4 mColor;
	uint32_t mFontSize;
	GLuint mTransformUniformLocation;
	GLuint mColorUniformLocation;
	GLuint mAADistUniformLocation;

	std::string mText;

	float mAA = 1.0f;
	glm::ivec2 mPos = { 0, 0 };

	void init();

public:
	TextSDF(Renderer* pRenderer);
	TextSDF(Renderer* pRenderer, FontSDF* pFont);
	TextSDF(Renderer* pRenderer, Shader* pShader);
	TextSDF(Renderer* pRenderer, Shader* pShader, FontSDF* pFont);
	~TextSDF();

	void updateFont(FontSDF* pFont);
	void setText(const std::string& pText);
	void setColor(const glm::vec4& pColor);
	void setSize(uint32_t size);
	void setAntiAliasAmmount(float ammount);
	void setPos(glm::ivec2 pos);
	void draw();
};

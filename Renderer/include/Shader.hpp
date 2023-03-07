#pragma once

#include <string>
#include <map>
#include <vector>
#include <optional>
#include <ostream>

#include "GL/glew.h"
#include "glm.hpp"

#include "glUtils.h"

enum class SHADER_SOURCE_TYPE
{
	NONE,
	VERTEX,
	TESS_CONTROL,
	TESS_EVALUATION,
	GEOMETRY,
	FRAGMENT,
	COMPUTE
};

std::ostream& operator<<(std::ostream& out, const SHADER_SOURCE_TYPE& in);

struct ShaderSourceInfo
{
	std::string path;
	SHADER_SOURCE_TYPE type;
};

class Shader
{
	const std::string mCommandInfoString = "\n// ################ INFO: ";
	const std::string mCommandWarningString = "\n// ################################ WARNING: ";

	struct ShaderSource
	{
		bool empty;

		std::string source;
		SHADER_SOURCE_TYPE type;

		ShaderSource();
		ShaderSource(std::string pSource, SHADER_SOURCE_TYPE pType);
	};

	struct ShaderInclude
	{
		std::string path;
		std::string source;

		bool comparePath(const std::string& pPath);
	};

	std::map<std::string, ShaderSource> mSources;
	std::map<std::string, GLuint> mUniformLocations;

	std::map<std::string, std::string> mShaderIncludes;

	std::optional<std::vector<std::string>> findCommand(std::string line, std::string pCommand);
	void processCommand(std::string& line, uint16_t pRecurDepth);

	
	std::string loadShaderSource(std::string pPath, uint16_t pRecurDepth = 0);
	void loadInclude(std::string pPath, uint16_t pRecurDepth = 0);

	struct raiiShaderID
	{
		GLuint id;

		operator GLuint();
		raiiShaderID(GLenum type);
		raiiShaderID(SHADER_SOURCE_TYPE type);
		raiiShaderID(const raiiShaderID& other) = delete;
		raiiShaderID(raiiShaderID&& other) noexcept;
		raiiShaderID& operator=(const raiiShaderID& other) = delete;
		raiiShaderID& operator=(raiiShaderID&& other) noexcept;

		~raiiShaderID();
	};
	struct raiiProgramID
	{
		GLuint id;

		operator GLuint();
		raiiProgramID();
		raiiProgramID(const raiiProgramID& other) = delete;
		raiiProgramID(raiiProgramID&& other) noexcept;
		raiiProgramID& operator=(const raiiProgramID& other) = delete;
		raiiProgramID& operator=(raiiProgramID&& other) noexcept;
		~raiiProgramID();
	};

	std::vector<raiiShaderID> mShaderIDs;
	raiiProgramID mProgramID;

public:
	Shader();
	~Shader();

	void addSource(const ShaderSourceInfo& sourceInfo);
	void addSources(std::vector<std::string> paths);

	void compile();
	void link();

	void use();

	GLuint getUniformLocation(const char* loc);

	void uniform(GLint location, const float& value);
	void uniform(GLint location, const glm::vec2& value);
	void uniform(GLint location, const glm::vec3& value);
	void uniform(GLint location, const glm::vec4& value);
	void uniform(GLint location, const int32_t& value);
	void uniform(GLint location, const glm::ivec2& value);
	void uniform(GLint location, const glm::ivec3& value);
	void uniform(GLint location, const glm::ivec4& value);
	void uniform(GLint location, const uint32_t& value);
	void uniform(GLint location, const glm::uvec2& value);
	void uniform(GLint location, const glm::uvec3& value);
	void uniform(GLint location, const glm::uvec4& value);
	void uniform(GLint location, GLsizei count, GLfloat* value);
	void uniform(GLint location, GLsizei count, glm::vec2* value);
	void uniform(GLint location, GLsizei count, glm::vec3* value);
	void uniform(GLint location, GLsizei count, glm::vec4* value);
	void uniform(GLint location, GLsizei count, GLint* value);
	void uniform(GLint location, GLsizei count, glm::ivec2* value);
	void uniform(GLint location, GLsizei count, glm::ivec3* value);
	void uniform(GLint location, GLsizei count, glm::ivec4* value);
	void uniform(GLint location, GLsizei count, GLuint* value);
	void uniform(GLint location, GLsizei count, glm::uvec2* value);
	void uniform(GLint location, GLsizei count, glm::uvec3* value);
	void uniform(GLint location, GLsizei count, glm::uvec4* value);
	void uniform(GLint location, GLsizei count, glm::mat2* value, GLboolean transpose = false);
	void uniform(GLint location, GLsizei count, glm::mat3* value, GLboolean transpose = false);
	void uniform(GLint location, GLsizei count, glm::mat4* value, GLboolean transpose = false);
	void uniform(GLint location, GLsizei count, glm::mat2x3* value, GLboolean transpose = false);
	void uniform(GLint location, GLsizei count, glm::mat3x2* value, GLboolean transpose = false);
	void uniform(GLint location, GLsizei count, glm::mat2x4* value, GLboolean transpose = false);
	void uniform(GLint location, GLsizei count, glm::mat4x2* value, GLboolean transpose = false);
	void uniform(GLint location, GLsizei count, glm::mat3x4* value, GLboolean transpose = false);
	void uniform(GLint location, GLsizei count, glm::mat4x3* value, GLboolean transpose = false);

	void uniform(GLint location, glm::mat2& value, GLboolean transpose = false);
	void uniform(GLint location, glm::mat3& value, GLboolean transpose = false);
	void uniform(GLint location, glm::mat4& value, GLboolean transpose = false);	
	void uniform(GLint location, glm::mat2x3& value, GLboolean transpose = false);
	void uniform(GLint location, glm::mat3x2& value, GLboolean transpose = false);
	void uniform(GLint location, glm::mat2x4& value, GLboolean transpose = false);
	void uniform(GLint location, glm::mat4x2& value, GLboolean transpose = false);
	void uniform(GLint location, glm::mat3x4& value, GLboolean transpose = false);
	void uniform(GLint location, glm::mat4x3& value, GLboolean transpose = false);
};

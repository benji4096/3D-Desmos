#include "Shader.hpp"

#include <sstream>
#include <fstream>
#include <stdexcept>
#include <filesystem>

std::ostream& operator<<(std::ostream& out, const SHADER_SOURCE_TYPE& in)
{
	switch (in)
	{
		case SHADER_SOURCE_TYPE::NONE:				out << "SHADER_SOURCE_TYPE::NONE";				break;
		case SHADER_SOURCE_TYPE::VERTEX:			out << "SHADER_SOURCE_TYPE::VERTEX";			break;
		case SHADER_SOURCE_TYPE::TESS_CONTROL:		out << "SHADER_SOURCE_TYPE::TESS_CONTROL";		break;
		case SHADER_SOURCE_TYPE::TESS_EVALUATION:	out << "SHADER_SOURCE_TYPE::TESS_EVALUATION";	break;
		case SHADER_SOURCE_TYPE::GEOMETRY:			out << "SHADER_SOURCE_TYPE::GEOMETRY";			break;
		case SHADER_SOURCE_TYPE::FRAGMENT:			out << "SHADER_SOURCE_TYPE::FRAGMENT";			break;
		case SHADER_SOURCE_TYPE::COMPUTE:			out << "SHADER_SOURCE_TYPE::COMPUTE";			break;
	}
	return out;
}

Shader::ShaderSource::ShaderSource()
	: empty(true), source(), type(SHADER_SOURCE_TYPE::NONE)
{
}
Shader::ShaderSource::ShaderSource(std::string pSource, SHADER_SOURCE_TYPE pType)
	: empty(false), source(pSource), type(pType)
{
}

bool Shader::ShaderInclude::comparePath(const std::string& pPath)
{
	return std::filesystem::path(path).compare(std::filesystem::path(pPath));
}

std::string Shader::loadShaderSource(std::string pPath, uint16_t pRecurDepth)
{
	std::ifstream fileStream;
	fileStream.open(pPath);
	if (!fileStream.is_open())
	{
		std::string err = "could not find source file: \"" + pPath + '\"';
		throw std::runtime_error(err);
		return { };
	}

	std::stringstream data;
	std::string line;
	while (std::getline(fileStream, line))
	{
		processCommand(line, pRecurDepth);
		data << line << '\n';
	}
	fileStream.close();

	return data.str();
}

void Shader::loadInclude(std::string pPath, uint16_t pRecurDepth)
{
	const uint16_t maxRecurDepth = 128;

	if (pRecurDepth > maxRecurDepth)
		throw std::runtime_error("Shader Include Max Recursion Depth Reached");
	else
	{
		std::ifstream fileStream;
		fileStream.open(pPath);
		if (!fileStream.is_open())
		{
			std::string err = "could not find include file: \"" + pPath + '\"';
			throw std::runtime_error(err);
		}

		std::stringstream data;
		std::string line;
		while (std::getline(fileStream, line))
		{
			processCommand(line, pRecurDepth);
			data << line << '\n';
		}
		fileStream.close();

		mShaderIncludes.insert({ pPath, data.str() });
	}
}

std::optional<std::vector<std::string>> Shader::findCommand(std::string line, std::string pCommand)
{
	std::optional<std::vector<std::string>> empty;
	if (line.size() < pCommand.size() + 1 || line[0] != '#')
		return { };
	else
	{
		bool failed = false;
		for (uint32_t i = 0; i < pCommand.size(); i++)
		{
			if (line[i + 1] != pCommand[i])
				failed = true;
		}
		if (failed)
			return { };
		else
		{
			std::vector<std::string> args = { };

			bool inQuotes = false;
			int j = -1;
			for (int i = pCommand.size() + 1; i < line.size(); i++)
			{
				if (line[i] == '\"')
					inQuotes = !inQuotes;
				else
				{
					if (line[i] == ' ' && !inQuotes)
					{
						j++;
						args.push_back("");
					}
					else
						args[j] += line[i];
				}
			}

			return { args };
		}
	}
}

void Shader::processCommand(std::string& line, uint16_t pRecurDepth)
{
	std::optional<std::vector<std::string>> paths = findCommand(line, "include");
	if (paths.has_value())
	{
		if (paths.value().size() > 0)
			for (int i = 0; i < paths.value().size(); i++)
			{
				if (!mShaderIncludes.count(paths.value()[i]))
					loadInclude(paths.value()[i], pRecurDepth + 1);
				line = mCommandInfoString + "include file start(\"" + paths.value()[i] + "\")\n" + mShaderIncludes[paths.value()[i]] + mCommandInfoString + "include file end\n";
			}
		else
			line = mCommandWarningString + "unused include command\n";
	}
}

void Shader::addSource(const ShaderSourceInfo& sourceInfo)
{
	mSources[sourceInfo.path] = { loadShaderSource(sourceInfo.path), sourceInfo.type};
	/*if (mSources.count(sourceInfo.path))
	{
		std::cout << std::endl << "\t\tPATH: \"" << sourceInfo.path << '\"' << std::endl;
		std::cout << mSources[sourceInfo.path].source;
		std::cout << std::endl;
	}*/
}

void Shader::addSources(std::vector<std::string> paths)
{
	for (int i = 0; i < paths.size(); i++)
	{
		std::stringstream fileExtensionSS;
		for (size_t j = paths[i].size(); j > 0; j--)
		{
			if (paths[i][j - 1] == '.')
				break;
			else
				fileExtensionSS << paths[i][j - 1];
		}
		std::string fileExtension = fileExtensionSS.str();

		if (fileExtension.size() != 4)
			throw std::runtime_error("Unknown File Extension!");

		const uint32_t VERT = 0x76657274;
		const uint32_t TESC = 0x74657363;
		const uint32_t TESE = 0x74657365;
		const uint32_t GEOM = 0x67656F6D;
		const uint32_t FRAG = 0x66726167;
		const uint32_t COMP = 0x636F6D70;

		uint32_t typeID = 0;
		for (uint16_t i = 0; i < 4; i++)
			typeID = typeID | (fileExtension[i] << 8 * i);

		SHADER_SOURCE_TYPE shaderType = SHADER_SOURCE_TYPE::NONE;
		switch (typeID)
		{
		case VERT:
		{ shaderType = SHADER_SOURCE_TYPE::VERTEX; break; }
		case TESC:
		{ shaderType = SHADER_SOURCE_TYPE::TESS_CONTROL; break; }
		case TESE:
		{ shaderType = SHADER_SOURCE_TYPE::TESS_EVALUATION; break; }
		case GEOM:
		{ shaderType = SHADER_SOURCE_TYPE::GEOMETRY; break; }
		case FRAG:
		{ shaderType = SHADER_SOURCE_TYPE::FRAGMENT; break; }
		case COMP:
		{ shaderType = SHADER_SOURCE_TYPE::COMPUTE; break; }
		}
		if (shaderType == SHADER_SOURCE_TYPE::NONE)
			throw std::runtime_error("Unknown File Extension!");

		addSource({ paths[i], shaderType });
	}
}

Shader::raiiShaderID::operator GLuint()
{ return id; }
Shader::raiiShaderID::raiiShaderID(GLenum type)
	: id(glCreateShader(type)) { }
Shader::raiiShaderID::raiiShaderID(SHADER_SOURCE_TYPE type)
	: id(0)
{
	switch (type)
	{
		case SHADER_SOURCE_TYPE::NONE:				throw std::runtime_error("raiiShaderID object initalized with none type"); break;
		case SHADER_SOURCE_TYPE::VERTEX:			id = glCreateShader(GL_VERTEX_SHADER); break;
		case SHADER_SOURCE_TYPE::TESS_CONTROL:		id = glCreateShader(GL_TESS_CONTROL_SHADER); break;
		case SHADER_SOURCE_TYPE::TESS_EVALUATION:	id = glCreateShader(GL_TESS_EVALUATION_SHADER); break;
		case SHADER_SOURCE_TYPE::GEOMETRY:			id = glCreateShader(GL_GEOMETRY_SHADER); break;
		case SHADER_SOURCE_TYPE::FRAGMENT:			id = glCreateShader(GL_FRAGMENT_SHADER); break;
		case SHADER_SOURCE_TYPE::COMPUTE:			id = glCreateShader(GL_COMPUTE_SHADER); break;
	}
}
Shader::raiiShaderID::raiiShaderID(raiiShaderID&& other) noexcept
	: id(other.id)
{ other.id = 0; }
Shader::raiiShaderID& Shader::raiiShaderID::operator=(raiiShaderID&& other) noexcept
{
	id = other.id;
	other.id = 0;
	return *this;
}
Shader::raiiShaderID::~raiiShaderID()
{ glDeleteShader(id); id = 0; }

Shader::raiiProgramID::operator GLuint()
{ return id; }
Shader::raiiProgramID::raiiProgramID()
	: id(glCreateProgram()) { }
Shader::raiiProgramID::raiiProgramID(raiiProgramID&& other) noexcept
	: id(other.id)
{ other.id = 0; }
Shader::raiiProgramID& Shader::raiiProgramID::operator=(raiiProgramID&& other) noexcept
{
	id = other.id;
	other.id = 0;
	return *this;
}
Shader::raiiProgramID::~raiiProgramID()
{ glDeleteProgram(id); }

Shader::Shader()
	: mSources(), mShaderIDs(), mProgramID()
{ }

Shader::~Shader()
{ }

void Shader::compile()
{
	for (const auto& source : mSources)
	{
		mShaderIDs.push_back(source.second.type);

		const char* src = source.second.source.c_str();
		glShaderSource(mShaderIDs.back(), 1, &src, NULL);
		glCompileShader(mShaderIDs.back());
		int success;
		char infoLog[512];
		glGetShaderiv(mShaderIDs.back(), GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(mShaderIDs.back(), 512, NULL, infoLog);
			glCheckError(__LINE__);
			std::stringstream ss;
			ss << "Failed to compile shader of type " << source.second.type << "\nInfo Log:\n" << infoLog;
			ss << "\n SHADER SOURCE CODE: \n" << src << std::endl;
			throw std::runtime_error(ss.str());
		}

		glAttachShader(mProgramID, mShaderIDs.back());
	}
}

void Shader::link()
{
	glLinkProgram(mProgramID);
	int success;
	char infoLog[512];
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(mProgramID, 512, NULL, infoLog);
		throw std::runtime_error(std::string("Failed to link shader: ") + infoLog);
	}
}

void Shader::use()
{
	glUseProgram(mProgramID);
}

GLuint Shader::getUniformLocation(const char* loc)
{
	return glGetUniformLocation(mProgramID, loc);
}

void Shader::uniform(GLint location, const GLfloat& value)
{
	glUseProgram(mProgramID);
	glUniform1f(location, value);
}
void Shader::uniform(GLint location, const glm::vec2& value)
{
	glUseProgram(mProgramID);
	glUniform2f(location, value.x, value.y);
}
void Shader::uniform(GLint location, const glm::vec3& value)
{
	glUseProgram(mProgramID);
	glUniform3f(location, value.x, value.y, value.z);
}
void Shader::uniform(GLint location, const glm::vec4& value)
{
	glUseProgram(mProgramID);
	glUniform4f(location, value.x, value.y, value.z, value.w);
}
void Shader::uniform(GLint location, const GLint& value)
{
	glUseProgram(mProgramID);
	glUniform1i(location, value);
}
void Shader::uniform(GLint location, const glm::ivec2& value)
{
	glUseProgram(mProgramID);
	glUniform2i(location, value.x, value.y);
}
void Shader::uniform(GLint location, const glm::ivec3& value)
{
	glUseProgram(mProgramID);
	glUniform3i(location, value.x, value.y, value.z);
}
void Shader::uniform(GLint location, const glm::ivec4& value)
{
	glUseProgram(mProgramID);
	glUniform4i(location, value.x, value.y, value.z, value.w);
}
void Shader::uniform(GLint location, const GLuint& value)
{
	glUseProgram(mProgramID);
	glUniform1ui(location, value);
}
void Shader::uniform(GLint location, const glm::uvec2& value)
{
	glUseProgram(mProgramID);
	glUniform2ui(location, value.x, value.y);
}
void Shader::uniform(GLint location, const glm::uvec3& value)
{
	glUseProgram(mProgramID);
	glUniform3ui(location, value.x, value.y, value.z);
}
void Shader::uniform(GLint location, const glm::uvec4& value)
{
	glUseProgram(mProgramID);
	glUniform4ui(location, value.x, value.y, value.z, value.w);
}
void Shader::uniform(GLint location, GLsizei count, GLfloat* value)
{
	glUseProgram(mProgramID);
	glUniform1fv(location, count, value);
}
void Shader::uniform(GLint location, GLsizei count, glm::vec2* value)
{
	glUseProgram(mProgramID);
	glUniform2fv(location, count, (GLfloat*)value);
}
void Shader::uniform(GLint location, GLsizei count, glm::vec3* value)
{
	glUseProgram(mProgramID);
	glUniform3fv(location, count, (GLfloat*)value);
}
void Shader::uniform(GLint location, GLsizei count, glm::vec4* value)
{
	glUseProgram(mProgramID);
	glUniform4fv(location, count, (GLfloat*)value);
}
void Shader::uniform(GLint location, GLsizei count, GLint* value)
{
	glUseProgram(mProgramID);
	glUniform1iv(location, count, (GLint*)value);
}
void Shader::uniform(GLint location, GLsizei count, glm::ivec2* value)
{
	glUseProgram(mProgramID);
	glUniform2iv(location, count, (GLint*)value);
}
void Shader::uniform(GLint location, GLsizei count, glm::ivec3* value)
{
	glUseProgram(mProgramID);
	glUniform3iv(location, count, (GLint*)value);
}
void Shader::uniform(GLint location, GLsizei count, glm::ivec4* value)
{
	glUseProgram(mProgramID);
	glUniform4iv(location, count, (GLint*)value);
}
void Shader::uniform(GLint location, GLsizei count, GLuint* value)
{
	glUseProgram(mProgramID);
	glUniform1uiv(location, count, (GLuint*)value);
}
void Shader::uniform(GLint location, GLsizei count, glm::uvec2* value)
{
	glUseProgram(mProgramID);
	glUniform2uiv(location, count, (GLuint*)value);
}
void Shader::uniform(GLint location, GLsizei count, glm::uvec3* value)
{
	glUseProgram(mProgramID);
	glUniform3uiv(location, count, (GLuint*)value);
}
void Shader::uniform(GLint location, GLsizei count, glm::uvec4* value)
{
	glUseProgram(mProgramID);
	glUniform4uiv(location, count, (GLuint*)value);
}
void Shader::uniform(GLint location, GLsizei count, glm::mat2* value, GLboolean transpose)
{
	glUseProgram(mProgramID);
	glUniformMatrix2fv(location, count, transpose, (GLfloat*)value);
}
void Shader::uniform(GLint location, GLsizei count, glm::mat3* value, GLboolean transpose)
{
	glUseProgram(mProgramID);
	glUniformMatrix3fv(location, count, transpose, (GLfloat*)value);
}
void Shader::uniform(GLint location, GLsizei count, glm::mat4* value, GLboolean transpose)
{
	glUseProgram(mProgramID);
	glUniformMatrix4fv(location, count, transpose, (GLfloat*)value);
}
void Shader::uniform(GLint location, GLsizei count, glm::mat2x3* value, GLboolean transpose)
{
	glUseProgram(mProgramID);
	glUniformMatrix2x3fv(location, count, transpose, (GLfloat*)value);
}
void Shader::uniform(GLint location, GLsizei count, glm::mat3x2* value, GLboolean transpose)
{
	glUseProgram(mProgramID);
	glUniformMatrix3x2fv(location, count, transpose, (GLfloat*)value);
}
void Shader::uniform(GLint location, GLsizei count, glm::mat2x4* value, GLboolean transpose)
{
	glUseProgram(mProgramID);
	glUniformMatrix2x4fv(location, count, transpose, (GLfloat*)value);
}
void Shader::uniform(GLint location, GLsizei count, glm::mat4x2* value, GLboolean transpose)
{
	glUseProgram(mProgramID);
	glUniformMatrix4x2fv(location, count, transpose, (GLfloat*)value);
}
void Shader::uniform(GLint location, GLsizei count, glm::mat3x4* value, GLboolean transpose)
{
	glUseProgram(mProgramID);
	glUniformMatrix3x4fv(location, count, transpose, (GLfloat*)value);
}
void Shader::uniform(GLint location, GLsizei count, glm::mat4x3* value, GLboolean transpose)
{
	glUseProgram(mProgramID);
	glUniformMatrix4x3fv(location, count, transpose, (GLfloat*)value);
}

void Shader::uniform(GLint location, glm::mat2& value, GLboolean transpose)
{
	glUseProgram(mProgramID);
	glUniformMatrix2fv(location, 1, transpose, (GLfloat*)(&value));
}
void Shader::uniform(GLint location, glm::mat3& value, GLboolean transpose)
{
	glUseProgram(mProgramID);
	glUniformMatrix3fv(location, 1, transpose, (GLfloat*)(&value));
}
void Shader::uniform(GLint location, glm::mat4& value, GLboolean transpose)
{
	glUseProgram(mProgramID);
	glUniformMatrix4fv(location, 1, transpose, (GLfloat*)(&value));
}
void Shader::uniform(GLint location, glm::mat2x3& value, GLboolean transpose)
{
	glUseProgram(mProgramID);
	glUniformMatrix2x3fv(location, 1, transpose, (GLfloat*)(&value));
}
void Shader::uniform(GLint location, glm::mat3x2& value, GLboolean transpose)
{
	glUseProgram(mProgramID);
	glUniformMatrix3x2fv(location, 1, transpose, (GLfloat*)(&value));
}
void Shader::uniform(GLint location, glm::mat2x4& value, GLboolean transpose)
{
	glUseProgram(mProgramID);
	glUniformMatrix2x4fv(location, 1, transpose, (GLfloat*)(&value));
}
void Shader::uniform(GLint location, glm::mat4x2& value, GLboolean transpose)
{
	glUseProgram(mProgramID);
	glUniformMatrix4x2fv(location, 1, transpose, (GLfloat*)(&value));
}
void Shader::uniform(GLint location, glm::mat3x4& value, GLboolean transpose)
{
	glUseProgram(mProgramID);
	glUniformMatrix3x4fv(location, 1, transpose, (GLfloat*)(&value));
}
void Shader::uniform(GLint location, glm::mat4x3& value, GLboolean transpose)
{
	glUseProgram(mProgramID);
	glUniformMatrix4x3fv(location, 1, transpose, (GLfloat*)(&value));
}


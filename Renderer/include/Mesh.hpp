#pragma once

#include <vector>
#include <iostream>

#include "GL/glew.h"


struct IMeshUniversalAttribute
{
	GLuint bindingIndex;
	GLuint stride;
	GLenum type;
	GLboolean normalized;
	GLuint offset;
};

template <class VT, class IT> // arbitrary vertex and index type
class IMeshUniversal
{
private:

	std::vector<IMeshUniversalAttribute> m_attributes;
	GLenum m_indexType;

	std::vector<VT> const *p_verts;
	std::vector<IT> const *p_indices;

	GLuint m_vao = 0;

	GLuint m_vbo = 0;
	GLuint m_ibo = 0;

	bool dataIsUploaded = false;

	bool hasVerts()
	{
		return p_verts->size();
	}

public:
	IMeshUniversal(std::vector<VT> const *verts, std::vector<IT> const *indices, const std::vector<IMeshUniversalAttribute> attributes, GLenum indexType)
		: p_verts(verts), p_indices(indices), m_attributes(attributes), m_indexType(indexType)
	{
		glCreateVertexArrays(1, &m_vao);
		//glGenVertexArrays(1, &m_vao);
	};
	~IMeshUniversal()
	{
		clear();
		glDeleteVertexArrays(1, &m_vao);
	}
	IMeshUniversal(const IMeshUniversal& other) = delete;
	IMeshUniversal operator=(const IMeshUniversal& other) = delete;

	void upload()
	{
		if (dataIsUploaded)
			clear();

		//////float vertices[] = {
		//////	 0.5f,  0.5f, 0.0f,  // top right
		//////	 0.5f, -0.5f, 0.0f,  // bottom right
		//////	-0.5f, -0.5f, 0.0f,  // bottom left
		//////	-0.5f,  0.5f, 0.0f   // top left 
		//////};
		//////for (int i = 0; i < sizeof(vertices) / sizeof(float); i++)
		//////	vertices[i] += 0.5;
		//////unsigned int indices[] = {  // note that we start from 0!
		//////	0, 1, 3,   // first triangle
		//////	1, 2, 3    // second triangle
		//////};

		//glGenVertexArrays(1, &vao);
		//glBindVertexArray(vao);
		if (hasVerts())
		{
			glCreateBuffers(1, &m_vbo);
			glNamedBufferStorage(m_vbo, sizeof(VT) * p_verts->size(), p_verts->data(), GL_DYNAMIC_STORAGE_BIT);
			glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, sizeof(VT));

			glCreateBuffers(1, &m_ibo);
			glNamedBufferStorage(m_ibo, sizeof(IT) * p_indices->size(), p_indices->data(), GL_DYNAMIC_STORAGE_BIT);
			glVertexArrayElementBuffer(m_vao, m_ibo);

			for (const IMeshUniversalAttribute& atribute : m_attributes)
			{
				glEnableVertexArrayAttrib(m_vao, atribute.bindingIndex);
				glVertexArrayAttribFormat(m_vao, atribute.bindingIndex, atribute.stride, atribute.type, atribute.normalized, atribute.offset);
				glVertexArrayAttribBinding(m_vao, atribute.bindingIndex, 0);
			}

			dataIsUploaded = true;
		}

		////glBindVertexBuffer(0, vbo, 0, sizeof(VertexType));
		//
		//
		////glGenBuffers(1, &vbo);
		////glBindBuffer(GL_ARRAY_BUFFER, vbo);
		////glBufferData(GL_ARRAY_BUFFER, mVerts.size() * sizeof(VertexType), &mVerts[0], GL_STATIC_DRAW);
		//
		//glGenBuffers(1, &ebo);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(IndexType), &mIndices[0], GL_STATIC_DRAW);
		//
		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		//glEnableVertexAttribArray(0);
		////for (size_t i = 0; i < mAttributes.size(); i++)
		////{
		////	addAttrib(mAttributes[i]);
		////}

		//glBindVertexArray(0);
	}
	const void draw()
	{
		if (hasVerts())
		{
			glBindVertexArray(m_vao);
			glDrawElements(GL_TRIANGLES, p_indices->size(), m_indexType, 0);
			glBindVertexArray(0);
		}
	}
	void clear()
	{
		if (dataIsUploaded)
		{
			glDeleteBuffers(1, &m_vbo); m_vbo = 0;
			glDeleteBuffers(1, &m_ibo); m_ibo = 0;
			dataIsUploaded = false;
		}
	}
};

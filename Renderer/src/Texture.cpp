#include "Texture.hpp"


//Texture2D::Texture2D(std::string path, bool mipmap)
//    : mMipmap(mipmap)
//{
//    glGenTextures(1, &id);
//    glBindTexture(GL_TEXTURE_2D, id);
//
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    if (mipmap)
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    else
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    int width, height, channels;
//
//    stbi_set_flip_vertically_on_load(true);
//    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
//    mSize = { width, height };
//
//    GLenum format = 0;
//    switch (channels)
//    {
//    case 1:
//    { format = GL_RED; break; }
//    case 2:
//    { format = GL_RG; break; }
//    case 3:
//    { format = GL_RGB; break; }
//    case 4:
//    { format = GL_RGBA; break; }
//    }
//    if (data != nullptr)
//    {
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//        if (mipmap)
//            glGenerateMipmap(GL_TEXTURE_2D);
//    }
//    else
//        throw std::runtime_error(std::string("Failed to load image: ") + path);
//
//    stbi_image_free(data);
//}

Texture2D::Texture2D(unsigned char* data, glm::ivec2 size, GLenum format, bool mipmap)
    : mSize(size), mMipmap(mipmap)
{
    //for (int i = 0; i < 4 * mSize.x * mSize.y; i++)
    //    std::cout << float(data[i]) / 255.0f << std::endl;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    if (mipmap)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format, mSize.x, mSize.y, 0, format, GL_UNSIGNED_BYTE, data);
    if (mMipmap)
        glGenerateMipmap(GL_TEXTURE_2D);
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &id);
}

void Texture2D::updateData(unsigned char* data, glm::ivec2 size, GLenum format)
{
    glBindTexture(GL_TEXTURE_2D, id);

    if (size == mSize)
    {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mSize.x, mSize.y, format, GL_UNSIGNED_BYTE, data);
        if (mMipmap)
            glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        mSize = size;
        glTexImage2D(GL_TEXTURE_2D, 0, format, mSize.x, mSize.y, 0, format, GL_UNSIGNED_BYTE, data);
        if (mMipmap)
            glGenerateMipmap(GL_TEXTURE_2D);
    }
}

GLuint Texture2D::get()
{
    return id;
}

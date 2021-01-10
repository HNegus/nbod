#include "texture.hpp"
#include "stb_image.h"

void Texture::Bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_renderer_id);
}


void Texture::UnBind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Renew(const std::string& path) {
    glDeleteTextures(1, &m_renderer_id);

    stbi_set_flip_vertically_on_load(1);
    m_local_buffer = stbi_load(path.c_str(), &m_width, &m_height, &m_bpp, 4);

    glGenTextures(1, &m_renderer_id);
    glBindTexture(GL_TEXTURE_2D, m_renderer_id);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                 m_width, m_height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, m_local_buffer);

    glBindTexture(GL_TEXTURE_2D, 0);

    if (m_local_buffer) stbi_image_free(m_local_buffer);
}

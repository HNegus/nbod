#pragma once

#include "utils.hpp"

class Texture {

private:
    unsigned int m_renderer_id;
    std::string m_filepath;
    unsigned char* m_local_buffer;
    int m_width, m_height, m_bpp;

public:
    Texture() {}
    Texture(const std::string& path)
        : m_renderer_id(0),
        m_filepath(path),
        m_local_buffer(nullptr),
        m_width(0),
        m_height(0),
        m_bpp(0)
    {
        Renew(path);
    }
    ~Texture() { glDeleteTextures(1, &m_renderer_id); }

    void Bind(unsigned int slot = 0) const;
    void UnBind() const;

    void Renew(const std::string& path);
    // TODO Update

    inline int GetWidth() const { return m_width; };
    inline int GetHeight() const { return m_height; };

};

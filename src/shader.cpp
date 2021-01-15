#include "shader.hpp"

void Shader::Bind() const {
    glUseProgram(m_renderer_id);
}

void Shader::UnBind() const {
    glUseProgram(0);
}

void Shader::Renew(const ShaderSources sources) {
    glDeleteProgram(m_renderer_id);
    m_uniform_cache.clear();
    m_sources = sources;
    m_renderer_id = CreateShaderProgram(sources);
    glUseProgram(m_renderer_id);
}

void Shader::SetUniform1i(const std::string& name, const int v0) {
    glUniform1i(GetUniformLocation(name), v0);
}

void Shader::SetUniform1f(const std::string& name, const float v0) {
    glUniform1f(GetUniformLocation(name), v0);
}



void Shader::SetUniform4f(const std::string& name,
                          const float v0, const float v1,
                          const float v2, const float v3) {

    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform4f(const std::string& name, const glm::vec4& vec) {
    SetUniform4f(name, vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4 mat) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

int Shader::GetUniformLocation(const std::string& name) const {

    int location;

    if (m_uniform_cache.find(name) == m_uniform_cache.end()) {
        location  = glGetUniformLocation(m_renderer_id, name.c_str());
        m_uniform_cache[name] = location;
    } else {
        location = m_uniform_cache[name];
    }

    if (location == -1) {
        std::cerr << "! Uniform '" << name << "' does not exist!" << std::endl;
        std::cout << m_sources.vertexShaderSrc << std::endl;
    }

    return location;
}

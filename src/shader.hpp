#pragma once
#include "utils.hpp"
#include <unordered_map>

struct ShaderSources {
    std::string vertexShaderSrc;
    std::string fragmentShaderSrc;
};

class Shader {

public:

    Shader() : m_renderer_id(0) {};
    Shader(const ShaderSources sources) { Renew(sources); };
    ~Shader() { glDeleteProgram(m_renderer_id); };


    inline unsigned int GetID() const { return m_renderer_id; }

    void Bind() const;
    void UnBind() const;
    void Renew(const ShaderSources sources);


    void SetUniform1i(const std::string& name, const int v0);
    void SetUniform1f(const std::string& name, const float v0);

    void SetUniform4f(const std::string& name,
                      const float v0, const float v1,
                      const float v2, const float v3);
    void SetUniform4f(const std::string& name, const glm::vec4& vec);



    void SetUniformMat4f(const std::string& name, const glm::mat4 mat);


    static ShaderSources  GetShaderSources(const std::string& vertexPath,
                                           const std::string& fragmentPath) {

        ShaderSources sources;
        std::ifstream is;
        std::stringstream ss;

        is = std::ifstream(vertexPath);
        ss << is.rdbuf();
        sources.vertexShaderSrc = ss.str();

        ss.str(std::string());

        is = std::ifstream(fragmentPath);
        ss << is.rdbuf();
        sources.fragmentShaderSrc = ss.str();

        return sources;
    }

private:
    ShaderSources m_sources;
    unsigned int m_renderer_id;
    mutable std::unordered_map<std::string, int> m_uniform_cache;

    int GetUniformLocation(const std::string& name) const;


    static unsigned int CompileShader(unsigned int type,
                                      const std::string& source) {

        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();

        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        return id;
    }

    static unsigned int CreateShaderProgram(ShaderSources sources) {

        unsigned int program;
        unsigned int vs = 0;
        unsigned int fs = 0;

        program = glCreateProgram();

        if (!sources.vertexShaderSrc.empty()) {
            vs = CompileShader(GL_VERTEX_SHADER, sources.vertexShaderSrc);
            glAttachShader(program, vs);
        }

        if (!sources.fragmentShaderSrc.empty()) {
            fs = CompileShader(GL_FRAGMENT_SHADER, sources.fragmentShaderSrc);
            glAttachShader(program, fs);
        }


        glLinkProgram(program);
        glValidateProgram(program);

        if (vs) {
            glDetachShader(program, vs);
            glDeleteShader(vs);
        }
        if (fs) {
            glDetachShader(program, fs);
            glDeleteShader(fs);
        }

        return program;
    }


};

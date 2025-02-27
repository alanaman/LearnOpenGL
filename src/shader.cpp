#include "shader.hpp"

ShaderProgramSource ParseShader(const std::string& filepath);

    Shader::Shader(const std::string& filepath)
    {
        ShaderProgramSource source = ParseShader(filepath);

        unsigned int program = glCreateProgram();
        unsigned int vs = CompileShader(GL_VERTEX_SHADER, source.VertexSource);
        unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, source.FragmentSource);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);
        ID = program;
    }
    void Shader::use()
    {
        (glUseProgram(ID));
    }
    void Shader::setBool(const std::string &name, bool value) const
    {         
        (glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value)); 
    }
    void Shader::setInt(const std::string &name, int value) const
    { 
        (glUniform1i(glGetUniformLocation(ID, name.c_str()), value)); 
    }
    void Shader::setFloat(const std::string &name, float value) const
    { 
        (glUniform1f(glGetUniformLocation(ID, name.c_str()), value)); 
    }
    void Shader::setMat4(const std::string &name, glm::mat4 value) const
    { 
        (glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)));
    }
    void Shader::setVec3(const std::string &name, float x, float y, float z) const
    {
        (glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z));
    }
    void Shader::setVec3(const std::string &name, glm::vec3 value) const
    {
        (glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z));
    }

    void Shader::deleteShader()
    {
        glDeleteShader(ID);
    }
    unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
    {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                << " shader!" << std::endl;
            std::cout << message << std::endl;

            glDeleteShader(id);
            return 0;
        }
        return id;
    }



ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(), ss[1].str() };
}



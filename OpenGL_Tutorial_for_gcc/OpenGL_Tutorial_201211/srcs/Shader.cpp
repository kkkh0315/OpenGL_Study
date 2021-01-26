#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformProj = 0;
	uniformModel = 0;
    uniformView = 0;
    uniformColor = 0;
    uniformAmbientIntensity = 0;
    uniformDirection = 0;
    uniformDiffuseIntensity = 0;
    uniformShininess = 0;
    uniformSpecularIntensity = 0;
    uniformEyePosition = 0;
}

void Shader::CreateFromString(const char* vsCode, const char* fsCode)
{
	CompileShader(vsCode, fsCode);
}

void Shader::CreateFromFiles(const char* vsPath, const char* fsPath)
{
    std::string vsFile = ReadFile(vsPath);
    std::string fsFile = ReadFile(fsPath);
    const char* vsCode = vsFile.c_str();
    const char* fsCode = fsFile.c_str();

    CompileShader(vsCode, fsCode);
}

std::string	Shader::ReadFile(const char* fPath)
{
    std::string content;
    std::ifstream fileStream(fPath, std::ios::in);

    while (!fileStream.is_open())
    {
        printf("Failed to read %s file! The file doesn't exist.", fPath);
        return "";
    }

    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

GLuint	Shader::GetProjLocation()
{
    return uniformProj;
}

GLuint	Shader::GetModelLocation()
{
    return uniformModel;
}

GLuint  Shader::GetViewLocation()
{
    return uniformView;
}

GLuint  Shader::GetColorLocation()
{
    return uniformColor;
}

GLuint  Shader::GetAmbientIntensityLocation()
{
    return uniformAmbientIntensity;
}

GLuint	Shader::GetDirectionLocation()
{
    return uniformDirection;
}

GLuint  Shader::GetDiffuseIntensityLocation()
{
    return uniformDiffuseIntensity;
}

GLuint Shader::GetShininessLocation()
{
    return uniformShininess;
}

GLuint Shader::GetSpecularIntensityLocation()
{
    return uniformSpecularIntensity;
}

GLuint Shader::GetEyePositionLocation()
{
    return uniformEyePosition;
}

void	Shader::CompileShader(const char* vsCode, const char* fsCode)
{
    GLuint vs, fs;
    
    shaderID = glCreateProgram();

    if (!shaderID)
    {
        printf("Error: Cannot create shader program.");
        return;
    }

    vs = AddShader(vsCode, GL_VERTEX_SHADER);
    fs = AddShader(fsCode, GL_FRAGMENT_SHADER);
    glAttachShader(shaderID, vs);  // Attach shaders to the program for linking process.
    glAttachShader(shaderID, fs);

    GLint result = 0;
    GLchar err_log[1024] = { 0 };

    glLinkProgram(shaderID);  // Create executables from shader codes to run on corresponding processors.
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(err_log), NULL, err_log);
        printf("Error linking program: '%s'\n", err_log);
        return;
    }

    // glValidateProgram is typically useful only during application development.
    glValidateProgram(shaderID); // Checks to see whether the executables contained in program can execute given the current OpenGL state.
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(err_log), NULL, err_log);
        printf("Error validating program: '%s'\n", err_log);
        return;
    }

    uniformModel = glGetUniformLocation(shaderID, "model"); // glLinkProgram initializes uniform variables to 0
    uniformProj = glGetUniformLocation(shaderID, "proj");   // This means that glGetUniformLocation should be called
    uniformView = glGetUniformLocation(shaderID, "view");   // after linking the program via glLinkProgram.
    uniformColor = glGetUniformLocation(shaderID, "directionalLight.color");
    uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.ambientIntensity");
    uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
    uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.diffuseIntensity");
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
    uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
    uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
}

GLuint	Shader::AddShader(const char* shaderCode, GLenum shaderType)
{
    GLuint new_shader = glCreateShader(shaderType);

    const GLchar* code[1];
    code[0] = shaderCode;

    glShaderSource(new_shader, 1, code, NULL);

    GLint result = 0;
    GLchar err_log[1024] = { 0 };

    glCompileShader(new_shader);
    glGetShaderiv(new_shader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(new_shader, sizeof(err_log), NULL, err_log);
        printf("Error compiling the %d shader: '%s'\n", shaderType, err_log);
        return 0;
    }
    return new_shader;
}

void Shader::UseShader()
{
    if (shaderID != 0)
        glUseProgram(shaderID);
    else
        printf("Error: Cannot find a shader program.");
}

void Shader::ClearShader()
{
    if (shaderID != 0)
    {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }
    
    shaderID = 0;
    uniformProj = 0;
    uniformModel = 0;
    uniformView = 0;
    uniformColor = 0;
    uniformAmbientIntensity = 0;
    uniformDirection = 0;
    uniformDiffuseIntensity = 0;
    uniformShininess = 0;
    uniformSpecularIntensity = 0;
    uniformEyePosition = 0;
}

Shader::~Shader()
{
    ClearShader();
}
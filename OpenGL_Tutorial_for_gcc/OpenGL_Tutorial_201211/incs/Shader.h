#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>

class Shader
{
public:
				Shader();

	void		CreateFromString(const char* vsCode, const char* fsCode);
	void		CreateFromFiles(const char* vsPath, const char* fsPath);
	std::string	ReadFile(const char* fPath);

	GLuint		GetProjLocation();
	GLuint		GetModelLocation();
	GLuint		GetViewLocation();
	GLuint		GetColorLocation();
	GLuint		GetAmbientIntensityLocation();
	GLuint		GetDirectionLocation();
	GLuint		GetDiffuseIntensityLocation();
	GLuint		GetShininessLocation();
	GLuint		GetSpecularIntensityLocation();
	GLuint		GetEyePositionLocation();

	void		UseShader();
	void		ClearShader();
				
				~Shader();

private:
	GLuint		shaderID, uniformProj, uniformModel, uniformView, uniformEyePosition,
				uniformColor, uniformAmbientIntensity, uniformDirection, uniformDiffuseIntensity,
				uniformSpecularIntensity, uniformShininess;

	void		CompileShader(const char* vsCode, const char* fsCode);
	GLuint		AddShader(const char* shaderCode, GLenum shaderType);
};


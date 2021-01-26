#pragma once

#include <GL/glew.h>
#include <GLM/glm.hpp>

class Light
{
public:
				Light();
				Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, 
					GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);

	void		UseLight(GLuint ambientIntensityLocation, GLuint colorLocation,
						GLuint diffuseIntensityLocation, GLuint directionLocation);

				~Light();

private:
	// ambient light
	glm::vec3	color;
	GLfloat		ambientIntensity;

	//diffuse light
	glm::vec3	direction;
	GLfloat		diffuseIntensity;
};


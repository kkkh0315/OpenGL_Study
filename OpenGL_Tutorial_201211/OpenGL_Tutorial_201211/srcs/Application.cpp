#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <stdio.h>
#include <string.h>
#include <vector>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Time.h"
#include "Texture.h"
#include "Light.h"
#include "Material.h"

// Global Variables
Window              mainWindow;
Camera              camera;
std::vector<Mesh*>  meshList;
std::vector<Shader> shaderList;
Texture             pinksmoke;
Texture             silver;
Time                time;
Light               mainLight;
Material            metal;
Material            matte;

static const char*  vShader = "shaders/shader.vert";
static const char*  fShader = "shaders/shader.frag";

void calculateAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
    unsigned int iLength, unsigned int vLength, unsigned int normalOffset)
{
    for (unsigned int i = 0; i < indiceCount; i += iLength)
    {
        unsigned int in[3];
        in[0] = indices[i] * vLength;
        in[1] = indices[i + 1] * vLength;
        in[2] = indices[i + 2] * vLength;

        glm::vec3 v1(vertices[in[1]] - vertices[in[0]], vertices[in[1] + 1] - vertices[in[0] + 1], vertices[in[1] + 2] - vertices[in[0] + 2]);
        glm::vec3 v2(vertices[in[2]] - vertices[in[0]], vertices[in[2] + 1] - vertices[in[0] + 1], vertices[in[2] + 2] - vertices[in[0] + 2]);
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        for (int i = 0; i < 3; i++)
        {
            vertices[in[i] + normalOffset] += normal.x;
            vertices[in[i] + normalOffset + 1] += normal.y;
            vertices[in[i] + normalOffset + 2] += normal.z;
        }
    }

    for (unsigned int i = 0; i < verticeCount; i += vLength)
    {
        glm::vec3 normal(vertices[i + normalOffset], vertices[i + normalOffset + 1], vertices[i + normalOffset + 2]);
        normal = glm::normalize(normal);
        vertices[i + normalOffset] = normal.x;
        vertices[i + normalOffset + 1] = normal.y;
        vertices[i + normalOffset + 2] = normal.z;
    }
}

void CreateObjects()
{
    GLuint indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
//   x      y     z         u     v         nx    ny    nz
    -1.0f, -1.0f, -1.0f,    0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
     0.0f, -1.0f, 1.0f,     0.5f, 0.0f,     0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, -1.0f,    1.0f, 0.0f,     0.0f, 0.0f, 0.0f,
     0.0f,  1.0f, 0.0f,     0.5f, 1.0f,     0.0f, 0.0f, 0.0f
    };
    calculateAverageNormals(indices, 12, vertices, 32, 3, 8, 5);

    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 32, 12); // Create proper VAO, VBO, IBO of obj1
    meshList.push_back(obj1);

    return;
}

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

int main(void)
{
    const float pi = 3.14159265f;
    const float toRadian = pi / 180.0f;

    // Create Window
    mainWindow = Window(1960, 1020);
    mainWindow.initWindow();

    // Create Camera
    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

    // Set projection
    glm::mat4 proj(1.0f);
    proj = glm::perspective(glm::radians(66.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

    // Set light
    mainLight = Light(1.0f, 1.0f, 1.0f, 0.2f, 
                      2.0f, 0.0f, 0.0f, 1.0f);

    // Set textures
    pinksmoke = Texture("textures/pinksmoke.png");
    pinksmoke.LoadTexture();
    silver = Texture("textures/silver.png");
    silver.LoadTexture();


    // Set materials
    metal = Material(0.7f, 32); // shininess is usually set to power of 2
    matte = Material(0.01f, 1);

    // Create Objects
    CreateObjects();

    // Create Shaders
    CreateShaders();

    GLuint uniformProj = 0;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;
    GLuint uniformColor = 0;
    GLuint uniformEyePosition = 0;
    GLuint uniformAmbientIntensity = 0;
    GLuint uniformDirection = 0;
    GLuint uniformDiffuseIntensity = 0;
    GLuint uniformShininess = 0;
    GLuint uniformSpecularIntensity = 0;

    float rot = 0.0f;
    float rotIncrement = 1.0f;

    time = Time();

    while (!mainWindow.getShouldClose())
    {
        camera.keyControl(mainWindow.getKeys(), time.getDeltaTime());
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        glClearColor(1.0f, 0.4f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();
        uniformProj = shaderList[0].GetProjLocation();
        uniformModel = shaderList[0].GetModelLocation();
        uniformView = shaderList[0].GetViewLocation();
        uniformColor = shaderList[0].GetColorLocation();
        uniformEyePosition = shaderList[0].GetEyePositionLocation();
        uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();
        uniformDirection = shaderList[0].GetDirectionLocation();
        uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();
        uniformShininess = shaderList[0].GetShininessLocation();
        uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();

        mainLight.UseLight(uniformAmbientIntensity, uniformColor, uniformDiffuseIntensity, uniformDirection);

        rot += rotIncrement;
        if (rot > 360)
            rot -= 360;

        /*
        void glUniform()

        glUniform modifies the value of a uniform variable or uniform variable array in the used program.
        */
        // uniformProj, uniformView, uniformEyePosition is initialized only once
        glUniformMatrix4fv(uniformProj, 1, GL_FALSE, glm::value_ptr(proj)); //glUseProgram 이후에 model을 설정해야함
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
        
        // uniformModel should be initialized per mesh
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::rotate(model, rot * toRadian, glm::vec3(0.0f, 1.0f, 0.0f));
        // model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f)); no need to scale cuz I can move around
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        silver.UseTexture();
        metal.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[0]->RenderMesh();

        glUseProgram(0);

        mainWindow.swapBuffers();

        glfwPollEvents();
        /*
        void glfwPollEvents(void)

        glfwPollEvents() processes only those events that have already been received and then returns immediately.
        Processing events will cause the window and input callbacks associated with those events to be called.
        */
    }

    glfwTerminate();
    return 0;
}
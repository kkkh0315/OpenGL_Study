## Directional Light
  * Simplest form of light.
  * Necessary information: color, ambient, diffuse, specular, direction.
  * Treat all light calculations using the same direction for the light vector.
  * Don't need to calculate a light vector. (Infinite light)
  * [How to calculate ambient, diffuse, specular](https://github.com/kkkh0315/OpenGL_Study/blob/main/09%20Lighting:%20Phong%20Lightings.md)

## Implementing Directional Light with C++ and OpenGL
  * First, create a light object.
    ~~~~
    #include "Light.h"

    Light::Light()
    {
      color = glm::vec3(1.0f, 1.0f, 1.0f);
      ambientIntensity = 1.0f;

      direction = glm::vec3(0.0f, -1.0f, 0.0f);
      diffuseIntensity = 0.2f;
    }

    Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
          GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity)
    {
      color = glm::vec3(red, green, blue);
      ambientIntensity = aIntensity;

      direction = glm::vec3(xDir, yDir, zDir);
      diffuseIntensity = dIntensity;
    }

    void Light::UseLight(GLuint ambientIntensityLocation, GLuint colorLocation,
              GLuint diffuseIntensityLocation, GLuint directionLocation)
    {
      glUniform1f(ambientIntensityLocation, ambientIntensity);
      glUniform3f(colorLocation, color.x, color.y, color.z);

      glUniform1f(diffuseIntensityLocation, diffuseIntensity);
      glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    }

    Light::~Light() {}
    ~~~~
    1. The constructor receives rgb values, the intensity of ambient light, xyz light directions, and the intensity of diffuse light.
  * In the main, create a light object and get locations of the uniforms that represent light color, light direction, ambient light intensity, and diffuse light intensity. Then, pass those values to UseLight method of the object to actaully use the light.
    ~~~~
    int main()
    {
      (...)
      mainLight = Light(1.0f, 1.0f, 1.0f, 0.2f, 2.0f, 0.0f, 0.0f, 1.0f);

      (...)
      while (!close_window)
      {
      (...)
        uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();
        uniformDirection = shaderList[0].GetDirectionLocation();
        uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();
        uniformShininess = shaderList[0].GetShininessLocation();
        uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
        
        mainLight.UseLight(uniformAmbientIntensity, uniformColor, uniformDiffuseIntensity, uniformDirection);
        (...)
      }
      (...)
    }
    ~~~~
  * Now, create a material object for specular light.
    ~~~~
    #include "Material.h"

    Material::Material()
    {
      specularIntensity = 0;
      shininess = 0;
    }

    Material::Material(GLfloat sIntensity, GLfloat shine)
    {
      specularIntensity = sIntensity;
      shininess = shine;
    }

    void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
    {
      glUniform1f(specularIntensityLocation, specularIntensity);
      glUniform1f(shininessLocation, shininess);
    }

    Material::~Material() {}
    ~~~~
      1. The constructor receives the intensity of specular, and the shininess of the material.
  * Like the light object, create a material object in main. The parameters could be set according to what kind of material you want to create. Get necessary uniform locations. Then, use UseMaterial method of the created material.
    ~~~~
    int main()
    {
      (...)
      metal = Material(0.7f, 32); // shininess is usually set to power of 2

      (...)
      while (!close_window)
      {
      (...)
        uniformEyePosition = shaderList[0].GetEyePositionLocation();
        uniformShininess = shaderList[0].GetShininessLocation();
        uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
        (...)
        metal.UseMaterial(uniformSpecularIntensity, uniformShininess);
      }
      (...)
    }
    ~~~~
  * Change the vertices array. We need normals of vertices.
    ~~~~
    void CreateObjects()
    {
      (...)
      GLfloat vertices[] = {
      -1.0f, -1.0f, -1.0f,    0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
       0.0f, -1.0f, 1.0f,     0.5f, 0.0f,     0.0f, 0.0f, 0.0f,
       1.0f, -1.0f, -1.0f,    1.0f, 0.0f,     0.0f, 0.0f, 0.0f,
       0.0f,  1.0f, 0.0f,     0.5f, 1.0f,     0.0f, 0.0f, 0.0f
      };
    }
    ~~~~
  * Normals of the vertices are going to be calculated by averaging the normals of the faces that the vertex is included. calculateAverageNormals function will do it. Normals are going to be then interpolated across the face. 
    ~~~~
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
    ~~~~
    1. indices: indices array in CreateObject function.
    2. indiceCount: Number of elements in indices array.
    3. verticies: verticies array in CreateObject function.
    4. vertexCount: Number of elements in verticies array.
    5. iLength: Number of elements contained in one face in indices array.
    6. vLength: Number of elements contained in one vertex, including location, texture uv, normal vectors, in vertices array.
    7. normalOffset: Number of elements that should be jumped from the location values to reach the normal values in verticies array.
  * Call calculateAverageNormals underneath the vertices array.
    ~~~~
    void CreateObjects()
    {
    (...)
      GLfloat vertices[] = {
      -1.0f, -1.0f, -1.0f,    0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
       0.0f, -1.0f, 1.0f,     0.5f, 0.0f,     0.0f, 0.0f, 0.0f,
       1.0f, -1.0f, -1.0f,    1.0f, 0.0f,     0.0f, 0.0f, 0.0f,
       0.0f,  1.0f, 0.0f,     0.5f, 1.0f,     0.0f, 0.0f, 0.0f
      };
      calculateAverageNormals(indices, 12, vertices, 32, 3, 8, 5);
    (...)
    }
    ~~~~
  * Be careful of the values passed to CreateMesh method of the mesh objects you create. Also, be careful with the parameters you pass to glVertexAttribPointer function in CreateMesh method because the vertices array has changed. Then, call glVertexAttribPointer once again in CreateMesh method of Mesh object for the normal values add in vertices array.
    ~~~~
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 5));
    ~~~~
  * Finally, write vertex shader and fragment shader.
    1. vertex shader
    ~~~~
    #version 330

    layout(location = 0) in vec3 pos;
    layout(location = 1) in vec2 tex;
    layout(location = 2) in vec3 nor;

    out vec4 vColor;
    out vec2 TexCoord;
    out vec3 Normal;
    out vec3 FragPos;

    uniform mat4 model;
    uniform mat4 proj;
    uniform mat4 view;

    void main()
    {
        gl_Position = proj * view * model * vec4(pos, 1.0);
        vColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
        TexCoord = tex;

        Normal = mat3(transpose(inverse(model))) * nor;
        FragPos = (model * vec4(pos, 1.0)).xyz;
    }
    ~~~~
    2. fragment shader
    ~~~~
    #version 330

    in vec4 vColor;
    in vec2 TexCoord;
    in vec3 Normal;
    in vec3 FragPos;

    out vec4 color;

    struct DirectionalLight
    {
        vec3 color;
        float ambientIntensity;

        vec3 direction;
        float diffuseIntensity;
    };

    struct Material
    {
        float specularIntensity;
        float shininess;
    };

    uniform sampler2D theTexture;
    uniform DirectionalLight directionalLight;
    uniform Material material;
    uniform vec3 eyePosition;

    void main()
    {
        vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;

        float diffuseFactor = max(dot(normalize(directionalLight.direction), normalize(Normal)), 0);
        vec4 diffuseColor = vec4(directionalLight.color, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

        vec4 specularColor = vec4(0, 0, 0, 0);
        if (diffuseFactor > 0.0f)
        {
            vec3 fragToEye = normalize(eyePosition - FragPos);
            vec3 reflectedLight = normalize(reflect(directionalLight.direction, normalize(Normal)));
            float specularFactor = dot(fragToEye, reflectedLight);
            if (specularFactor > 0.0f)
            {
                specularFactor = pow(specularFactor, material.shininess);
                specularColor = vec4(directionalLight.color, 1.0f) * material.specularIntensity * specularFactor;
            }
        }

        color = texture(theTexture, TexCoord) * (ambientColor + diffuseColor + specularColor);
    }
    ~~~~

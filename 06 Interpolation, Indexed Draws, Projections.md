## Interpolation
  * Per-vertex attributes passed on are "interpolated" using the other values on the primitve.
      * That is, a weighted average of the three vertices on a triangle is passed on.
  * Fragment shader picks up the interpolated value and uses that.
  * The value is effectively an estimate of what the value should be at that position, had we difined it ourself.
  * Happens in Rasterization stage in the pipeline.
  * Usage
      * Interpolation is used for quickly and accuartely estimating values without defining them.
      * Can be used for interpolating Texture Co-ordinates when mapping textures.
      * Can be Used for interpolating Normal Vectors when handling lighting.
      * Especially useful in Phong Shading to create the illusion of smooth/rounded surfaces.
## Indexed Draws
  * Indexing vertices to prevent programmer from defining one vertex multiple times.
## Projection
  * How things are visualized.
  * Used to convert from 'view space' to 'clip space'.
  * Understanding co-ordinate systems is crucial.
      * Co-ordinate systems
          1. Local Space: Raw position of each vertex drawn relative to origin.
              * Multiply by Model Matrix to get world space.
          2. World Space: Position of vertex in the world itself if camera is assumed to be positioned at the origin.
              * Multiply by View Matrix to get view space.
          3. View Space: Position of vertex in the world, relative to the camera position and orientation.
              * Multiply by Projection Matrix to get clip space.
          4. Clip Space: Position of vertex in the world, relative to the camera position and orientation, as viewed in the are not to be 'clipped' from the final output.
              * To create clip space, we define an area, frustum, of what is not to be clipped, with a Projection Matrix.
          5. Screen Space: AFter clipping takes place, the final image is created and placed on the co-ordinate system of the window itself.
  * Two commonly used types of Projection
      1) Orthographic
      2) Perspective
  * Projections with GLM and OpenGL
      * glm::mat4 proj = glm::perspective(fov, aspect ratio of the viewport(width/height), distance of the near plane from camera, distance of the far plane from camera)
      * Bind the given matrix to a uniform in the shader.
      * When multiplying matrices, ORDER DOES MATTER!!!
          * gl_Position = projection matrix * view matrix * model matrix * vec4(pos, 1.0);
## 전체 코드(Interpolation, Indexed Draw, Projection 적용)
    ~~~~
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <GLM/glm.hpp>
    #include <GLM/gtc/matrix_transform.hpp>
    #include <GLM/gtc/type_ptr.hpp>

    #include <stdio.h>
    #include <string.h>

    const GLint WIDTH = 800, HEIGHT = 600;
    const float pi = 3.14159265f;
    const float toRadian = pi / 180.0f;

    /*
    ** Vertex Shader
    */

    static const char* vShader = "                                \n\
    #version 330                                                  \n\
                                                                  \n\
    layout(location = 0) in vec3 pos;                             \n\
                                                                  \n\
    out vec4 vColor;                                              \n\
                                                                  \n\
    uniform mat4 model;                                           \n\
    uniform mat4 proj;                                            \n\
                                                                  \n\
    void main()                                                   \n\
    {                                                             \n\
        gl_Position = proj * model * vec4(pos.x, pos.y, pos.z, 1.0);;    \n\
        vColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);              \n\
    }";

    /*
    ** Fragment Shader
    */

    static const char* fShader = "               \n\
    #version 330                                 \n\
                                                 \n\
    out vec4 color;                              \n\
                                                 \n\
    in vec4 vColor;                              \n\
                                                 \n\
    void main()                                  \n\
    {                                            \n\
        color = vColor;        \n\
    }";

    void CreateTriangle(GLuint* tri_VAO, GLuint* tri_VBO, GLuint* tri_IBO)
    {
        GLuint indices[] = {
            0, 3, 1,
            1, 3, 2,
            2, 3, 0,
            0, 1, 2
        };

        GLfloat tri_pos[] = {
        -1.0f, -1.0f, 0.0f,
         0.0f, -1.0f, 1.0f,
         0.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        };

        glGenVertexArrays(1, tri_VAO);
        glBindVertexArray(*tri_VAO);

        glGenBuffers(1, tri_IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *tri_IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glGenBuffers(1, tri_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, *tri_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tri_pos), tri_pos, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0); // You should unbind VBO and IBO AFTER unbinding VAO!
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        return;
    }

    GLuint AddShader(const char* shaderCode, GLenum shaderType)
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

    GLuint CompileShaders()
    {
        GLuint vs, fs;
        GLuint shader_program = glCreateProgram();

        if (!shader_program)
        {
            printf("Error: Cannot create shader program.");
            return 0;
        }

        vs = AddShader(vShader, GL_VERTEX_SHADER);
        fs = AddShader(fShader, GL_FRAGMENT_SHADER);
        glAttachShader(shader_program, vs);
        glAttachShader(shader_program, fs);

        GLint result = 0;
        GLchar err_log[1024] = { 0 };

        glLinkProgram(shader_program);
        glGetProgramiv(shader_program, GL_LINK_STATUS, &result);
        if (!result)
        {
            glGetProgramInfoLog(shader_program, sizeof(err_log), NULL, err_log);
            printf("Error linking program: '%s'\n", err_log);
            return 0;
        }

        glValidateProgram(shader_program);
        glGetProgramiv(shader_program, GL_VALIDATE_STATUS, &result);
        if (!result)
        {
            glGetProgramInfoLog(shader_program, sizeof(err_log), NULL, err_log);
            printf("Error validating program: '%s'\n", err_log);
            return shader_program;
        }

        return shader_program;
    }

    int main(void)
    {
        if (!glfwInit())
        {
            printf("Error: Cannot init GLFW.");
            glfwTerminate();
            return -1;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        GLFWwindow* window = glfwCreateWindow(1080, 720, "Hello World", NULL, NULL);
        if (!window)
        {
            printf("Error: Cannot create window.");
            glfwTerminate();
            return -1;
        }

        int frame_buf_width, frame_buf_height;
        glfwGetFramebufferSize(window, &frame_buf_width, &frame_buf_height);
        glViewport(0, 0, frame_buf_width, frame_buf_height);

        glfwMakeContextCurrent(window);

        glfwSwapInterval(1);

        glewExperimental = GL_TRUE;

        if (glewInit() != GLEW_OK)
        {
            printf("Error: Cannot init GLEW.");
            glfwDestroyWindow(window);
            glfwTerminate();
            return -1;
        }

        glEnable(GL_DEPTH_TEST);

        GLuint tri_VAO = 0;
        GLuint tri_VBO = 0;
        GLuint tri_IBO = 0;

        CreateTriangle(&tri_VAO, &tri_VBO, &tri_IBO);
        GLuint shader_program = CompileShaders();

        glm::mat4 proj(1.0f);
        proj = glm::perspective(30.0f * toRadian, (GLfloat)frame_buf_width / (GLfloat)frame_buf_height, 0.1f, 100.0f);

        GLuint uniformModel = glGetUniformLocation(shader_program, "model");
        GLuint uniformProj = glGetUniformLocation(shader_program, "proj");

        bool direction = true;
        float triOffset = 0.0f;
        float triMaxOffset = 0.5f;
        float triIncrement = 0.0005f;

        float rot = 0.0f;
        float rotIncrement = 1.0f;

        while (!glfwWindowShouldClose(window))
        {
            glClearColor(1.0f, 0.4f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glUseProgram(shader_program);

            if (direction)
                triOffset += triIncrement;
            else
                triOffset -= triIncrement;
            if (triOffset >= triMaxOffset || -triOffset >= triMaxOffset)
                direction = !direction;

            rot += rotIncrement;
            if (rot > 360)
                rot -= 360;

            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
            model = glm::rotate(model, rot * toRadian, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
            glUniformMatrix4fv(uniformProj, 1, GL_FALSE, glm::value_ptr(proj));  //glUseProgram 이후에 model을 설정해야함
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));  //glUseProgram 이후에 model을 설정해야함

            glBindVertexArray(tri_VAO);
            glBindBuffer(GL_ARRAY_BUFFER, tri_VBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri_IBO);
            glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glUseProgram(0);

            glfwSwapBuffers(window);

            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }
    ~~~~

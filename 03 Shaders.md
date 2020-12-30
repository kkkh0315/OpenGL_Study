## Shader란?
  * Shader is a block of codes that runs on GPU, not CPU like .cpp file.
  * Use shader in order to tell our GPUs what to do.
  * Two types of shaders are going to be used mainly.
      * Vertex shaders
          * Vertex shader is called once per vertex.
          * It tells OpenGL where the vertex should locate in your window.
          * How? It passes vertex attributes to the next level, which could be frament shaders, but there are many more things in between.
      * Fragment shaders(or pixel shaders)
          * Fragment shader is called once per pixel to be filled in.
          * It tells OpenGL which color the pixel should be.
      * Fragment shaders are usually called alot more than vertex shaders. Therefore, we need to utilize vertex shader wisely for optimization. However, there are some things that need to be done by fragment shaders, such as calculating lighting for each pixel.

  * **Vertex Shader, Fragment Shader 활용하여 분홍 바탕에 빨간 삼각형 그리기**
    ~~~~
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <stdio.h>
    #include <string.h>

    GLuint tri_VAO;
    GLuint tri_VBO;
    GLuint shader_program;

    /*
    ** Vertex Shader
    */

    static const char* vShader = "                                \n\
    #version 330                                                  \n\
                                                                  \n\
    layout(location = 0) in vec3 pos;                             \n\
                                                                  \n\
    void main()                                                   \n\
    {                                                             \n\
        gl_Position = vec4(0.7 * pos.x, 0.7 * pos.y, pos.z, 1.0); \n\
    }";

    /*
    ** Fragment Shader
    */

    static const char* fShader = "               \n\
    #version 330                                 \n\
                                                 \n\
    out vec4 color;                              \n\
                                                 \n\
    void main()                                  \n\
    {                                            \n\
        color = vec4(1.0, 0.0, 0.0, 1.0);        \n\
    }";

    void CreateTriangle()
    {
        GLfloat tri_pos[9] = {
        -1.0f, -1.0f, 0.0f,
         0.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        };

        glGenVertexArrays(1, &tri_VAO);
        glBindVertexArray(tri_VAO);
        glGenBuffers(1, &tri_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, tri_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tri_pos), tri_pos, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        return;
    }

    void AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
    {
        GLuint new_shader = glCreateShader(shaderType);

        const GLchar* code[1];
        code[0] = shaderCode;

        GLint code_len[1];
        code_len[0] = strlen(shaderCode);

        glShaderSource(new_shader, 1, code, code_len);

        GLint result = 0;
        GLchar err_log[1024] = { 0 };

        glCompileShader(new_shader);
        glGetShaderiv(new_shader, GL_COMPILE_STATUS, &result);
        if (!result)
        {
            glGetShaderInfoLog(new_shader, sizeof(err_log), NULL, err_log);
            printf("Error compiling the %d shader: '%s'\n", shaderType, err_log);
            return;
        }

        glAttachShader(program, new_shader);
    }

    void CompileShaders()
    {
        shader_program = glCreateProgram();

        if (!shader_program)
        {
            printf("Error: Cannot create shader program.");
            return;
        }

        AddShader(shader_program, vShader, GL_VERTEX_SHADER);
        AddShader(shader_program, fShader, GL_FRAGMENT_SHADER);

        GLint result = 0;
        GLchar err_log[1024] = { 0 };

        glLinkProgram(shader_program);
        glGetProgramiv(shader_program, GL_LINK_STATUS, &result);
        if (!result)
        {
            glGetProgramInfoLog(shader_program, sizeof(err_log), NULL, err_log);
            printf("Error linking program: '%s'\n", err_log);
            return;
        }

        glValidateProgram(shader_program);
        glGetProgramiv(shader_program, GL_VALIDATE_STATUS, &result);
        if (!result)
        {
            glGetProgramInfoLog(shader_program, sizeof(err_log), NULL, err_log);
            printf("Error validating program: '%s'\n", err_log);
            return;
        }
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

        glewExperimental = GL_TRUE;

        if (glewInit() != GLEW_OK)
        {
            printf("Error: Cannot init GLEW.");
            glfwDestroyWindow(window);
            glfwTerminate();
            return -1;
        }

        CreateTriangle();
        CompileShaders();

        while (!glfwWindowShouldClose(window))
        {
            glClearColor(1.0f, 0.4f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shader_program);
            glBindVertexArray(tri_VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);
            glUseProgram(0);

            glfwSwapBuffers(window);

            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }
    ~~~~

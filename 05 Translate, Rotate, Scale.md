## Translate, Rotate, Scale
  * Vertex Shader 설정
      ~~~~
      #version 330                                                  

      layout(location = 0) in vec4 pos;                             

      uniform mat4 model;                                            

      void main()                                                   
      {                                                             
          gl_Position = model * vec4(pos.x, pos.y, pos.z, 1.0);;
      }";
      ~~~~
  * main 함수
      ~~~~
      int main(void)
      {
          (중략)

          GLuint uniformModel = glGetUniformLocation(shader_program, "model");
          LOOP
          {
              glm::mat4 model(1.0f);
              model = glm::translate(model, glm::vec3(0.5f,0.0f, 0.0f));
              model = glm::rotate(model, triOffset * toRadian, glm::vec3(0.0f, 0.0f, 1.0f));
              model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
              glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
          }
          (중략)
      }
      ~~~~
  * LOOP 안에 model uniform을 설정할 때, glUseProgram 함수를 호출하기 전에 설정을 했더니 도형이 나타나지 않았다. glUseProgram을 호출한 이후에 uniform을 설정하도록 하자!
## 전체 코드 (Translate, Rotate, Scale 적용)
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
 layout(location = 0) in vec4 pos;                             \n\
 uniform mat4 model;                                            \n\
                                                               \n\
 void main()                                                   \n\
 {                                                             \n\
     gl_Position = model * vec4(pos.x, pos.y, pos.z, 1.0);; \n\
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

 GLuint CreateTriangle(GLuint tri_VAO, GLuint tri_VBO)
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
     return tri_VAO;
 }

 GLuint AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
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

     vs = AddShader(shader_program, vShader, GL_VERTEX_SHADER);
     fs = AddShader(shader_program, fShader, GL_FRAGMENT_SHADER);
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
         return 0;
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

     GLuint tri_VAO = 0;
     GLuint tri_VBO = 0;

     tri_VAO = CreateTriangle(tri_VAO, tri_VBO);
     GLuint shader_program = CompileShaders();

     GLuint uniformModel = glGetUniformLocation(shader_program, "model");
     bool direction = true;
     float triOffset = 0.0f;
     float triMaxOffset = 360.0f;
     float triIncrement = 1.0f;

     while (!glfwWindowShouldClose(window))
     {
         glClearColor(1.0f, 0.4f, 0.3f, 1.0f);
         glClear(GL_COLOR_BUFFER_BIT);

         glUseProgram(shader_program);

         if (direction)
             triOffset += triIncrement;
         else
             triOffset -= triIncrement;
         if (triOffset >= triMaxOffset || -triOffset >= triMaxOffset)
             direction = !direction;

         glm::mat4 model(1.0f);
         model = glm::translate(model, glm::vec3(0.5f,0.0f, 0.0f));
         model = glm::rotate(model, triOffset * toRadian, glm::vec3(0.0f, 0.0f, 1.0f));
         model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
         glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));  //glUseProgram 이후에 model을 설정해야함

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

## 화면 그리기

  * Window를 만드는 역할은 GLFW가 한다. 따라서 GLFW를 Init한다.
      ~~~~
      if(!glfwInit())
      {
          glfwTerminate();
          return -1;
      }
      ~~~~
  * Window를 만들기 전에 설정할 조건들이 있다. 이 조건들은 window를 만들거나 OpenGL context를 만들기 전에 설정해야한다.
      ~~~~
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 버전
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1); // 3.1에 맞춰 실행
      //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 더 이상 쓰이지 않는 함수들 에러 처리
      //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 상위 호환성 지원
      ~~~~
      (아래 두 함수를 주어진 인자들과 호출했을 땐 window가 뜨지 않는 현상이 생겼다. 원인을 찾아봐야겠다.(
  * Window 만들기
      ~~~~
      GLFWwindow* window = glfwCreateWindow(1080, 720, "Hello World", NULL, NULL);
      if (!window)
      {
          glfwTerminate();
          return -1;
      }
      ~~~~
  * Window의 Framebuffer 사이즈를 구하는 작업이 그 뒤 진행된다. 해당 사이즈로 뷰포트를 생성할 수 있다.
      ~~~~
      int frame_buf_width, frame_buf_height;
      glfwGetFramebufferSize(window, &frame_buf_width, &frame_buf_height);
      glViewport(0, 0, frame_buf_width, frame_buf_height);
      ~~~~
  * 현재 만들어진 Window가 OpenGL context가 적용되어야 함을 GLEW에게 전달하기 위한 작업이 이어진다.
      ~~~~
      glfwMakeContextCurrent(window);
      ~~~~
  * 이제 GLEW를 Init한다.
      ~~~~
      if (glewInit() != GLEW_OK)
      {
          glfwDestroyWindow(window); // glfwCreateWindow()로 만든 window 제거
          glfwTerminate();
          return -1;
      }
      ~~~~
  * 이제 그림을 그릴 루프를 만든다. 매 프레임마다 호출되어 window에 그림을 그린다.
      ~~~~
      while (!glfwWindowShouldClose(window))
      {
          glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
          glClear(GL_COLOR_BUFFER_BIT);

          /* Swap front and back buffers */
          glfwSwapBuffers(window);

          /* Poll for and process events */
          glfwPollEvents();
      }
      ~~~~
  * OpenGL은 유한 상태 기계 모델로 작동한다. (OpenGL is a state machine.)
    * 유한 상태 기계란 유한한 개수의 상태를 가질 수 있는 추상 기계로, 한번에 오로지 하나의 상태만을 가지며, 어떠한 사건(event)에 의해 한 상태에서 다른 상태로 전이한다. 특정한 유한 상태 기계는 현재 상태로부터 가능한 전이 상태와, 이러한 전이를 유발하는 조건들의 집합으로서 정의된다.
    * OpenGL은 유한 상태 기계로 수많은 상태를 저장해두고 명령을 받으면 현재 상태를 바탕으로 작동한다.
  * **Vertex Buffer**
    * Legacy OpenGL을 활용하여 삼각형 그리기
        ~~~~
        // Legacy OpenGL을 활용하여 삼각형 그리기
        LOOP
        {
            glBegin(GL_TRIANGLES);
            glVertex2f(-0.5f, -0.5f);
            glVertex2f( 0.0f,  0.5f);
            glVertex2f( 0.5f, -0.5f);
            glEnd();
        }
        ~~~~
    * Modern OpenGL을 활용하여 삼각형 그리기 (Vertex Buffer 사용)
      ~~~~
      //Modern OpenGL을 활용하여 삼각형 그리기

      float tri_pos[6] = {
          -0.5f, -0.5f,
           0.0f,  0.5f,
           0.5f, -0.5f
      };

      unsigned int tri_buf;
      glGenBuffers(1, &tri_buf);
      glBindBuffer(GL_ARRAY_BUFFER, tri_buf);
      glBufferData(GL_ARRAY_BUFFER, sizeof(tri_pos), tri_pos, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
      LOOP 
      {
          glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
          glClear(GL_COLOR_BUFFER_BIT);

          glDrawArrays(GL_TRIANGLES, 0, 3);

          /* Swap front and back buffers */
          glfwSwapBuffers(window);

          /* Poll for and process events */
          glfwPollEvents();
      }
      ~~~~
      * 파라미터 설명
          * glGenBuffer(버퍼 몇개 생성할 것인지, 버퍼 아이디를 담을 unsigned int 포인터): 버퍼 생성
          * glBindBuffer(버퍼 종류, 버퍼 아이디): 버퍼 선택 (선택을 해야 아래 작업들을 할 수 있다. 중요!)
          * glBufferData(버퍼 종류, 버퍼 크기이자 버퍼에 담을 데이터의 총 크기, 버퍼에 담을 데이터, 버퍼 사용 방법): 버퍼에 데이터 담기
              * 버퍼 사용 방법 규정 방식
                  * STATIC: 한 번만 정의하고 매 프레임마다 매번 사용할 버퍼
                  * DYNAMIC: 정의를 동적으로 바꾸면서 매 프레임마다 매펀 사용할 버퍼
          * glVertexAttribPointer(Attribute Index, 한 Attribute 안에 담길 데이터의 갯수, 그 데이터의 종류, stride(한 Vertex의 총 크기. 즉, 모든 Attribute의 크기의 합), Attribute의 첫 시작 지점까지의 offset(즉, 앞에 있는 Attributes의 크기의 합)): Vertex의 Attribute 내용 설정
          * glEnableVertexAttribArray(Attribute Index): 해당 Attribute을 enable 시킴

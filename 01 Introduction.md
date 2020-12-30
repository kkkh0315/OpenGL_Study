# OpenGL이란

## OpenGL이란?
* OpenGL을 library라고 생각했다. 하지만 OpenGL은 설명서(Specification)와 같은 것이다. 42로 따지면 과제마다 나오는 subject 처럼 함수의 이름, 기능, 인자, 작동 방식 등이 설명된 설명서이다.
  * 그렇다면 OpenGL 문서에 작성된 내용을 누가 구현하느냐? 메모리카드 제조업체가 구현하는 것이다. 당연히 메모리카드 제조업체는 한 군데가 아니며, 따라서 OpenGL의 구현도 하나가 아니다.
  * 이렇게 작성된 코드들의 집합을 OpenGL API(Application Programming Interface)라고 부른다. API란 운영 프로그램에서 사용할 수 있도록 운영 체제나 프로그래밍 언어가 제공하는 기능을 제어할 수 있게 만드는 인터페이스이다.

## GLFW(Graphics Library Framework)이란?
  * GLFW(Graphics Library Framework)는 OpenGL과 함께 쓰는 유틸리티 라이브러리이다.
  * 창을 생성, 제어할 수 있게 해주고, OpenGL context를 생성 및 관리할 수 있게 해준다.
  * 키 입력과 이벤트 생성 등도 할 수 있게 해준다.

## Visual Studio 2019에서 OpenGL API와 GLFW 사용하기
  * OpenGL은 다운로드 받는 게 아니다. 그래픽카드에 포함되어 있다. 따라서, 탑재된 OpenGL API를 사용하면 된다.
  * GLFW는 다운로드 받아야 한다. 나는 정적 라이브러리로 이미 컴파일 되어있는 GLFW를 다운받아 Visual Studio로 하여금 자동적으로 이를 링크시키도록 설정해두었다.
  
## GLEW(OpenGL Extension Wrangler)란?
  * GLEW는 크로스플랫폼 C/C++ 라이브러리로서 OpenGL extension을 쿼리하고 로드하는 기능을 수행합니다.  
  * OpenGL을 windows 환경에서 사용할 때 opengl32.h 파일을 추가합니다. 하지만 이 헤더파일은 windows에서 만든 파일로 windows 환경에 맞춰있습니다. 더불어 OpenGL 1.x 버전 이후로 업데이트 되지 않았습니다. 따라서 최신 OpenGL을 사용하기 위해선 익스텐션 라이브러리가 필요합니다.
  * OpenGL의 최신 구현은 그래픽 카드에 탑재되어있습니다. 이를 사용하기 위해 필요한 것이 익스텐션 라이브러리입니다. 익스텐션 라이브러리는 그래픽 카드 드라이버의 dll(동적 라이브러리)에 탑재되어있는 OpenGL 함수들의 포인터들을 불러와 자체적으로 정의한 OpenGL 함수들과 연결합니다. 우리는 OpenGL 함수들을 GLEW를 통하여 사용하게 되는 것입니다.

## GLEW를 Visual Studio 2019에서 사용하기
  * GLEW를 다운받아 GLFW를 VS에서 사용했듯이 사용하면 됩니다. 솔루션 속성에서 헤더파일의 경로를 추가하고, 정적 라이브러리를 링크하도록 그 이름과 주소를 저장해두면 GLEW Library를 사용할 수 있습니다.
  * **GLEW의 헤더파일은 가장 우선적으로 include 되어야 합니다.**  
      ~~~~
      #include <GL/glew.h>
      ~~~~
  * **GLEW를 사용하기 위해선 glewInit()을 호출해야하는데, 꼭 OpenGL context를 생성하고 호출해야합니다.**
      > First you need to create a valid OpenGL rendering context and call glewInit() to initialize the extension entry points. If glewInit() returns GLEW_OK, the initialization succeeded and you can use the available extensions as well as core OpenGL functionality. For example:
      ~~~~
      #include <GL/glew.h>
      #include <GL/glut.h>
      ...
      glutInit(&argc, argv);
      glutCreateWindow("GLEW Test");
      GLenum err = glewInit();
      if (GLEW_OK != err)
      {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        ...
      }
      fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
      ~~~~

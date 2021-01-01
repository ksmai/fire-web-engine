#include <iostream>
#include <emscripten.h>
#include <GLES3/gl3.h>
#include <SDL.h>
#include <cmath>

const float square[] = {
  -0.5f, 0.5f, 0.0f,
  0.5f, 0.5f, 0.0f,
  -0.5f, -0.5f, 0.0f,
  0.5f, -0.5f, 0.0f,
};

GLuint vbo, vertexShader, fragmentShader, shaderProgram, uColor;

const char* vertexShaderSource = "#version 300 es\nlayout (location = 0) in vec3 aPos;\nvoid main() {gl_Position = vec4(aPos, 1.0);}";

const char* fragmentShaderSource = "#version 300 es\nprecision mediump float;\nuniform vec3 uColor;\nout vec4 FragColor;\nvoid main() { FragColor = vec4(uColor, 1.0); }";

class Game {
public:
  Game():
    currentTimestamp{emscripten_get_now()}
  {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow(
      "MySDLWindowTitle",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      640,
      480,
      SDL_WINDOW_OPENGL
    );
    if (window == nullptr) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s", SDL_GetError());
    }

    context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create gl context: %s", SDL_GetError());
    }

    std::cout << "GL Version = " << glGetString(GL_VERSION) << "\n";
    std::cout << "GLSL Version = " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

    glGenBuffers(1, &vbo);

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      char info[512];
      glGetShaderInfoLog(vertexShader, 512, nullptr, info);
      std::cout << "vertex shader no success: " << info << "\n";
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      char info[512];
      glGetShaderInfoLog(fragmentShader, 512, nullptr, info);
      std::cout << "fragment shader no success: " << info << "\n";
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
      char info[512];
      glGetProgramInfoLog(shaderProgram, 512, nullptr, info);
      std::cout << "shaderProgram no success: " << info << "\n";
    }

    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    uColor = glGetUniformLocation(shaderProgram, "uColor");
  }

  void update() {
    lastTimestamp = currentTimestamp;
    currentTimestamp = emscripten_get_now();
    std::cout << "dt = " << (currentTimestamp - lastTimestamp) << "\n";

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glUseProgram(shaderProgram);
    glUniform3f(uColor, 0.0f, 0.0f, std::sin(currentTimestamp/100.0)/2.0 +0.5);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }

private:
  double lastTimestamp, currentTimestamp;
  SDL_Window* window;
  SDL_GLContext context;
};

void loop(void* arg) {
  Game* gameLogic = static_cast<Game*>(arg);
  gameLogic->update();
}

extern "C" {
  EMSCRIPTEN_KEEPALIVE
  void start() {
    emscripten_set_main_loop_arg(loop, new Game{}, 0, false);
  }
}

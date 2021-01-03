#include <iostream>
#include <emscripten.h>
#include <emscripten/fetch.h>
#include <GLES3/gl3.h>
#include <SDL/SDL.h>
#include <cmath>
#include <cstring>
#include "Shader/Shader.h"
#include "Shader/Program.h"
#include "Resource/ResourceCache.h"
#include "Resource/StringLoader.h"
#include "Resource/RawLoader.h"
#include "Process/DelayProcess.h"
#include "Process/ProcessManager.h"

const float square[] = {
  -0.5f, 0.5f, 0.0f,
  0.5f, 0.5f, 0.0f,
  -0.5f, -0.5f, 0.0f,
  0.5f, -0.5f, 0.0f,
};

GLuint vbo, uColor;
FW::Program program;
FW::ResourceCache resourceCache;
FW::ProcessManager processManager;



class Game {
public:
  void init() {
    currentTimestamp = emscripten_get_now();

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

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    const char* vertexShaderSource = reinterpret_cast<const char*>(
      resourceCache.getResource("nested/DefaultVertex.glsl")->buffer()
    );
    const char* fragmentShaderSource = reinterpret_cast<const char*>(
      resourceCache.getResource("nested/nested2/DefaultFragment.glsl")->buffer()
    );
    FW::Shader vertexShader{FW::Shader::createVertexShader(vertexShaderSource)};
    FW::Shader fragmentShader{FW::Shader::createFragmentShader(fragmentShaderSource)};
    program = FW::Program{vertexShader, fragmentShader};
    uColor = glGetUniformLocation(program.get(), "uColor");

    initialized = true;

    FW::Process::StrongPtr parentProcess{new FW::DelayProcess{3000.0}};
    FW::Process::StrongPtr childProcess{new FW::DelayProcess{2000.0}};
    parentProcess->attachChild(std::move(childProcess));
    processManager.attachProcess(std::move(parentProcess));
  }

  void update() {
    lastTimestamp = currentTimestamp;
    currentTimestamp = emscripten_get_now();
    // std::cout << "dt = " << (currentTimestamp - lastTimestamp) << "\n";

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    program.use();
    float red = std::sin(currentTimestamp / 307.0) / 2.0 + 0.5;
    float green = std::sin(currentTimestamp / 509.0 + 41.0) / 2.0 + 0.5;
    float blue = std::sin(currentTimestamp / 203.0 + 27.0) / 2.0 + 0.5;
    glUniform3f(uColor, red, green, blue);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    processManager.update(currentTimestamp - lastTimestamp);
  }

  bool hasInitialized() const {
    return initialized;
  }

private:
  double lastTimestamp, currentTimestamp;
  SDL_Window* window;
  SDL_GLContext context;
  bool initialized = false;

};

void loop(void* arg) {
  Game* gameLogic = static_cast<Game*>(arg);
  if (!resourceCache.isLoading()) {
    if (!gameLogic->hasInitialized()) {
      gameLogic->init();
    } else {
      gameLogic->update();
    }
  }
}



extern "C" {
  EMSCRIPTEN_KEEPALIVE
  void start() {
    resourceCache.addLoader(new FW::StringLoader);
    resourceCache.addLoader(new FW::RawLoader);
    resourceCache.loadZipFile("/resources.zip");
    emscripten_set_main_loop_arg(loop, new Game{}, 0, false);
  }
}

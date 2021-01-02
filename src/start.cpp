#include <iostream>
#include <emscripten.h>
#include <emscripten/fetch.h>
#include <GLES3/gl3.h>
#include <SDL/SDL.h>
#include <cmath>
#include <cstring>
#include "Shader/Shader.h"
#include "Shader/Program.h"
#include "Resource/Fetcher.h"
#include "Resource/ZipFile.h"

const float square[] = {
  -0.5f, 0.5f, 0.0f,
  0.5f, 0.5f, 0.0f,
  -0.5f, -0.5f, 0.0f,
  0.5f, -0.5f, 0.0f,
};

GLuint vbo, uColor;
FW::Program program;

char* vertexShaderSource = nullptr;

char* fragmentShaderSource = nullptr;

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

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    FW::Shader vertexShader{FW::Shader::createVertexShader(vertexShaderSource)};
    FW::Shader fragmentShader{FW::Shader::createFragmentShader(fragmentShaderSource)};
    program = FW::Program{vertexShader, fragmentShader};
    uColor = glGetUniformLocation(program.get(), "uColor");
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

void onVertexShaderDownloaded(emscripten_fetch_t* fetch) {
  std::cout << "Downloaded " << fetch->numBytes << " bytes from " << fetch->url << "\n";
  vertexShaderSource = new char[fetch->numBytes + 1];
  for (uint64_t i = 0; i < fetch->numBytes; ++i) {
    vertexShaderSource[i] = fetch->data[i];
  }
  vertexShaderSource[fetch->numBytes] = '\0';
  emscripten_fetch_close(fetch);
  if (fragmentShaderSource) {
    emscripten_set_main_loop_arg(loop, new Game{}, 0, false);
  }
}

void onFragmentShaderDownloaded(emscripten_fetch_t* fetch) {
  std::cout << "Downloaded " << fetch->numBytes << " bytes from " << fetch->url << "\n";
  fragmentShaderSource = new char[fetch->numBytes + 1];
  for (uint64_t i = 0; i < fetch->numBytes; ++i) {
    fragmentShaderSource[i] = fetch->data[i];
  }
  fragmentShaderSource[fetch->numBytes] = '\0';
  emscripten_fetch_close(fetch);
  if (vertexShaderSource) {
    emscripten_set_main_loop_arg(loop, new Game{}, 0, false);
  }
}

void onDownloadFailed(emscripten_fetch_t* fetch) {
  std::cout << "Download failed: " << fetch->url << " [" << fetch->status << "]\n";
  emscripten_fetch_close(fetch);
}

void onZipDownloaded(emscripten_fetch_t* fetch) {
  std::cout << "Zip file downloaded: " << fetch->url << " [" << fetch->numBytes << " bytes]\n";
  FW::ZipFile file{fetch->data, fetch->numBytes};
  emscripten_fetch_close(fetch);
}

extern "C" {
  EMSCRIPTEN_KEEPALIVE
  void start() {
    FW::Fetcher fetcher;
    fetcher.fetch("/DefaultVertex.glsl", onVertexShaderDownloaded, onDownloadFailed);
    fetcher.fetch("/DefaultFragment.glsl", onFragmentShaderDownloaded, onDownloadFailed);
    fetcher.fetch("/resources.zip", onZipDownloaded, onDownloadFailed);
  }
}

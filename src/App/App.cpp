#include "App/App.h"
#include "Resource/StringLoader.h"
#include "Resource/RawLoader.h"

// for temp testing
#include <cmath>
#include "Graphics/Shader.h"

FW::App::App(const Config& config) {
  resourceCache.addLoader(new StringLoader{});
  resourceCache.addLoader(new RawLoader{});
  resourceCache.loadZipFile(config.sharedResourceFile);

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    throw "SDL_Init failed";
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  window = SDL_CreateWindow(
    config.title.c_str(),
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    config.canvasWidth,
    config.canvasHeight,
    SDL_WINDOW_OPENGL
  );
  if (window == nullptr) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s", SDL_GetError());
    throw "SDL_CreateWindow failed";
  }

  context = SDL_GL_CreateContext(window);
  if (context == nullptr) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create gl context: %s", SDL_GetError());
    throw "SDL_GL_CreateContext failed";
  }

  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "GL Version = %d", glGetString(GL_VERSION));
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "GLSL Version = %d", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

FW::App::~App() {
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void FW::App::init() {
  if (initialized || resourceCache.isLoading()) {
    return;
  }

  // for temp testing
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  const float square[] = {
    -0.5f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
  };

  const unsigned int elements[] = {0, 1, 2, 3};

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

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

  FW::Process::StrongPtr parentProcess{new FW::DelayProcess{3000.0}};
  FW::Process::StrongPtr childProcess{new FW::DelayProcess{2000.0}};
  parentProcess->attachChild(std::move(childProcess));
  processManager.attachProcess(std::move(parentProcess));

  initialized = true;
}

void FW::App::update() {
  if (!initialized) {
    init();
    return;
  }
  clock.update();
  double dt = clock.dt();

  processManager.update(dt);

  // for temp testing
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glBindVertexArray(vao);
  program.use();
  float red = std::sin(clock.time() / 307.0) / 2.0 + 0.5;
  float green = std::sin(clock.time() / 509.0 + 41.0) / 2.0 + 0.5;
  float blue = std::sin(clock.time() / 203.0 + 27.0) / 2.0 + 0.5;
  glUniform3f(uColor, red, green, blue);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

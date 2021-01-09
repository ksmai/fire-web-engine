#include "SDL_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "App/App.h"
#include "Resource/TextLoader.h"
#include "Resource/RawLoader.h"
#include "Resource/ImageLoader.h"

// for temp testing
#include <cmath>
#include "Graphics/Shader.h"

FW::App::App(const Config& config) {
  resourceCache.addLoader(new TextLoader{});
  resourceCache.addLoader(new ImageLoader{});
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
    -0.5f,  0.5f, 0.0f,  0.0f, 2.0f,
     0.5f,  0.5f, 0.0f,  2.0f, 2.0f,
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
     0.5f, -0.5f, 0.0f,  2.0f, 0.0f,
  };

  const unsigned int elements[] = {0, 1, 2, 3};

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // texture
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  Resource::Buffer img{resourceCache.getResource("demo/face.png")->buffer()};
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  const char* vertexShaderSource = reinterpret_cast<const char*>(
    resourceCache.getResource("demo/DefaultVertex.glsl")->buffer()
  );
  const char* fragmentShaderSource = reinterpret_cast<const char*>(
    resourceCache.getResource("demo/DefaultFragment.glsl")->buffer()
  );
  FW::Shader vertexShader{FW::Shader::createVertexShader(vertexShaderSource)};
  FW::Shader fragmentShader{FW::Shader::createFragmentShader(fragmentShaderSource)};
  program = FW::Program{vertexShader, fragmentShader};
  uColor = glGetUniformLocation(program.get(), "uColor");
  uModelTransform = glGetUniformLocation(program.get(), "uModelTransform");

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
  glUniform3f(uColor, 1.0f, 1.0f, 1.0f);
  glm::mat4 t{glm::rotate(glm::mat4{1.0f}, static_cast<float>(clock.time()/100.0f), glm::vec3{0.0f, 0.0f, 1.0f})};
  glUniformMatrix4fv(uModelTransform, 1, GL_FALSE, glm::value_ptr(t));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBindTexture(GL_TEXTURE_2D, texture);
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glm::vec2 myVec{42.42f, 3.14159f};
  SDL_Log("myVec.x = %f", myVec.x);
}

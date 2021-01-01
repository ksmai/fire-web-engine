#ifndef __SHADER_H__
#define __SHADER_H__

#include <GLES3/gl3.h>

class Shader {
public:
  using Type = GLuint;
  using Source = const char*;
  using Ref = GLuint;

  Shader(const Shader&) =delete;

  Shader(Shader&&);

  Shader& operator=(const Shader&) =delete;

  ~Shader();

  static Shader createVertexShader(Source);

  static Shader createFragmentShader(Source);

  Ref get() const;

private:
  Shader(Type, Source);

  void compile(Source source) const;

  GLuint shader;
};

#endif

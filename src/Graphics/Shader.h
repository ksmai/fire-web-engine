#ifndef __SHADER_H__
#define __SHADER_H__

#include "GLES3/gl3.h"

namespace FW {
  class Shader {
  public:
    using Source = const unsigned char*;
    using Length = const unsigned int;

    friend class Program;

    Shader(const Shader&) =delete;
    Shader& operator=(const Shader&) =delete;
    ~Shader();

    static Shader createVertexShader(Source, Length);
    static Shader createFragmentShader(Source, Length);

  private:
    Shader(GLuint type, Source, Length);

    void compile(Source source, Length) const;

    GLuint get() const;

    GLuint shader;
  };
}

#endif

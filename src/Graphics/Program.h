#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "GLES3/gl3.h"
#include "Graphics/Shader.h"

namespace FW {
  class Program {
  public:
    using Ref = GLuint;

    Program();

    Program(const Shader& vertexShader, const Shader& fragmentShader);

    Program(Program&&);

    Program& operator=(Program&&);

    Program(const Program&) =delete;

    Program& operator=(const Program&) =delete;

    ~Program();

    void use() const;

    Ref get() const;

  private:
    GLuint program;
  };
}

#endif

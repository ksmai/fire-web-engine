#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "GLES3/gl3.h"
#include "Graphics/Shader.h"

namespace FW {
  class Program {
  public:
    Program(Shader::Source, Shader::Length, Shader::Source, Shader::Length);
    Program(const Program&) =delete;
    Program& operator=(const Program&) =delete;
    ~Program();
    GLuint get() const;
    void use() const;

  private:
    GLuint program;
  };
}
#endif

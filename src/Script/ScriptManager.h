#ifndef __SCRIPT_MANAGER_H__
#define __SCRIPT_MANAGER_H__

#include <string>
#include "lua-5.4.2/src/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "App/Logger.h"

namespace FW {
  class ScriptManager {
  public:
    class Function {
    public:
      Function(const std::string& name, const luabridge::LuaRef& ref):
        name{name},
        ref{ref}
      {
      }

      Function(const Function&) =delete;
      Function& operator=(const Function&) =delete;
      Function(Function&&) =default;

      void operator()() {
        if (ref.isFunction()) {
          try {
            ref();
          } catch (const luabridge::LuaException& e) {
            Logger::error("An error occurred when calling Lua function %s: %s", name.c_str(), e.what());
          }
        } else {
          Logger::info("Calling Lua function that does not exist: %s", name.c_str());
        }
      }

      template <typename P1>
      void operator()(P1 p1) {
        if (ref.isFunction()) {
          try {
            ref(p1);
          } catch (const luabridge::LuaException& e) {
            Logger::error("An error occurred when calling Lua function %s: %s", name.c_str(), e.what());
          }
        } else {
          Logger::info("Calling Lua function that does not exist: %s", name.c_str());
        }
      }

      template <typename P1, typename P2>
      void operator()(P1 p1, P2 p2) {
        if (ref.isFunction()) {
          try {
            ref(p1, p2);
          } catch (const luabridge::LuaException& e) {
            Logger::error("An error occurred when calling Lua function %s: %s", name.c_str(), e.what());
          }
        } else {
          Logger::info("Calling Lua function that does not exist: %s", name.c_str());
        }
      }

      template <typename P1, typename P2, typename P3>
      void operator()(P1 p1, P2 p2, P3 p3) {
        if (ref.isFunction()) {
          try {
            ref(p1, p2, p3);
          } catch (const luabridge::LuaException& e) {
            Logger::error("An error occurred when calling Lua function %s: %s", name.c_str(), e.what());
          }
        } else {
          Logger::info("Calling Lua function that does not exist: %s", name.c_str());
        }
      }

      template <typename P1, typename P2, typename P3, typename P4>
      void operator()(P1 p1, P2 p2, P3 p3, P4 p4) {
        if (ref.isFunction()) {
          try {
            ref(p1, p2, p3, p4);
          } catch (const luabridge::LuaException& e) {
            Logger::error("An error occurred when calling Lua function %s: %s", name.c_str(), e.what());
          }
        } else {
          Logger::info("Calling Lua function that does not exist: %s", name.c_str());
        }
      }

      template <typename P1, typename P2, typename P3, typename P4, typename P5>
      void operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) {
        if (ref.isFunction()) {
          try {
            ref(p1, p2, p3, p4, p5);
          } catch (const luabridge::LuaException& e) {
            Logger::error("An error occurred when calling Lua function %s: %s", name.c_str(), e.what());
          }
        } else {
          Logger::info("Calling Lua function that does not exist: %s", name.c_str());
        }
      }

      template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
      void operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) {
        if (ref.isFunction()) {
          try {
            ref(p1, p2, p3, p4, p5, p6);
          } catch (const luabridge::LuaException& e) {
            Logger::error("An error occurred when calling Lua function %s: %s", name.c_str(), e.what());
          }
        } else {
          Logger::info("Calling Lua function that does not exist: %s", name.c_str());
        }
      }

      template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
      void operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) {
        if (ref.isFunction()) {
          try {
            ref(p1, p2, p3, p4, p5, p6, p7);
          } catch (const luabridge::LuaException& e) {
            Logger::error("An error occurred when calling Lua function %s: %s", name.c_str(), e.what());
          }
        } else {
          Logger::info("Calling Lua function that does not exist: %s", name.c_str());
        }
      }

      template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
      void operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) {
        if (ref.isFunction()) {
          try {
            ref(p1, p2, p3, p4, p5, p6, p7, p8);
          } catch (const luabridge::LuaException& e) {
            Logger::error("An error occurred when calling Lua function %s: %s", name.c_str(), e.what());
          }
        } else {
          Logger::info("Calling Lua function that does not exist: %s", name.c_str());
        }
      }

    private:
      const std::string name;
      const luabridge::LuaRef ref;
    };

    ScriptManager();
    ScriptManager(const ScriptManager&) =delete;
    ScriptManager& operator=(const ScriptManager&) =delete;
    ~ScriptManager();

    bool runLua(const std::string&);

    Function getFunction(const std::string&);

    template <typename ReturnType, typename... Params>
    void addFunction(const char* name, ReturnType(*fp)(Params...)) {
      luabridge::getGlobalNamespace(L)
        .beginNamespace(luaNamespace)
        .addFunction(name, fp)
        .endNamespace();
    }

    template <typename T, typename Ctor>
    void addClassConstructor(const char* className) {
      luabridge::getGlobalNamespace(L)
        .beginNamespace(luaNamespace)
        .beginClass<T>(className)
        .template addConstructor<Ctor>()
        .endClass()
        .endNamespace();
    }

    template <typename T, typename ReturnType, typename... Params>
    void addClassMethod(const char* className, const char* methodName, ReturnType(T::*fp)(Params...)) {
      luabridge::getGlobalNamespace(L)
        .beginNamespace(luaNamespace)
        .beginClass<T>(className)
        .addFunction(methodName, fp)
        .endClass()
        .endNamespace();
    }

    template <typename T, typename MemberType>
    void addClassVariable(const char* className, const char* variableName, MemberType T::* variable) {
      luabridge::getGlobalNamespace(L)
        .beginNamespace(luaNamespace)
        .beginClass<T>(className)
        .addProperty(variableName, variable)
        .endClass()
        .endNamespace();
    }

  private:
    static const char* luaNamespace;
    static void luaDebug(const char*);

    lua_State* L;
  };
}
#endif

#include "App/abort.h"
#include "Script/ScriptManager.h"

const char* FW::ScriptManager::luaNamespace{"FW"};

void FW::ScriptManager::luaDebug(const char* message) {
  Logger::info("Lua debug: %s", message);
}

FW::ScriptManager::ScriptManager():
  L{luaL_newstate()}
{
  luaL_openlibs(L);
  beginNamespace()
    .addFunction("debug", &luaDebug)
    .endNamespace();
}

FW::ScriptManager::~ScriptManager() {
  lua_close(L);
}

bool FW::ScriptManager::runLua(const std::string& luaSource) {
  bool error = (luaL_loadstring(L, luaSource.c_str()) || lua_pcall(L, 0, LUA_MULTRET, 0));
  if (error) {
    Logger::error("Error in Lua: %s", lua_tostring(L, -1));
  }
  return error;
}

FW::ScriptManager::Function FW::ScriptManager::getFunction(const std::string& name) {
  return Function{name, luabridge::getGlobal(L, name.c_str())};
}

luabridge::Namespace FW::ScriptManager::beginNamespace() const {
  return luabridge::getGlobalNamespace(L).beginNamespace(luaNamespace);
}

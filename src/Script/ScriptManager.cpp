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
  addFunction("debug", &luaDebug);
}

FW::ScriptManager::~ScriptManager() {
  lua_close(L);
}

bool FW::ScriptManager::runLua(const std::string& luaSource) {
  return luaL_dostring(L,  luaSource.c_str());
}

FW::ScriptManager::Function FW::ScriptManager::getFunction(const std::string& name) {
  return Function{name, luabridge::getGlobal(L, name.c_str())};
}

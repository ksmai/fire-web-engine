#include "File/XMLFile.h"
#include "Game/SpriteSheetRepository.h"

FW::SpriteSheetRepository::SpriteSheetRepository(Graphics* graphics):
  graphics{graphics}
{
}

void FW::SpriteSheetRepository::load(ZipFile& resources) {
  if (!resources.hasFile("SpriteSheets.xml")) {
    return;
  }
  XMLFile xml{resources.getFileContent("SpriteSheets.xml")};
  for (auto& node : xml.root().find("SpriteSheet")) {
    ID id{std::stoul(node.attr("id"))};
    std::size_t width{std::stoul(node.attr("width"))};
    std::size_t height{std::stoul(node.attr("height"))};
    const char* marginAttr{node.attr("margin")};
    std::size_t margin{marginAttr ? std::stoul(marginAttr) : 0};
    const char* path{node.attr("path")};
    add(id, graphics->makeSpriteSheet(resources.getFileContent(path), width, height, margin));
  }
}

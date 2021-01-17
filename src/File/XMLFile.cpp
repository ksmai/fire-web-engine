#include <cstring>
#include "File/XMLFile.h"

FW::XMLFile::XMLFile(Data&& data):
  src{data.begin(), data.end()}
{
  doc.parse<0>(&src[0]);
  // data can be freed automatically by its destructor after being copied into src
  // src is modified by rapidxml at this point and should serve only
  // as a container for data
}

FW::XMLFile::XMLNode FW::XMLFile::root() {
  return XMLNode{doc.first_node()};
}

FW::XMLFile::XMLNode::XMLNode(rapidxml::xml_node<>* node):
  node{node}
{
}

const char* FW::XMLFile::XMLNode::attr(const char* name) {
  auto attr = node->first_attribute(name);
  return attr ? attr->value() : nullptr;
}

std::vector<FW::XMLFile::XMLNode> FW::XMLFile::XMLNode::find(const char* name) {
  std::vector<XMLNode> result;
  for (auto child = node->first_node(name); child; child = child->next_sibling(name)) {
    result.emplace_back(child);
  }
  return result;
}

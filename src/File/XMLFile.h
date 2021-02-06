#ifndef __XML_FILE_H__
#define __XML_FILE_H__

#include <string>
#include <vector>
#include "rapidxml.hpp"
#include "File/Data.h"

namespace FW {
  class XMLFile {
  public:
    class XMLNode {
    public:
      XMLNode(rapidxml::xml_node<>*);
      // get first attribute value. ignore the rest with the same name
      // returns nullptr if attribute doesn't exist
      const char* attr(const char*);
      std::vector<XMLNode> find(const char*);

    private:
      rapidxml::xml_node<>* node;
    };

    XMLFile(Data&&);
    XMLFile(const XMLFile&) =delete;
    XMLFile& operator=(const XMLFile&) =delete;
    ~XMLFile() =default;

    XMLNode root();

  private:
    std::string src;
    rapidxml::xml_document<> doc;
  };
}
#endif

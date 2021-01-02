#include <iostream>
#include "Resource/ZipFile.h"

FW::ZipFile::ZipFile(ZipFileData data, Size size) {
  std::size_t signaturePosition = findEndOfCentralDirectorySignature(data, size);
  std::cout << "Found at " << signaturePosition << "\n";
}

FW::ZipFile::Size FW::ZipFile::findEndOfCentralDirectorySignature(ZipFileData data, Size size) const {
  Size bytesSearched = 0;
  Size start = size - 22;
  while (bytesSearched < (1<<16) && start >= bytesSearched) {
    std::size_t i = start - bytesSearched;
    if (data[i] == 0x50 && data[i+1] == 0x4b && data[i+2] == 0x05 && data[i+3] == 0x06) {
      return i;
    }
  }
  throw "Invalid zip file: end of central directory signature not found";
}
